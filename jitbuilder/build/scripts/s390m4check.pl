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

use warnings;
use strict;
use bytes;    # avoid using Unicode internally

die "usage: $0 filename\n" unless @ARGV == 1;
my $inputFile = pop @ARGV;
open FILE, $inputFile or die "unable to open file $inputFile";

my $MAX_LENGTH = 71;
my $is_m4      = ( $inputFile =~ m/\.m4$/ );
my $is_ascii   = 1 if ord('[') == 91;

while (<FILE>)
   {
   chomp;
   my $line   = $_;
   my $length = length($line);
   die "line longer than $MAX_LENGTH characters ($length)"
      if ( $length > $MAX_LENGTH );

   # check that only valid characters show up in the string
   # %goodchars is a hash of characters allowed
   my %goodchars = $is_ascii ?
      map { $_ => 1 } ( 10, 13, 31 .. 127 ) : # ascii
      map { $_ => 1 } ( 13, 21, 64 .. 249 );  # ebcdic
   my $newline = $line;
   $newline =~ s/(.)/$goodchars{ord($1)} ? $1 : ' '/eg;
   die "invalid character(s)"
      if $newline ne $line;

   my $comment_regex =
        $is_m4    ? "ZZ"  # M4
      : $is_ascii ? "#"   # GAS
      : "\\*\\*";         # HLASM

   # skip comment lines
   next if $line =~ m/^$comment_regex/;
   die "comment lines should not have preceding whitespace"
      if $line =~ m/^\s+$comment_regex/;

   # strip the comments out
   $line =~ s/$comment_regex.*$//;

   # the # character is considered a comment everywhere
   $line =~ s/\#.*$//;

   die "comma in first column"    if $line =~ m/^,/;
   die "whitespace next to comma" if $line =~ m/\s,|,\s/;
   die "whitespace within parentheses"
      if $line =~ m/\(.*\s.*\)/;
   die "whitespace before parentheses"
      if $line =~ m/,\s+\(/;
   }
