/*******************************************************************************
 * Copyright IBM Corp. and others 2024
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] https://openjdk.org/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0-only WITH Classpath-exception-2.0 OR GPL-2.0-only WITH OpenJDK-assembly-exception-1.0
 *******************************************************************************/

#include "optimizer/ConstRefRematerialization.hpp"
#include "optimizer/Optimization_inlines.hpp"

#include "il/Block.hpp"
#include "il/Node.hpp"
#include "il/TreeTop.hpp"
#include "infra/ILWalk.hpp"

#define INVALID_AID UINT32_MAX
#define INVALID_SCI UINT32_MAX

// Bound-checked access with a fatal assertion. The method vector::at throws an
// exception if an out-of-bounds access is attempted, which will just fail the
// compilation and could easily go unnoticed.
template<typename T>
static typename TR::vector<T, TR::Region &>::reference at(TR::vector<T, TR::Region &> &v,
    typename TR::vector<T, TR::Region &>::size_type i)
{
    TR_ASSERT_FATAL(i < v.size(), "index out of bounds");
    return v[i];
}

TR::ConstRefRematerialization::ConstRefRematerialization(TR::OptimizationManager *manager)
    : Optimization(manager)
    , _memRegion(NULL)
    , _srnToAid(NULL)
    , _autoInfos(NULL)
{}

namespace {

struct StackEntry {
    uint32_t _aid;
    uint32_t _edgeIndex;
};

struct IndexRange {
    IndexRange(uint32_t start, uint32_t end)
        : _start(start)
        , _end(end)
    {}

    uint32_t _start;
    uint32_t _end;
};

} // namespace

