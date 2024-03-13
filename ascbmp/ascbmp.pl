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

# convert a line of up to 8 chars to their bitmap representation
# use : $0 <char>
# char - the character for bit == 1

use strict;
use warnings;

my $P=$ARGV[0];

die("0 <char>") unless($P);

while(<STDIN>) {
  chomp;
  my @b = unpack("c*");
  my $n = 0;
  for(my $i=0; $i<@b; $i++) {
    $n |= $b[$i] == ord($P) ? 1<<($#b-$i) : 0;
  }
  printf("0b%08b,\n", $n);
}
