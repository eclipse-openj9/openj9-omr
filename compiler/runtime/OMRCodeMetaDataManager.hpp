/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef OMR_CODE_METADATA_MANAGER_INCL
#define OMR_CODE_METADATA_MANAGER_INCL

/*
 * The following #define(s) and typedef(s) must appear before any #includes in this file
 */
#ifndef OMR_CODE_METADATA_MANAGER_CONNECTOR
#define OMR_CODE_METADATA_MANAGER_CONNECTOR
namespace OMR { class CodeMetaDataManager; }
namespace OMR { typedef OMR::CodeMetaDataManager CodeMetaDataManagerConnector; }
#endif

#ifndef OMR_METADATA_HASHTABLE_CONNECTOR
#define OMR_METADATA_HASHTABLE_CONNECTOR
namespace OMR { class MetaDataHashTable; }
namespace OMR { typedef OMR::MetaDataHashTable MetaDataHashTableConnector; }
#endif

#include <stdint.h>               // for uintptr_t, intptr_t
#include "env/TRMemory.hpp"       // for TR_Memory, etc
#include "infra/Annotations.hpp"  // for OMR_EXTENSIBLE
#include "j9nongenerated.h"       // for J9AVLTree (ptr only), etc

namespace TR { class CodeCache; }
namespace TR { class CodeMetaDataManager; }
namespace TR { class MetaDataHashTable; }
namespace TR { struct MethodMetaDataPOD; }

namespace OMR
{
class OMR_EXTENSIBLE CodeMetaDataManager
   {

   public:

   TR_PERSISTENT_ALLOC(TR_Memory::CodeMetaData);

   inline TR::CodeMetaDataManager *self();

   static TR::CodeMetaDataManager *codeMetaDataManager() { return _codeMetaDataManager; }

   bool initializeCodeMetaDataManager();

   /**
   @brief For a given method's MethodMetaDataPOD, finds the appropriate
   hashtable in the AVL tree and inserts the exception table.

   Note, insertArtifact does not check to verify that an artifact's given range
   is not already occupied by an existing artifact.  This is because artifact
   ranges represent the virtual memory actually occupied by compiled code.
   Thus, if two artifact ranges overlap, then two pieces of compiled code
   co-exist on top of each other, which is inherently incorrect.  If the
   possiblity occurs that two artifacts may legitimately have range values that
   overlap, checks will need to be added.

   @param compiledMethod The J9JITExceptionTable representing the newly compiled
   method to insert into the artifacts.
   @return Returns true if successful, and false otherwise.
   */
   bool insertMetaData(TR::MethodMetaDataPOD *metaData);

   /**
   @brief Determines if the JIT artifacts contain a reference to a particular
   MethodMetaDataPOD.

   Note: Just because the JIT artifacts return an artifact for a particular
   startPC does not mean that it is the same artifact being searched for.
   The method could have been recompiled, and then subsequently unloaded and
   another method compiled to the same location before the recompilation-based
   reclamation could occur.  This will result in the JIT artifacts containing a
   different MethodMetaDataPOD for a given startPC.

   @param artifact The artifact for which to search for.
   @return Returns true if the same artifact is found for the artifact's startPC
   and false otherwise.
   */
   bool containsMetaData(TR::MethodMetaDataPOD *metaData);

   /**
   @brief Remove a given artifact from the JIT artifacts if it is found therein.

   @param compiledMethod The MethodMetaDataPOD representing the method to
   remove from the JIT artifacts.
   @return Returns true if the artifact is found within, and successfully
   removed from, the JIT artifacts and false otherwise.
   */
   bool removeMetaData(TR::MethodMetaDataPOD *metaData);


   /**
   @brief Attempts to find a registered artifact for a given artifact's startPC.

   Note: findArtifactForPC does not locally acquire the JIT artifact monitor.
   Users must first manually acquire the monitor or go through another function
   that does.

   @param pc The PC for which we require the JIT artifact.
   @return If an artifact for a given startPC is successfully found, returns
   that artifact, returns NULL otherwise.
   */
   const TR::MethodMetaDataPOD *findMetaDataForPC(uintptr_t pc);


   TR::MetaDataHashTable *addCodeCache(TR::CodeCache *codeCache);


   protected:

