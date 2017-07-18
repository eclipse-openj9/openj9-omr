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


create_jitbuilder_test(simple       src/Simple.cpp) 
create_jitbuilder_test(conditionals src/Conditionals.cpp) 
create_jitbuilder_test(dotproduct   src/DotProduct.cpp) 

# Mandelbrot takes arguments for its test 
# so will require we enhance create_jitbuilder_target. 
#create_jitbuilder_target(mandelbrot   src/Mandelbrot.cpp) 
