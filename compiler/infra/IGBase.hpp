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
 ******************************************************************************/

#ifndef IGBASE_INCL
#define IGBASE_INCL

#include <stddef.h>          // for NULL
#include <stdint.h>          // for uint32_t
#include "env/TRMemory.hpp"  // for TR_Memory, etc
#include "infra/IGNode.hpp"  // for IGNodeIndex, IGNodeColour

class TR_BitVector;

typedef uint64_t IMIndex;

// Number of pre-computed high indices into the interference matrix.
//
#define NUM_PRECOMPUTED_HIGH_INDICES  64

class TR_IGBase
   {
   private:

   // Number of nodes in the graph.
   //
   IGNodeIndex _numNodes;

   // The _interferenceMatrix is a lower triangular bit matrix that summarizes interferences
   // between nodes.  For a node pair (L,H), the corresponding bit should be set if the two
   // nodes interfere.
   //
   // The low and high node indices begin at 0.
   //
   // The bit matrix is implemented with a bit vector, as follows:
   //
   //                       Low Node
   //                        Index
   //
   //                  0  1  2  3  4  5 ...
   //                +---------------------
   //               0| X                         The bit position for a node pair (L,H)
   //               1| 0  X                      can be computed with:
   //   High Node   2| 1  2  X
   //     Index     3| 3  4  5  X                   bit = L + ((H-1)*H)/2
   //               4| 6  7  8  9  X
   //               5|10 11 12 13 14  X
   //               :|
   //               :|
   //
   // The total number of bits required for N nodes = N * (N-1) / 2
   //
   TR_BitVector * _interferenceMatrix;
   static IMIndex _highIndexTable[NUM_PRECOMPUTED_HIGH_INDICES];

   // For graph colouring, attempt to colour the graph using this many colours.
   //
   IGNodeColour _numColours;

   // Actual number of colours that graph was coloured in.
   //
   IGNodeColour _numberOfColoursUsedToColour;

   protected:

   IMIndex getNodePairToBVIndex(IGNodeIndex index1, IGNodeIndex index2);
   IMIndex getOrderedNodePairToBVIndex(IGNodeIndex low, IGNodeIndex high);

   public:

   TR_ALLOC(TR_Memory::IGBase)

   TR_IGBase() :
      _numNodes(0),
      _numColours(0),
      _numberOfColoursUsedToColour(0),
      _interferenceMatrix(NULL)
       {
       }

   IGNodeIndex getNumNodes()              {return _numNodes;}
   IGNodeIndex setNumNodes(IGNodeIndex n) {return (_numNodes = n);}
   IGNodeIndex incNumNodes()              {return (++_numNodes);}
   bool isEmpty()                         {return (_numNodes == 0);}

   IGNodeColour getNumColours()               {return _numColours;}
   IGNodeColour setNumColours(IGNodeColour c) {return (_numColours = c);}

   TR_BitVector *getInterferenceMatrix()        {return _interferenceMatrix;}
   void setInterferenceMatrix(TR_BitVector *im) {_interferenceMatrix = im;}

   IGNodeColour getNumberOfColoursUsedToColour() {return _numberOfColoursUsedToColour;}
   IGNodeColour setNumberOfColoursUsedToColour(IGNodeColour c) {return (_numberOfColoursUsedToColour = c);}
   };
#endif
