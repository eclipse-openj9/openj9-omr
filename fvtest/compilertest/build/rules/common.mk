# Add our targets to the global targets
all: jit
clean: jit_cleanobjs jit_cleandeps jit_cleandll
cleanobjs: jit_cleanobjs
cleandeps: jit_cleandeps
cleandll: jit_cleandll

#
# Define our targets. "jit_cleanobjs" "jit_cleandeps" and "jit_cleandll" are double-colon so they can be appended to
# throughout the makefile.
#
.PHONY: jit jit_createdirs jit_cleanobjs jit_cleandeps jit_cleandll
jit:
jit_createdirs::
jit_cleanobjs::
jit_cleandeps::
jit_cleandll::

include $(JIT_MAKE_DIR)/rules/$(TOOLCHAIN)/common.mk
