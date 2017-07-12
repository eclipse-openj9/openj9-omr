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

using MandelbrotFunction = void(int32_t, int32_t, int32_t*);

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
    Tril::JitBuilderCompiler mandelbrotCompiler{trees};
    assert(mandelbrotCompiler.compile() == 0);
    auto mandelbrot = mandelbrotCompiler.getEntryPoint<MandelbrotFunction*>();

    constexpr auto size = 80;                   // number of rows/columns in the output table
    constexpr auto iterations = 1000;           // number of iterations to be performed
    int32_t table[size][size] = {{0}};          // the output table

    mandelbrot(iterations, size, &table[0][0]);

    // iterate over each cell in the table and print a corresponding character
    for (auto y = 0; y < size; ++y) {
        for (auto x = 0; x < size; ++x) {

#if defined(FULL_COLOUR)
            // if the current cell is *outside* the Mandelbrot set, print a '#',
            // other wise print ' ' (blank space)
            auto c = table[y][x] < iterations ? '#' : ' ';

            // map the modulus of the cell's value to a terminal color
            int colors[] = {1, 1, 5, 4, 6, 2, 3, 3, 3, 3};
            auto color = colors[table[y][x] % 10];

            // print the selected character in the calculated color
            // using ANSI escape codes
            printf(" \e[0;3%dm%c\e[0m ", color, c);
#elif defined(SIMPLE_COLOUR)
            // if the current cell is *inside* the Mandelbrot set, print a '#',
            // other wise print ' ' (blank space)
            auto c = table[y][x] >= 1000 ? '#' : ' ';

            // map the cell's x-coordinate to a color
            int colors[] = {0, 1, 3, 2, 6, 4, 5, 5, 5};
            auto color = colors[x / 10];

            // print the selected character in the calculated color
            // using ANSI escape codes
            printf(" \e[0;3%dm%c\e[0m ", color , c);
#else
            // if the current cell is *inside* the Mandelbrot set, print a '#',
            // other wise print ' ' (blank space)
            auto c = table[y][x] >= 1000 ? '#' : ' ';

            // print the selected character
            printf(" %c ", c );
#endif

        }
        printf("\n");
    }

   shutdownJit();
   return 0;
}
