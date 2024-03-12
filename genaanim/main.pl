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

# tiny 'shadertoy' like prog for an ansi term

use warnings;
use strict;
use Time::HiRes qw(time);

my($W,$H)=(qx{tput cols},qx{tput lines});
my@D=(0)x($W*$H);
my@P=reverse(" ",".","*","x","#");
my$S=time();

sub glength($$) {
  return sqrt($_[0]*$_[0]+$_[1]*$_[1]);
}

sub gpix($) {
  my($lidx,$idx)=($#P,int($_[0]*$#P));
  return $P[$idx>$lidx?$lidx:$idx<0?0:$idx];
}

# hide cursor
print"\033[?25l";

while(1){
  my $t=time()-$S;

  for(my $y=0;$y<$H;$y++){
    for(my $x=0;$x<$W;$x++){
      my($px,$py)=((-1.0+2.0*$x/$W)*$W/$H,-1.0+2.0*$y/$H);

      $px+=sin($t);

      $D[$y*$W+$x]=gpix(glength($px,$py));
    }
  }
  
  # draw
  print"\033[H".join("",@D);
}
