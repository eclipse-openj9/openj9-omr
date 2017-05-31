message(STATUS "SRC = ${CMAKE_SOURCE_DIR}")
if(EXISTS "${omr_SOURCE_DIR}/omrcfg.h")
	message(FATAL_ERROR "An existing omrcfg.h has been detected in the source tree. This causes unexpected errors when compiling. Please build from a clean source tree.")
endif()


