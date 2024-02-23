#!/usr/bin/perl -W

use strict;

sub parsecolu8($);
sub parsecolf($);
sub parsecolh($);
sub colinv($);
sub fmtcol($$);

while (<>) {
  my $c = ($_=~/^#/) ? parsecolh($_) : (($_=~/\./) ? parsecolf($_) : parsecolu8($_));
  if (!$c || $#{$c}<0) { next; }
  my $ic = colinv($c);
  foreach ("f", "b", "h") {
    printf("%s%s", ${_}eq"h"?"#":"", fmtcol($c, $_));
    printf(" / %s%s\n", ${_}eq"h"?"#":"", fmtcol($ic, $_));
  }
}

# parse color with u8 components
sub parsecolu8($) {
  my @ret = map {$_/255.0} $_[0] =~ /(\d+)/g;
  return \@ret;
}

# parse color with f components
sub parsecolf($) {
  my @ret = map {$_} $_[0] =~ /([\d\.]+)/g;
  return \@ret;
}

# parse color with hex components
sub parsecolh($) {
  my @ret = map {hex($_)/255.0} $_[0] =~ /([0123456789abcdef]{2})/gi;
  return \@ret;
}

# get inverse of color
sub colinv($) {
  my @ret = map {1.0-$_} @{$_[0]};
  return \@ret;
}

# format color
sub fmtcol($$) {
  my ($col,$fmt) = @_;
  if ($fmt eq "f") {
    return join(', ', map { sprintf("%.03f",$_) } @{$col});
  }
  elsif ($fmt eq "b") {
    return join(', ', map { sprintf("%d",$_*255) } @{$col});
  }
  elsif ($fmt eq "h") {
    return join('', map { sprintf("%02X",int($_*255)) } @{$col});
  }
}

