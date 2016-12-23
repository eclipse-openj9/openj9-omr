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
 ******************************************************************************/


#ifndef TEST_METHODINFO_INCL
#define TEST_METHODINFO_INCL

#include "compile/Method.hpp"
#include "infra/Assert.hpp"

namespace TR { class IlInjector; }
namespace TR { class IlType; }

namespace TestCompiler
{

/**
 * Describes a function, such as its signature and IlInjector.
 */
class MethodInfo
   {
   public:
   MethodInfo()
   :
      _file(NULL), _line(NULL), _name(NULL),
      _numArgs(0), _argTypes(NULL), _returnType(NULL)
      {
      }

   /**
    * Define the signature of the method this class is describing.
    */
   void DefineFunction(char *file,
                       char *line,
                       char *name,
                       int32_t numArgs,
                       TR::IlType **argTypes,
                       TR::IlType *returnType)
      {
      _file = file;
      _line = line;
      _name = name;
      _numArgs = numArgs;
      _argTypes = argTypes;
      _returnType = returnType;
      }

   /**
    * Define the IlInjector this method is based on.
    */
   void DefineILInjector(TR::IlInjector *ilInjector)
      {
      _ilInjector = ilInjector;
      }

   /**
    * Return a TR::ResolvedMethod based on the defined function and
    * IlInjector.
    *
    * Before calling this, the function signature and IlInjector must
    * be defined.
    */
   TR::ResolvedMethod ResolvedMethod() const
      {
      TR_ASSERT(
         _file != NULL
         && _line != NULL
         && _name != NULL
         && (_numArgs == 0 || _argTypes != NULL),
         "Cannot create ResolvedMethod without signature");
      TR_ASSERT(_ilInjector != NULL,
                "Cannot create ResolvedMethod without IlInjector");
      return TR::ResolvedMethod(_file, _line, _name, _numArgs, _argTypes, _returnType, 0, _ilInjector);
      }

   private:
   char *_file;
   char *_line;
   char *_name;
   int32_t _numArgs;
   TR::IlType **_argTypes;
   TR::IlType *_returnType;
   TR::IlInjector *_ilInjector;
   };

} // namespace TestCompiler

#endif // !defined(TEST_METHODINFO_INCL)
