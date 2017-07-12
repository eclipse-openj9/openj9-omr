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

#include <assert.h>
#include <stdio.h>

typedef int32_t (IncOrDecFunction)(int32_t*);

int main(int argc, char const * const * const argv) {
    assert(argc == 2);
    assert(initializeJit());

    // parse the input Tril file
    FILE* inputFile = fopen(argv[1], "r");
    assert(inputFile != nullptr);
    ASTNode* trees = parseFile(inputFile);
    fclose(inputFile);

    printf("parsed trees:\n");
    printTrees(stdout, trees, 0);

    // assume that the file contians a single method and compile it
    Tril::JitBuilderCompiler incordecCompiler{trees};
    assert(incordecCompiler.compile() == 0);
    auto incordec = incordecCompiler.getEntryPoint<IncOrDecFunction*>();

    int32_t value = 1;
    printf("%d -> %d\n", value, incordec(&value));
    value = 2;
    printf("%d -> %d\n", value, incordec(&value));
    value = -1;
    printf("%d -> %d\n", value, incordec(&value));
    value = -2;
    printf("%d -> %d\n", value, incordec(&value));

    shutdownJit();
    return 0;
}