int32_t TR::ConstRefRematerialization::perform()
{
    if (!comp()->useConstRefs()) {
        return 0;
    }

    TR::KnownObjectTable *knot = comp()->getKnownObjectTable();
    if (knot == NULL) {
        return 0;
    }

    TR_ASSERT_FATAL(knot->getEndIndex() >= 1 && knot->isNull(0), "expect null index at 0");
    if (knot->getEndIndex() == 1) {
        return 0;
    }

    TR::Region &stackRegion = comp()->trMemory()->currentStackRegion();
    _memRegion = &stackRegion;

    bool trace = this->trace();
    OMR::Logger *log = comp()->log();

    // Overview of the algorithm:
    //
    // 1. Find all definitions of address-typed autos (other than known object
    //    temps). Assign a small sequential ID to each relevant auto, called an
    //    "auto ID" (AID), and for each one allocate an AutoInfo in _autoInfos
    //    (via getAid()). Analyze the definitions to (a) build a graph of which
    //    autos are copied into which others (_copyFrom and _copyTo), and (b)
    //    for any auto that has non-copy definitions (_seenNonCopyDef),
    //    determine whether they are all the same known object, and if so, which
    //    one (_koi, where KOI stands for known object index).
    //
    // 2. Identify strongly-connected components in the directed graph of autos,
    //    where a -> b iff there statically exists a copy from a to b (anywhere).
    //    That is, a -> b iff b is in a._copyTo, iff a is in b._copyFrom.
    //
    // 3. Analyze each strong component in topological order. That is, before
    //    a strong component can be analyzed, we must first analyze all of its
    //    predecessors, which are the strong components containing autos whose
    //    values are copied (anywhere) into the current strong component. Within
    //    a strong component, all copying auto defs come from the same strong
    //    component or from a predecessor. Treat copies from predecessors as
    //    "non-copy" defs, since at this point it's known whether or not they
    //    can be treated as constant. Once those are taken into account, copies
    //    within the same strong component can be ignored, and we make the final
    //    determination of whether these autos are constant, and if so, which
    //    known object is the value (_koi again).
    //
    // 4. Find all loads of constant address-typed autos (including known object
    //    temps this time) and replace their symrefs with the corresponding
    //    const ref symrefs.
    //
    // NOTE: TR::vector indexing is done using the freestanding at() function
    // defined above, which will cause a crash on index out of bounds.

    TR::vector<uint32_t, TR::Region &> srnToAid(stackRegion); // symref num to auto ID
    _srnToAid = &srnToAid;
    srnToAid.resize(comp()->getSymRefTab()->getNumSymRefs(), INVALID_AID);

    TR::vector<AutoInfo, TR::Region &> autoInfos(stackRegion);
    _autoInfos = &autoInfos;

    TR::TreeTop *startTree = comp()->getStartTree();
    bool foundKnownObjectTemp = false;
    for (TR::TreeTopIterator it(startTree, comp()); it != NULL; ++it) {
        TR::Node *node = it.currentTree()->getNode();
        if (node->getOpCodeValue() != TR::astore) {
            continue;
        }

        TR::SymbolReference *symRef = node->getSymbolReference();
        if (!symRef->getSymbol()->isAuto()) {
            continue;
        }

        if (symRef->hasKnownObjectIndex()) {
            // known object temps are trivially constant
            foundKnownObjectTemp = true;
            continue;
        }

        // Found an address-typed auto that occurs in the trees (and that is not
        // trivially constant).
        uint32_t aid = getAid(symRef);
        AutoInfo &autoInfo = at(autoInfos, aid);

        TR::Node *child = node->getChild(0);
        TR::KnownObjectTable::Index childKoi = TR::KnownObjectTable::UNKNOWN;
        if (child->getOpCode().hasSymbolReference()) {
            childKoi = child->getSymbolReference()->getKnownObjectIndex();
        }

        if (childKoi != TR::KnownObjectTable::UNKNOWN) {
            logprintf(trace, log, "aid%u (#%d) def n%un [%p] is constant obj%d\n", aid, symRef->getReferenceNumber(),
                node->getGlobalIndex(), node, childKoi);

            // definition with a constant value
            if (!autoInfo._seenNonCopyDef) {
                autoInfo._koi = childKoi;
                autoInfo._seenNonCopyDef = true;
            } else if (childKoi != autoInfo._koi) {
                if (autoInfo._koi != TR::KnownObjectTable::UNKNOWN) {
                    logprints(trace, log, "  conflicting constant values\n");
                }

                autoInfo._koi = TR::KnownObjectTable::UNKNOWN;
            }
        } else if (child->getOpCodeValue() == TR::aload && child->getSymbol()->isAuto()) {
            // copy from another auto
            TR::SymbolReference *childSymRef = child->getSymbolReference();
            if (childSymRef != symRef) // ignore self-stores
            {
                // This might invalidate autoInfo, so avoid autoInfo here and look
                // it up over again.
                uint32_t childAid = getAid(childSymRef);
                at(autoInfos, aid)._copyFrom.push_back(childAid);
                at(autoInfos, childAid)._copyTo.push_back(aid);
                logprintf(trace, log, "aid%u (#%d) def n%un [%p] is a copy from aid%u (#%d)\n", aid,
                    symRef->getReferenceNumber(), node->getGlobalIndex(), node, childAid,
                    childSymRef->getReferenceNumber());
            }
        } else {
            autoInfo._koi = TR::KnownObjectTable::UNKNOWN;
            autoInfo._seenNonCopyDef = true;
            logprintf(trace, log, "aid%u (#%d) def n%un [%p] value is unknown\n", aid, symRef->getReferenceNumber(),
                node->getGlobalIndex(), node);
        }

        // NOTE: autoInfo may have been invalidated at getAid(childSymRef).
    }

    // Identify strongly connected components in the graph of autos.
    // Kosaraju-Sharir's algorithm. First do a postorder traversal.
    uint32_t numAutoInfos = (uint32_t)autoInfos.size();

    TR::vector<uint32_t, TR::Region &> aidTraversal(stackRegion);
    aidTraversal.reserve(numAutoInfos);

    TR::vector<StackEntry, TR::Region &> aidTraversalStack(stackRegion);
    for (uint32_t startingAid = 0; startingAid < numAutoInfos; startingAid++) {
        AutoInfo &startingInfo = at(autoInfos, startingAid);
        if (startingInfo._visited) {
            continue;
        }

        startingInfo._visited = true;
        StackEntry startingEntry = { startingAid, 0 };
        aidTraversalStack.push_back(startingEntry);
        while (!aidTraversalStack.empty()) {
            StackEntry *top = &aidTraversalStack.back();
            AutoInfo *autoInfo = &at(autoInfos, top->_aid);
            while (top->_edgeIndex < autoInfo->_copyTo.size()) {
                uint32_t succAid = at(autoInfo->_copyTo, top->_edgeIndex++);
                AutoInfo *succInfo = &at(autoInfos, succAid);
                if (!succInfo->_visited) {
                    succInfo->_visited = true;
                    StackEntry succEntry = { succAid, 0 };
                    aidTraversalStack.push_back(succEntry);
                    top = &aidTraversalStack.back();
                    autoInfo = succInfo;
                }
            }

            aidTraversal.push_back(top->_aid);
            aidTraversalStack.pop_back();
        }
    }

    if (trace) {
        log->prints("auto copy graph reverse postorder (AIDs):");
        for (auto it = aidTraversal.rbegin(); it != aidTraversal.rend(); it++) {
            log->printf(" %u", *it);
        }

        log->println();
    }

    // Now go through the nodes in reverse postorder and search backward
    // (_copyFrom instead of _copyTo) to assign strongly connected components.
    TR::vector<IndexRange, TR::Region &> scRanges(stackRegion);
    TR::vector<uint32_t, TR::Region &> scAutos(stackRegion);

    for (auto it = aidTraversal.rbegin(); it != aidTraversal.rend(); it++) {
        uint32_t startingAid = *it;
        AutoInfo &startingInfo = at(autoInfos, startingAid);
        if (startingInfo._strongComponentIndex != INVALID_SCI) {
            continue;
        }

        // These will fit in uint32_t because there are no more strong components
        // than autos and because each auto occurs in exactly one of them.
        TR_ASSERT_FATAL(scRanges.size() < UINT32_MAX, "too many strong components");
        TR_ASSERT_FATAL(scAutos.size() < UINT32_MAX, "too many autos in strong components");
        uint32_t strongComponentIndex = (uint32_t)scRanges.size();
        startingInfo._strongComponentIndex = strongComponentIndex;

        uint32_t scAutosStart = (uint32_t)scAutos.size();
        scRanges.push_back(IndexRange(scAutosStart, scAutosStart));

        StackEntry startingEntry = { startingAid, 0 };
        aidTraversalStack.push_back(startingEntry);
        scAutos.push_back(startingAid);
        while (!aidTraversalStack.empty()) {
            StackEntry *top = &aidTraversalStack.back();
            AutoInfo *autoInfo = &at(autoInfos, top->_aid);
            while (top->_edgeIndex < autoInfo->_copyFrom.size()) {
                uint32_t predAid = at(autoInfo->_copyFrom, top->_edgeIndex++);
                AutoInfo *predInfo = &at(autoInfos, predAid);
                if (predInfo->_strongComponentIndex == INVALID_SCI) {
                    predInfo->_strongComponentIndex = strongComponentIndex;
                    StackEntry predEntry = { predAid, 0 };
                    aidTraversalStack.push_back(predEntry);
                    scAutos.push_back(predAid);
                    top = &aidTraversalStack.back();
                    autoInfo = predInfo;
                }
            }

            aidTraversalStack.pop_back();
        }

        TR_ASSERT_FATAL(scAutos.size() < UINT32_MAX, "too many autos in strong components");
        scRanges.back()._end = (uint32_t)scAutos.size();
    }

    // Reset the visited flag. This will be used below to indicate that we've
    // already determined whether the auto is constant, and if so, which known
    // object it is.
    for (auto it = autoInfos.begin(); it != autoInfos.end(); it++) {
        it->_visited = false;
    }

    // For each strong component, determine the constant value, if any.
    bool foundConstantAuto = false;
    for (auto rangeIt = scRanges.begin(); rangeIt != scRanges.end(); rangeIt++) {
        logprints(trace, log, "auto copy graph strongly connected component:\n");

        uint32_t start = rangeIt->_start;
        uint32_t end = rangeIt->_end;
        TR::KnownObjectTable::Index koi = TR::KnownObjectTable::UNKNOWN;
        bool seenNonCopyDef = false;
        for (uint32_t i = start; i < end; i++) {
            uint32_t aid = at(scAutos, i);
            AutoInfo &autoInfo = at(autoInfos, aid);
            if (trace) {
                log->printf("  aid%u: ", aid);
                if (!autoInfo._seenNonCopyDef) {
                    log->prints("copies only\n");
                } else if (autoInfo._koi == TR::KnownObjectTable::UNKNOWN) {
                    log->prints("unknown\n");
                } else {
                    log->printf("obj%d\n", autoInfo._koi);
                }
            }

            // Take account of any copies from prior strongly connected components.
            // These will now be considered "non-copy" defs, since we only want to
            // ignore copies between autos in the same strong component.
            for (auto it = autoInfo._copyFrom.begin(); it != autoInfo._copyFrom.end(); it++) {
                uint32_t srcAid = *it;
                AutoInfo &srcInfo = at(autoInfos, srcAid);
                if (srcInfo._strongComponentIndex == autoInfo._strongComponentIndex) {
                    continue;
                }

                // We can only copy from earlier strong components.
                TR_ASSERT_FATAL(srcInfo._strongComponentIndex < autoInfo._strongComponentIndex,
                    "backwards copy exists between strong components %u -> %u", srcInfo._strongComponentIndex,
                    autoInfo._strongComponentIndex);

                // It must have already had its constant/unknown value determined.
                TR_ASSERT_FATAL(srcInfo._visited, "copy source %u not yet processed", srcAid);

                if (trace) {
                    log->printf("    copy from aid%u: ", srcAid);
                    if (srcInfo._koi == TR::KnownObjectTable::UNKNOWN) {
                        log->prints("unknown\n");
                    } else {
                        log->printf("obj%d\n", srcInfo._koi);
                    }
                }

                if (!autoInfo._seenNonCopyDef) {
                    autoInfo._koi = srcInfo._koi;
                    autoInfo._seenNonCopyDef = true;
                } else if (srcInfo._koi != autoInfo._koi) {
                    logprintf(trace, log, "      conflicting constant values for aid%u\n", aid);
                    autoInfo._koi = TR::KnownObjectTable::UNKNOWN;
                }
            }

            // Now that this auto has been updated to account for values copied
            // from other strongly connected components, incorporate it into the
            // potentially-constant value of the current component.
            if (!seenNonCopyDef) {
                koi = autoInfo._koi;
                seenNonCopyDef = autoInfo._seenNonCopyDef;
            } else if (autoInfo._seenNonCopyDef && autoInfo._koi != koi) {
                logprints(trace, log, "    conflicting constant values in the strong component\n");
                koi = TR::KnownObjectTable::UNKNOWN;
            }
        }

        TR_ASSERT_FATAL(seenNonCopyDef, "the values are coming from nowhere");

        if (koi != TR::KnownObjectTable::UNKNOWN) {
            foundConstantAuto = true;
            logprintf(trace, log, "  all are const obj%d\n", koi);
        }

        for (uint32_t i = start; i < end; i++) {
            uint32_t aid = at(scAutos, i);
            AutoInfo &autoInfo = at(autoInfos, aid);
            autoInfo._koi = koi;
            autoInfo._visited = true;
        }
    }

    // Change loads of these autos to instead load the corresponding const refs.
    // Don't worry about the stores. If we eliminate all of the loads, global
    // dead store elimination will get them.

    if (!foundConstantAuto && !foundKnownObjectTemp) {
        return 1;
    }

    for (TR::PostorderNodeIterator it(startTree, comp()); it != NULL; ++it) {
        TR::Node *node = it.currentNode();
        if (node->getOpCodeValue() != TR::aload) {
            continue;
        }

        TR::SymbolReference *symRef = node->getSymbolReference();
        if (!symRef->getSymbol()->isAuto()) {
            continue;
        }

        TR::KnownObjectTable::Index koi = symRef->getKnownObjectIndex();
        if (koi == TR::KnownObjectTable::UNKNOWN) {
            uint32_t aid = at(srnToAid, symRef->getReferenceNumber());
            if (aid == INVALID_AID) {
                // This node is an aload of an auto that is not defined anywhere.
                // The value of the load node must be completely irrelevant.
                //
                // The only scenario where this is known to happen safely is with
                // involuntary OSR. Suppose an auto is initially live at at least
                // one OSR point, so that it's listed in the prepareForOSR tree.
                // Later, it's possible that all OSR points where it was live are
                // removed as unreachable. In that case, it might also happen
                // that all of its definitons are also removed as unreachable.
                // The remaining use in prepareForOSR is okay because it will not
                // transition to a point where the variable is live, and so the
                // value is actually irrelevant.
                //
                // Otherwise, this is likely indicative of a bug. However, don't
                // assert, at least for the time being. Even if it reveals a bug,
                // the bug is unlikely to be caused by this pass or even by const
                // refs, so failing an assertion here would unnecessarily make
                // const refs depend on a fix to the existing bug.
                //
                // Just leave this load alone.
                //
                continue;
            }

            AutoInfo &autoInfo = at(autoInfos, aid);
            koi = autoInfo._koi;
            if (koi == TR::KnownObjectTable::UNKNOWN) {
                continue;
            }
        }

        TR::SymbolReference *constSymRef = knot->constSymRef(koi);
        if (!performTransformation(comp(), "%sChange auto load n%un [%p] #%d to load const ref #%d\n",
                optDetailString(), node->getGlobalIndex(), node, symRef->getReferenceNumber(),
                constSymRef->getReferenceNumber())) {
            continue;
        }

        node->setSymbolReference(constSymRef);
    }

    return 1;
}

