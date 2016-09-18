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

#ifndef ANALYSER_INCL
#define ANALYSER_INCL

#define NUM_ACTIONS 64

#include <stddef.h>  // for NULL
#include <stdint.h>  // for uint8_t

#define Clob2 0x01
#define Mem2  0x02
#define Reg2  0x04
#define Clob1 0x08
#define Mem1  0x10
#define Reg1  0x20

namespace TR { class Compilation; }
namespace TR { class Node; }
namespace TR { class Register; }

class TR_Analyser
   {
   uint8_t _inputs;

   public:

   TR_Analyser() : _inputs(0) {};

   void setInputs(TR::Node     *firstChild,
                  TR::Register *firstRegister,
                  TR::Node     *secondChild,
                  TR::Register *secondRegister,
                  bool         nonClobberingDestination = false,
                  bool         dontClobberAnything = false,
                  TR::Compilation *comp = NULL,
                  bool         lockedIntoRegister1  = false,
                  bool         lockedIntoRegister2  = false);

   void resetReg1()  {_inputs &= ~Reg1;}
   void resetReg2()  {_inputs &= ~Reg2;}
   void resetMem1()  {_inputs &= ~Mem1;}
   void resetMem2()  {_inputs &= ~Mem2;}
   void resetClob1() {_inputs &= ~Clob1;}
   void resetClob2() {_inputs &= ~Clob2;}
   void setClob1()   {_inputs |=  Clob1;}
   void setClob2()   {_inputs |=  Clob2;}
   void setReg1()    {_inputs |=  Reg1;}
   void setReg2()    {_inputs |=  Reg2;}
   void setMem1()    {_inputs |=  Mem1;}
   void setMem2()    {_inputs |=  Mem2;}
   uint8_t getMem1()    { return _inputs &  Mem1;}
   uint8_t getMem2()    { return _inputs &  Mem2;}


   uint8_t getInputs() {return _inputs;}

   };

#endif
