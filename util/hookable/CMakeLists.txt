###############################################################################
#
# (c) Copyright IBM Corp. 2017
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

#TODO there is a bunch of stuff with vpaths, presumably for some extensibility reasons
# need to figure out if its actually needed and implement properly if required
add_library(j9hookstatic STATIC
	hookable.cpp
	ut_j9hook.c
	ut_j9hook.h
)
target_include_directories(j9hookstatic
	PUBLIC
		.
)
add_tracegen(j9hook.tdf)

