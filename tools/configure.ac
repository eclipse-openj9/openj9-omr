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

# Configures the build environment for build tools.
# For cross-compiled packages, the build tools execute on the build
# system, not the host/target system.

# This configure.ac has been confirmed to work with Autoconf v2.63,
# and to fail with Autoconf v2.59. Autoconf v2.59 does not recognize
# AS_CASE(), AS_ECHO(), AC_MSG_ERROR().
AC_PREREQ(2.63)
AC_INIT([OMR], [1.0])
m4_include([../omrmakefiles/confighelpers.m4])

##
## Tools are never cross compiled. Make the host platform the current build platform.
##

AS_IF([test "$host" != "$build"],
  [AC_MSG_NOTICE([tools will not be cross compiled])
   CC=""
   CXX=""])

AC_ARG_VAR([BUILD_CC], [The C compiler targeting the build platform])

AS_IF([test "x" != "x$BUILD_CC"],
  [CC="$BUILD_CC"])

AC_ARG_VAR([CXX_BUILD], [The C++ compiler targeting the build platform])

AS_IF([test "x" != "x$BUILD_CXX"],
  [CXX="$BUILD_CC"])

host="$build"
host_alias="$build_alias"
cross_compiling="no"
ac_tool_prefix=

AC_CANONICAL_BUILD

# Categorize the Build OS
AC_MSG_CHECKING([OMR_BUILD_OS])
OMRCFG_CATEGORIZE_OS([OMR_BUILD_OS], [build_os])
AC_MSG_RESULT([$OMR_BUILD_OS])

# Categorize the Build architecture
AC_MSG_CHECKING([OMR_BUILD_ARCH])
OMRCFG_CATEGORIZE_ARCH([OMR_BUILD_ARCH], [build_cpu])
AC_MSG_RESULT([$OMR_BUILD_ARCH])

AC_SUBST(CC)
AC_SUBST(CXX)
AC_SUBST(CCLINK)
AC_SUBST(CCLINKSHARED)
AC_SUBST(CCLINKEXE)
AC_SUBST(CXXLINK)
AC_SUBST(CXXLINKSHARED)
AC_SUBST(CXXLINKEXE)
AC_SUBST(exeext)
AC_SUBST(solibext)
AC_SUBST(arlibext)
AC_SUBST(objext)
AC_SUBST(libprefix)

AC_PROG_CC()
AC_PROG_CXX()

CCLINK=${CCLINK:-"\$(CC)"}
CCLINKSHARED=${CCLINKSHARED:-"\$(CCLINK)"}
CCLINKEXE=${CCLINKEXE:-"\$(CCLINK)"}

CXXLINK=${CXXLINK:-"\$(CXX)"}
CXXLINKSHARED=${CXXLINKSHARED:-"\$(CXXLINK)"}
CXXLINKEXE=${CXXLINKEXE:-"\$(CXXLINK)"}

: ${AR=ar}
: ${AS=as}
: ${OBJCOPY=objcopy}

AS_CASE([$OMR_HOST_OS],
	[win],[
		: ${objext=.$OBJEXT}
		: ${exeext=.$EXEEXT}
		: ${arlibext=.lib}
		: ${solibext=.dll}
		: ${libprefix=''}
	],
	[osx],[
		: ${objext=.$OBJEXT}
		: ${exeext=''}
		: ${arlibext=.a}
		: ${solibext=.dylib}
		: ${libprefix=lib}
	],
	[
		: ${objext=.$OBJEXT}
		: ${exeext=''}
		: ${arlibext=.a}
		: ${solibext=.so}
		: ${libprefix=lib}
	]
)

AC_CHECK_SIZEOF([void *])
AC_MSG_CHECKING([OMR_BUILD_DATASIZE])
AS_IF([test "$ac_cv_sizeof_void_p" = 8],
	[OMR_BUILD_DATASIZE=64],
	[OMR_BUILD_DATASIZE=32])
AC_MSG_RESULT([$OMR_BUILD_DATASIZE])

AC_MSG_CHECKING([OMR_BUILD_TOOLCHAIN])
OMRCFG_CATEGORIZE_TOOLCHAIN([OMR_BUILD_TOOLCHAIN])
AC_MSG_RESULT([$OMR_BUILD_TOOLCHAIN])

###
### Variable Substitution
###

AC_SUBST(OMR_BUILD_OS)
AC_SUBST(OMR_BUILD_ARCH)
AC_SUBST(OMR_BUILD_TOOLCHAIN)
AC_SUBST(OMR_BUILD_DATASIZE)
AC_SUBST(OMR_BUILD_CC,[$CC])
AC_SUBST(OMR_BUILD_CXX,[$CXX])

AC_CONFIG_FILES([toolconfigure.mk])
AC_CONFIG_HEADERS([toolconfig.h])

AC_OUTPUT
