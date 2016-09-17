#!/bin/bash

################################################################################
##
## (c) Copyright IBM Corp. 2000, 2016
##
##  This program and the accompanying materials are made available
##  under the terms of the Eclipse Public License v1.0 and
##  Apache License v2.0 which accompanies this distribution.
##
##      The Eclipse Public License is available at
##      http://www.eclipse.org/legal/epl-v10.html
##
##      The Apache License v2.0 is available at
##      http://www.opensource.org/licenses/apache2.0.php
##
## Contributors:
##    Multiple authors (IBM Corp.) - initial implementation and documentation
################################################################################


if [[ -z ${TRSRC} || ! -d ${TRSRC} ]]; then
    echo "\${TRSRC} must be set for this script to work"
    exit 1
fi

files=`find ${TRSRC} -name \*.h -or -name \*.hpp -or -name \*.c -or -name \*.cpp -or -name \*.inc`

# These files have errors, but until they can be fixed, they are deemed
# to be 'grandfathered'. i.e. they are ignored.
# ultimately this list can be removed and all files will be subject to
# the same kind of checking
declare -a grandfathered=(frontend/WCode/static/ValueProfiler.cpp)

#echo ${files}
errors=0
for f in ${files}; do
    skipit=0

    # check if the file is in the grandfathered list, if so skip it
    for g in ${grandfathered[@]}; do
	if [[ $f == *$g ]]; then   # $f ends with $g
	    skipit=1
	fi
    done

    if [[ ${skipit} -eq 0 ]]; then

	# Do not include frontend specific code in common code
	grep -HEn "#include.*(codert|jitrt)/" $f && errors=$[${errors}+1]

	# Add more checks here
    fi

done

echo "Found ${errors} errors."

if [ "$errors" -eq 0 ]
then
  echo "SUCCESSFUL - trlint.sh"
fi

exit ${errors}
