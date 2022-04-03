#!/usr/bin/perl -w
use strict;

my $gitlog = "git-log";
open(GITLOG, "< $gitlog") || die("no $gitlog dump");
my $commit = "";
my $last   = 0;
my $first  = 1000000000;
my %commits;

while (<GITLOG>) {
    chomp;
    if (/commit (.*)/) {
        $commit = $1;
    }
    elsif (/structured_networking step (\d*)/) {
        my $id = int($1);
        $commits{$id} = $commit;
        if ($last < $id) {
            $last = $id;
        }
        if ($id < $first) {
            $first = $id;
        }
    }
}
close(GITLOG);

open(GITLOG, "git log -1 |") || die("can't get git log");
my $current = 0;
while (<GITLOG>) {
    if (/structured_networking step (\d*)/) {
        $current = $1;
        last;
    }
}
if ($current == 0) {
    $current = $last;
}

$current = int($current);

sub usage() {
    die("usage: $0 (first|last|prev|next|goto <num>");
}

sub switch_commit($) {
    my $num = shift;
    if (exists $commits{$num}) {
        my $command = "git checkout -f $commits{$num}";
        system($command);
    }
    else {
        printf("no such commit $num\n");
    }
}

if ($#ARGV < 0) {
    usage();
}
if ($ARGV[0] eq "first") {
    switch_commit($first);
}
elsif ($ARGV[0] eq "last") {
    switch_commit($last);
}
elsif ($ARGV[0] eq "next") {
    switch_commit($current +1);
}
elsif ($ARGV[0] eq "prev") {
    switch_commit($current -1);
}
elsif ($ARGV[0] eq "goto" && 1 <= $#ARGV) {
    switch_commit(int($ARGV[1]));
}
else {
    usage();
}
