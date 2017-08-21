/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
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

#include "jitbuilder_compiler.hpp"
#include "Jit.hpp"
#include <cstdio>
#include <string>


int main(int argc, char** argv)
   { 

   FILE* out = stdout; 
   FILE* in = stdin;

   std::string program_name = argv[0];

   bool isDumper = program_name.find("tril_dumper") != std::string::npos;  

   if (2 == argc)
      {
      in = fopen(argv[1], "r"); 
      }

   auto trees = parseFile(in);

   if (trees)
      {
      printTrees(out, trees, 1); 
      if (!isDumper) 
         {
         initializeJit();
         Tril::JitBuilderCompiler compiler{trees}; 
         if (compiler.compile() != 0) { 
            fprintf(out, "Error compiling trees!"); 
         }
         shutdownJit();
         }
      }
   else
      { 
      fprintf(out, "Parse error\n");
      }


   if (2 == argc)
      {
      fclose(in);
      }
   }
