###############################################################################
#
# (c) Copyright IBM Corp. 2015, 2016
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

LLVM_CONFIG 	:= llvm-config
CLANG 		:= g++

OMR_CHECKER_HOME := ../OMRChecker/

# This list taken from https://raw.githubusercontent.com/eliben/llvm-clang-samples/master/Makefile
# List of Clang libraries to link. The proper -L will be provided by the
# call to llvm-config
CLANG_LIBS := \
   -Wl,--start-group \
   $(shell $(LLVM_CONFIG) --libs --system-libs) \
   -lclangAST \
   -lclangAnalysis \
   -lclangBasic \
   -lclangDriver \
   -lclangEdit \
   -lclangFrontend \
   -lclangFrontendTool \
   -lclangLex \
   -lclangParse \
   -lclangSema \
   -lclangEdit \
   -lclangASTMatchers \
   -lclangRewrite \
   -lclangRewriteFrontend \
   -lclangStaticAnalyzerFrontend \
   -lclangStaticAnalyzerCheckers \
   -lclangStaticAnalyzerCore \
   -lclangSerialization \
   -lclangToolingCore \
   -lclangTooling \
   -Wl,--end-group


LDFLAGS   	:= $(shell $(LLVM_CONFIG) --ldflags )
CXXFLAGS  	:= $(shell $(LLVM_CONFIG) --cxxflags) -I$(OMR_CHECKER_HOME) -std=c++0x

PROGRAM := OMRRewriter

$(PROGRAM): $(PROGRAM).o 
	$(CLANG) -o $@ $^ $(LDFLAGS) $(CLANG_LIBS) -lpthread

$(PROGRAM).o: $(PROGRAM).cpp 
	$(CLANG) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean: 
	- rm $(PROGRAM).o $(PROGRAM)  
	- rm TestFile.cpp.OMRRewritten TestFile.hpp.OMRRewritten

.PHONY: test
test: $(PROGRAM)
	python test.py --rewriter $(PROGRAM)

