#!/bin/sed

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

# Do comments first since they can appear anywhere
s/;/@/g

# Fix up directives
s/^[[:space:]]*DCD[[:space:]]\+\(.*\)$/.long \1/g
s/^[[:space:]]*DCW[[:space:]]\+\(.*\)$/.short \1/g
s/^[[:space:]]*DCB[[:space:]]\+\(.*\)$/.byte \1/g
s/^[[:space:]]*\([[:alnum:]_]\+\)[[:space:]]\+[Ee][Qq][Uu][[:space:]]\+\(.*\)$/ \1 = \2/g
s/^[[:space:]]*AREA.*CODE.*$/.text/g
s/^[[:space:]]*AREA.*DATA.*$/.data/g
s/^[[:space:]]*IMPORT[[:space:]]\+\(.*\)$/.globl \1/g
s/^[[:space:]]*EXPORT[[:space:]]\+\(.*\)$/.globl \1/g
s/^[[:space:]]*ALIGN[[:space:]]\+\(.*\)$/.balign \1/g
s/^[[:space:]]*include[[:space:]]\+\(.*$\)/.include "\1"/g
s/^[[:space:]]*END[[:space:]]*$//g

# Ok, now look for labels
s/^\([[:alpha:]_][[:alnum:]_]*\)\(.*\)/\1:\2/g

# Fix up non-local branches to append "(PLT)"
s/^\([[:space:]]\+[Bb][Ll]\{0,1\}\(eq\|ne\|cs\|cc\|mi\|pl\|vs\|vc\|hi\|ls\|lt\|gt\|le\|ge\|al\)\{0,1\}[[:space:]]\+\)\([[:alpha:]_][[:alnum:]_]*\)\(.*\)$/\1\3(PLT)\4/g
s/^\([[:space:]]\+[Bb][Ll]\{0,1\}\(eq\|ne\|cs\|cc\|mi\|pl\|vs\|vc\|hi\|ls\|lt\|gt\|le\|ge\|al\)\{0,1\}[[:space:]]\+\)\(L_[[:alnum:]_]*\)(PLT)\(.*\)$/\1\3\4/g

