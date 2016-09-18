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

#ifndef TR_X86_INSTOPCODE_INCL
#define TR_X86_INSTOPCODE_INCL

namespace TR
{

/*
 * TODO: this is temporary so that X86 instruction can be re-shaped first.
 * Once X86 instruction is done, then this needs to be done properly.
 */
class InstOpCode
   {
   public:

   enum Mnemonic
      {
      BAD
      };

   InstOpCode(); // TODO: this needs to be private later
   InstOpCode(Mnemonic m): _mnemonic(m)  {}

   Mnemonic getMnemonic() {return _mnemonic;}
   void setMnemonic(Mnemonic op) {_mnemonic = op;}


   private:

   Mnemonic _mnemonic;

   };
}

#endif
