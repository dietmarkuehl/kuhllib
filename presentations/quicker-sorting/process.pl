#!/usr/bin/perl -w
use strict;

my %sizes;
my %times;
my @names;

while (0 <= $#ARGV) {
  push @names, shift @ARGV;
}
while (<>) {
  chomp;
  if (/name=\s*(\S.*) size=\s*(\S.*) duration=\s*(\d*)us.*/) {
    my($name, $size, $duration) =($1, $2, $3);
    $sizes{$size} = 1;
    $times{join($;, $name, $size)} = $duration;
  }
}

print "Name";
foreach my $name (@names) {
  print ",$name";
}
print "\n";
foreach my $size (sort {$a<=>$b} keys %sizes) {
  next if ($size < 100);
  print "$size";
  # my $scale = $size * log($size);
  my $scale = 1.0 * $times{join($;, "std::sort", $size)};
  foreach my $name (@names) {
    print ",";
    if (exists $times{join($;, $name, $size)}) {
      my $value = 1.0 * $times{join($;, $name, $size)};
      print $value / $scale;
    }
  }
  print "\n";
}
print "\n";
