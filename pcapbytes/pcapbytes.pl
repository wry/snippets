#!env perl

# extract bytes from a tcpdump packet capture stream (for code or for further processing)
# use : tcpdump -X -v ... | $0 [0|1]

use strict;
use warnings;

my $argCode = 0;

if ($ARGV[0]) {
  $argCode = $ARGV[0] eq "1";
}

my $buf = "";
my $buflen = 0;
while(<STDIN>) {
  my $tmpbuf;
  if ($_=~/^\d\d:\d\d:\d\d.+length\s(\d+).*$/) {
    $buf = "";
    $buflen = $1;
  }
  elsif ($_=~/^\s+0x\d{4}:(.+)\s\s.+/) {
    $tmpbuf = $1;
    $tmpbuf =~ s/\s//g;
  }

  if ($tmpbuf) {
    $buf .= $tmpbuf;
  }

  if (length($buf) == $buflen*2) {
    print $argCode ? join(",", map{"0x$_"} ($buf=~m/../g))."\n" : "$buf\n";
  }
}

