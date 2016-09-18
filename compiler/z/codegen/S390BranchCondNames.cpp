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

#include "codegen/InstOpCode.hpp"  // for InstOpCode::S390NumBranchConditions

const char *BranchConditionToNameMap[TR::InstOpCode::S390NumBranchConditions] =
   {
   "NOPR",
   "JNOP",
   "BRO",
   "JO",
   "BRH",
   "JH",
   "BRP",
   "JP",
   "BRL",
   "JL",
   "BRM",
   "JM",
   "BRNE",
   "JNE",
   "BRNZ",
   "JNZ",
   "BRE",
   "JE",
   "BRZ",
   "JZ",
   "BRNH",
   "JNH",
   "BRNL",
   "JNL",
   "BRNM",
   "JNM",
   "BRNP",
   "JNP",
   "BRNO",
   "JNO",
   "B",
   "BR",
   "BRU",
   "J",
   "BRUL",
   "JLU",
   "BC",
   "BCR",
   "BCRC",
   "BE",
   "BER",
   "BERC",
   "BH",
   "BHR",
   "BHRC",
   "BL",
   "BLR",
   "BLRC",
   "BM",
   "BMR",
   "BMRC",
   "BNANRC",
   "BNCRC",
   "BNE",
   "BNER",
   "BNERC",
   "BNH",
   "BNHR",
   "BNHRC",
   "BNL",
   "BNLR",
   "BNLRC",
   "BNM",
   "BNMR",
   "BNMRC",
   "BNNANRC",
   "BNO",
   "BNOR",
   "BNORC",
   "BNP",
   "BNPR",
   "BNPRC",
   "BNZ",
   "BNZR",
   "BNZRC",
   "BNZORC",
   "BO",
   "BOR",
   "BORC",
   "BP",
   "BPR",
   "BPRC",
   "J",
   "BZ",
   "BZR",
   "BZRC",
   "NOP",
   "VGNOP",
   "MASK0",
   "MASK1",
   "MASK2",
   "MASK3",
   "MASK4",
   "MASK5",
   "MASK6",
   "MASK7",
   "MASK8",
   "MASK9",
   "MASK10",
   "MASK11",
   "MASK12",
   "MASK13",
   "MASK14",
   "MASK15"
   };


const char *BranchConditionToNameMap_ForListing[S390_MAX_BRANCH_MASK] =
{
    "",         // 0x0
    "O",        // 0x1
    "HT",       // 0x2
    "",         // 0x3
    "LT",       // 0x4
    "",         // 0x5
    "NE",       // 0x6
    "",         // 0x7
    "EQ",       // 0x8
    "",         // 0x9
    "HE",       // 0xA - same as NL (not less than)
    "",         // 0xB
    "LE",       // 0xC - same as NH (not higer than)
    "",         // 0xD
    "NO",       // 0xE
    "UNCOND"    // 0xF
};