   /**
   @brief Initializes the translation metadata manager's members.

   The non-cache members are pointers to types instantiated externally.  The
   constructor simply copies the pointers into the objects private members.
   */
   CodeMetaDataManager();


   /**
   @brief Inserts an artifact into the JIT artifacts causing it to represent a
   given memory range.

   Note this method expects to be called via another method in the artifact
   manager and thus does not acquire the artifact manager's monitor.

   @param artifact The MethodMetaDataPOD which will represent the given memory
   range.
   @param startPC The beginning of the memory range to represent.
   @param endPC The end of the memory range to represent.
   @return Returns true if the artifact was successfully inserted as
   representing the given range, false otherwise.
   */
   bool insertRange(TR::MethodMetaDataPOD *metaData, uintptr_t startPC, uintptr_t endPC);

   /**
   @brief Removes an artifact from the JIT artifacts causing it to no longer
   represent a given memory range.

   Note this method expects to be called via another method in the artifact
   manager and thus does not acquire the artifact manager's monitor.

   @param artifact The MethodMetaDataPOD to remove from representing the given
   memory range.
   @param startPC The beginning of the memory range the artifact represents.
   @param endPC The end of the memory range the artifact represents.
   @return Returns true if the artifact was successfully removed from
   representing the given range, false otherwise.
   */
   bool removeRange(TR::MethodMetaDataPOD *metaData, uintptr_t startPC, uintptr_t endPC);


   /**
   @brief Determines if the current artifactManager query is using the same
   artifact as the previous query, and if not, searches for and retrieves the
   new artifact's code cache's hash table.

   Note this method expects to be called via another method in the artifact
   manager and thus does not acquire the artifact manager's monitor.

   @param artifact The artifact we are currently inquiring about.
   */
   void updateCache(uintptr_t currentPC);

   TR::MethodMetaDataPOD *findMetaDataInHash(
      TR::MetaDataHashTable *table,
      uintptr_t searchValue);

   uintptr_t insertMetaDataRangeInHash(
      TR::MetaDataHashTable *table,
      TR::MethodMetaDataPOD *dataToInsert,
      uintptr_t startPC,
      uintptr_t endPC);

   TR::MethodMetaDataPOD **insertMetaDataArrayInHash(
      TR::MetaDataHashTable *table,
      TR::MethodMetaDataPOD **array,
      TR::MethodMetaDataPOD *dataToInsert,
      uintptr_t startPC);

   TR::MethodMetaDataPOD **allocateMethodStoreInHash(TR::MetaDataHashTable *table);

   uintptr_t removeMetaDataRangeFromHash(
      TR::MetaDataHashTable *table,
      TR::MethodMetaDataPOD *dataToRemove,
      uintptr_t startPC,
      uintptr_t endPC);

   TR::MethodMetaDataPOD **removeMetaDataArrayFromHash(
      TR::MethodMetaDataPOD **array,
      TR::MethodMetaDataPOD *dataToRemove);

   TR::MetaDataHashTable *allocateCodeMetaDataHash(
      uintptr_t start,
      uintptr_t end);

   J9AVLTree *allocateMetaDataAVL();

   private:

   J9AVLTree *_metaDataAVL;

   mutable uintptr_t _cachedPC;
   mutable TR::MetaDataHashTable *_cachedHashTable;
   mutable TR::MethodMetaDataPOD *_retrievedMetaDataCache;

   // Singleton
   static TR::CodeMetaDataManager *_codeMetaDataManager;

   };


struct OMR_EXTENSIBLE MetaDataHashTable
   {
   J9AVLTreeNode parentAVLTreeNode;
   uintptr_t *buckets;
   uintptr_t start;
   uintptr_t end;
   uintptr_t flags;
   uintptr_t *methodStoreStart;
   uintptr_t *methodStoreEnd;
   uintptr_t *currentAllocate;
   };


extern "C"
{
intptr_t avl_jit_metadata_insertionCompare(J9AVLTree *tree, TR::MetaDataHashTable *insertNode, TR::MetaDataHashTable *walkNode);

intptr_t avl_jit_metadata_searchCompare(J9AVLTree *tree, uintptr_t searchValue, TR::MetaDataHashTable *walkNode);
}


}

#endif
