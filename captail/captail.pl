#!/usr/bin/env perl
#
#  Copyright (c) 2024 wry
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

# extract bytes from a tcpdump packet capture stream
# use : tcpdump -X -v -tt ... | $0 [0|1|2] [0|1]

use strict;
use warnings;

my $argFmt = $ARGV[0] || 0;
my $argTS = $ARGV[1] || 0;

my $buf = "";
my $buflen = -1;
my $ts = "";
while(<STDIN>) {
  my $tmpbuf;
  if ($_=~/^([\d\.]+)\s.+length\s(\d+).*$/) {
    $buf = "";
    $buflen = $2;
    if ($1 =~ /(\d+)\.(\d{6})$/) {
      $ts = $1 . $2;
    }
  }
  elsif ($_=~/^\s+0x[0-9a-f]{4}:(.+)\s\s.+/) {
    $tmpbuf = $1;
    $tmpbuf =~ s/\s//g;
  }

  if ($tmpbuf) {
    $buf .= $tmpbuf;
  }

  if (length($buf) == $buflen*2) {
    if ($argTS == 1) {
      print "$ts|"
    }
    if ($argFmt == 1) {
      print join(",", map{"0x".$_} ($buf=~m/../g))."\n";
    }
    elsif ($argFmt == 2) {
      print join("", map{my $b=hex($_); $b > 0x1f && $b < 0x7f ? chr($b) : '.'} ($buf=~m/../g))."\n";
    }
    else {
      print "$buf\n";
    }
  }
}
