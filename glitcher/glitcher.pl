#!/usr/bin/perl -W

# $0 in.jpg out.jpg

use strict;

my $ccnt = 2; # char count
my @alf = map { chr($_); } 48..57,65..90,97..122,43,45,61,47,33;

my @fra = ();
my @toa = ();

sub swapif($$$);
sub getrndc();
sub loadars($);

my $inf = $ARGV[0];
my $outf = $ARGV[1];

if (! defined $inf || ! defined $outf) { die ("\$0 <in> <out>"); }

loadars($ccnt-1);

my $ptr = 0;

open(WRITE, ">:raw", $outf);
open(FILE, "<:raw", $inf);
while((my $cnt = read(FILE, my $b, $ccnt)) != 0)
{
  if($ptr > 0)
  {
    foreach(0..$#fra)
    {
      $b = swapif($b, $fra[$_], $toa[$_]);
    }
  }

  print WRITE "$b";

  $ptr += $cnt;
}
close(FILE);
close(WRITE);

sub swapif($$$)
{
  my ($a,$b,$c) = @_;
  return $a eq $b ? $c : $a;
}

sub getrndc()
{
  return int(rand($#alf+1));
}

sub loadars($)
{
  my $len = shift;
  foreach(0..60)
  {
    my $fr = "";
    my $to = "";
    foreach(0..$len)
    {
      my $fri = getrndc();
      my $toi = getrndc();
      $fr .= sprintf("%s", $alf[$fri]);
      $to .= sprintf("%s", $alf[$toi]);
    }
    push(@fra, $fr);
    push(@toa, $to);
  }
}