uint32_t TR::ConstRefRematerialization::getAid(TR::SymbolReference *symRef)
{
    int32_t symRefNum = symRef->getReferenceNumber();
    TR_ASSERT_FATAL(symRef->getSymbol()->isAuto(), "symRef #%d is not an auto", symRefNum);

    uint32_t &aid = at(*_srnToAid, symRefNum);
    if (aid == INVALID_AID) {
        // This fits in uint32_t because symref numbers are int32_t.
        size_t n = _autoInfos->size();
        TR_ASSERT_FATAL(n < UINT32_MAX, "too many autos");
        aid = (uint32_t)n;
        _autoInfos->push_back(AutoInfo(*_memRegion));
        logprintf(trace(), comp()->log(), "assign aid%u to auto #%d\n", aid, symRefNum);
    }

    return aid;
}

const char *TR::ConstRefRematerialization::optDetailString() const throw()
{
    return "O^O CONST REF REMATERIALIZATION: ";
}

TR::ConstRefRematerialization::AutoInfo::AutoInfo(TR::Region &region)
    : _copyFrom(region)
    , _copyTo(region)
    , _koi(TR::KnownObjectTable::UNKNOWN)
    , _strongComponentIndex(INVALID_SCI)
    , _seenNonCopyDef(false)
    , _visited(false)
{}
