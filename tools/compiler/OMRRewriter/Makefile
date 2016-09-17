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

