/*******************************************************************************
*
* (c) Copyright IBM Corp. 2016, 2016
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


/**
 * Description: Calls an extensible class static member function on
 *    the concrete class using scope resolution, which must be done
 *    for static functions. This is the normal way of calling static
 *    member functions in an extensible class.
 */

#define OMR_EXTENSIBLE __attribute__((annotate("OMR_Extensible")))

namespace TR  { class ExtClass; }         // forward declaration required to declared `self()`

namespace OMR
{

class OMR_EXTENSIBLE ExtClass
   {
   public:
   TR::ExtClass * self();         // declaration of down cast function

   static void functionCalled();  // function to be called
   void callingFunction();        // function that will make call
                                  //   using scope resolution
   };

} // namespace OMR

namespace TR { class OMR_EXTENSIBLE ExtClass : public OMR::ExtClass {}; }

TR::ExtClass * OMR::ExtClass::self() { return static_cast<TR::ExtClass *>(this); }

void OMR::ExtClass::functionCalled() {}

void OMR::ExtClass::callingFunction() { TR::ExtClass::functionCalled(); }
