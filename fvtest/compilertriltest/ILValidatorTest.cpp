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

#include "JitTest.hpp"
#include "jitbuilder_compiler.hpp"

#include <string>

class IllformedTrees : public TRTest::JitTest, public ::testing::WithParamInterface<std::string> {};

TEST_P(IllformedTrees, FailCompilation) {
    auto inputTrees = GetParam();
    auto trees = parseString(inputTrees.c_str());

    ASSERT_NOTNULL(trees);

    Tril::JitBuilderCompiler compiler{trees};

    ASSERT_DEATH(compiler.compile(), "VALIDATION ERROR")
            << "Compilation did not fail due to ill-formed input trees";
}

INSTANTIATE_TEST_CASE_P(ILValidatorDeathTest, IllformedTrees, ::testing::Values(
    "(method return=Int32 (block (ireturn (iadd (iconst 1) (sconst 3)))))",
    "(method return=Int32 (block (ireturn (sadd (iconst 1) (iconst 3)))))",
    "(method return=Address (block (areturn (aiadd (aconst 4) (lconst 1)))))",
    "(method return=Address (block (areturn (aladd (aconst 4) (iconst 1)))))",
    "(method return=Address (block (areturn (aiadd (iconst 1) (aconst 4)))))",
    "(method return=Address (block (areturn (aladd (lconst 1) (aconst 4)))))",
    "(method return=Int32 (block (ireturn (acmpeq (iconst 4) (aconst 4)))))",
    "(method return=Int32 (block (ireturn (acmpge (lconst 4) (aconst 4)))))",
    "(method return=NoType (block (return (GlRegDeps))))",
    "(method return=Int32 (block (ireturn (GlRegDeps) (iconst 3))))",
    "(method return=Int32 (block (ireturn (GlRegDeps) (iadd (iconst 1) (iconst 3)))))",
    "(method return=Int32 (block (ireturn (iconst 3 (GlRegDeps)))))",
    "(method return=Int32 (block (ireturn (iadd (GlRegDeps) (iconst 1) (iconst 3)))))"
    ));

class WellformedTrees : public TRTest::JitTest, public ::testing::WithParamInterface<std::string> {};

TEST_P(WellformedTrees, CompileOnly) {
    auto inputTrees = GetParam();
    auto trees = parseString(inputTrees.c_str());

    ASSERT_NOTNULL(trees);

    Tril::JitBuilderCompiler compiler{trees};

    ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly";
}

INSTANTIATE_TEST_CASE_P(ILValidatorTest, WellformedTrees, ::testing::Values(
    "(method return=Int32 (block (ireturn (iconst 3))))",
    "(method return=Int32 (block (ireturn (sconst 3))))",
    "(method return=Int32 (block (ireturn (iadd (iconst 1) (iconst 3)))))",
    "(method return=Address (block (areturn (aladd (aconst 4) (lconst 1)))))",
    "(method return=Int32 (block (ireturn (acmpge (aconst 4) (aconst 4)))))",
    "(method return=Int32 (block (ireturn (scmpeq (sconst 1) (sconst 3)))))",
    "(method return=Int32 (block (ireturn (lcmpeq (lconst 1) (lconst 3)))))"
    ));

class CommoningTest : public TRTest::JitTest, public ::testing::WithParamInterface<std::tuple<int32_t, int32_t>> {};

TEST_P(CommoningTest, CommoningUnderSameTree)
   {
   auto param = GetParam();
   auto tril = TRIL((method return=Int32 args=[Int32, Int32]
                       (block
                           (ireturn
                               (icmpeq
                                  (imul
                                     (iadd (iload parm=0 id="loadParm0") (iload parm=1 id="loadParm1"))
                                     (isub (@common id="loadParm0") (@common id="loadParm1")))
                                  (isub
                                     (imul (@common id="loadParm0") (@common id="loadParm0"))
                                     (imul (@common id="loadParm1") (@common id="loadParm1")))
                                  )))));

   auto ast = parseString(tril);
   ASSERT_NOTNULL(ast) << "Parsing failed unexpectedly";

   Tril::JitBuilderCompiler compiler{ast};
   ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly";

   auto entry_point = compiler.getEntryPoint<int32_t (*)(int32_t, int32_t)>();
   ASSERT_NOTNULL(entry_point);

   ASSERT_EQ(1, entry_point(std::get<0>(param), std::get<1>(param)));
   }

TEST_P(CommoningTest, CommoningWithinBlock)
   {
   auto param = GetParam();
   auto tril = TRIL((method return=Int32 args=[Int32, Int32]
                       (block
                           (iload parm=0 id="loadParm0")
                           (iload parm=1 id="loadParm1")
                           (ireturn
                               (icmpeq
                                  (imul
                                     (iadd (@common id="loadParm0") (@common id="loadParm1"))
                                     (isub (@common id="loadParm0") (@common id="loadParm1")))
                                  (isub
                                     (imul (@common id="loadParm0") (@common id="loadParm0"))
                                     (imul (@common id="loadParm1") (@common id="loadParm1")))
                                  )))));

   auto ast = parseString(tril);
   ASSERT_NOTNULL(ast) << "Parsing failed unexpectedly";

   Tril::JitBuilderCompiler compiler{ast};
   ASSERT_EQ(0, compiler.compile()) << "Compilation failed unexpectedly";

   auto entry_point = compiler.getEntryPoint<int32_t (*)(int32_t, int32_t)>();
   ASSERT_NOTNULL(entry_point);

   ASSERT_EQ(1, entry_point(std::get<0>(param), std::get<1>(param)));
   }

INSTANTIATE_TEST_CASE_P(CommingValidationTest, CommoningTest,
  ::testing::ValuesIn(TRTest::const_value_pairs<int32_t, int32_t>()));
