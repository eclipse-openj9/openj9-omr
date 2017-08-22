###############################################################################
#
# (c) Copyright IBM Corp. 2017, 2017
#
#  This program and the accompanying materials are made available
#  under the terms of the Eclipse Public License v1.0 and
#  Apache License v2.0 which accompanies this distribution.
#
#      The Eclipse Public License is available at
#      http://www.eclipse.org/legal/epl-v10.html
#
#      The Apache License v2.0 is available at
#      http://www.opensource.org/licenses/apache2.0.php
#
# Contributors:
#    Multiple authors (IBM Corp.) - initial implementation and documentation
###############################################################################



macro(create_jitbuilder_test target) 
    add_executable(${target} ${ARGN})
    target_link_libraries(${target} 
        jitbuilder
        ${CMAKE_DL_LIBS}) 
    add_test(NAME ${target}_example_as_test COMMAND ${target}) 
endmacro(create_jitbuilder_test) 

# Basic Tests: These should run properly on all platforms. 
create_jitbuilder_test(conditionals    src/Conditionals.cpp) 
create_jitbuilder_test(isSupportedType src/IsSupportedType.cpp)
create_jitbuilder_test(iterfib         src/IterativeFib.cpp)
create_jitbuilder_test(nestedloop      src/NestedLoop.cpp)
create_jitbuilder_test(pow2            src/Pow2.cpp)
create_jitbuilder_test(simple          src/Simple.cpp) 
create_jitbuilder_test(toiltype        src/ToIlType.cpp)
create_jitbuilder_test(worklist        src/Worklist.cpp)

# Additional Tests: These may not run properly on all platforms
# Opt in by setting OMR_JITBUILDER_ADDITIONAL
if(OMR_JITBUILDER_ADDITIONAL) 
    create_jitbuilder_test(call              src/Call.cpp) 
    create_jitbuilder_test(conststring       src/ConstString.cpp) 
    create_jitbuilder_test(dotproduct        src/DotProduct.cpp) 
    create_jitbuilder_test(fieldaddress      src/FieldAddress.cpp) 
    create_jitbuilder_test(linkedlist        src/LinkedList.cpp) 
    create_jitbuilder_test(localarray        src/LocalArray.cpp) 
    create_jitbuilder_test(operandarraytests src/OperandArrayTests.cpp) 
    create_jitbuilder_test(operandstacktests src/OperandStackTests.cpp) 
    create_jitbuilder_test(pointer           src/Pointer.cpp) 
    create_jitbuilder_test(recfib            src/RecursiveFib.cpp) 
    create_jitbuilder_test(structArray       src/StructArray.cpp) 
    create_jitbuilder_test(switch            src/Switch.cpp) 
    create_jitbuilder_test(union             src/Union.cpp) 
endif()

# Experimental Tests: These are still on the experimental side 
# Opt in by setting OMR_JITBUILDER_EXPERIMENTAL
if(OMR_JITBUILDER_EXPERIMENTAL) 
    create_jitbuilder_test(atomicoperations             src/AtomicOperations.cpp) 
    create_jitbuilder_test(transactionaloperations      src/TransactionalOperations.cpp) 
endif()


# Additional Tests: These may not run properly on all platforms
# Mandelbrot takes arguments for its test 
# so will require we enhance create_jitbuilder_target. 
#create_jitbuilder_target(mandelbrot   src/Mandelbrot.cpp) 
