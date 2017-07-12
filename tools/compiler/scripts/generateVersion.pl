#!/bin/perl

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

use strict;

# FIXME: incorporate the variant name into the buidl too (prod vs debug)

die("\nUsage:\n  $0 rel_name\n") unless (@ARGV eq 1);

my $rel = $ARGV[0];

my $snapshot_name;
if (defined $ENV{"TR_BUILD_NAME"}) {
    $snapshot_name = $ENV{"TR_BUILD_NAME"};
} else {
    use POSIX;

    # FIXME: try to include a workspace name too
    # Optionally, check if the user has defined $USER_TR_VERSION, and incorporate
    # too.
    my $time = POSIX::strftime("\%Y\%m\%d_\%H\%M", localtime($^T));
    $snapshot_name = $rel . "_" . $time . "_" . $ENV{LOGNAME};
}

print "#include \"control/OMROptions.hpp\"\n\nconst char TR_BUILD_NAME[] = \"$snapshot_name\";\n\n";
