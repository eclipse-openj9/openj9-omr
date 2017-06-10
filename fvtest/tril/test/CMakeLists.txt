cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(tril_test LANGUAGES C CXX)

set(TRIL_AST_DIR ..)

# Setup testing
enable_testing()
find_package(GTest REQUIRED)
include_directories(
   ${GTEST_INCLUDE_DIR}
   ${TRIL_AST_DIR}
)

add_executable(triltest
   ASTTest.cpp
   ${TRIL_AST_DIR}/ast.c
)

target_link_libraries(triltest 
   ${GTEST_BOTH_LIBRARIES} 
)

add_test(
    NAME triltest
    COMMAND triltest --gtest_color=yes
)

