#!/usr/bin/perl -w
use strict;

my $gitlog = "git-log";
open(GITLOG, "< $gitlog") || die("no $gitlog dump");
my $commit = "";
my %commits;

while (<GITLOG>) {
    chomp;
    if (/commit (.*)/) {
        $commit = $1;
    }
    elsif (/structured_networking step (\d*)/) {
        $commits{int($1)} = $commit;
    }
}
close(GITLOG);

open(GITLOG, "git log -1 |") || die("can't get git log");
my $current = 0;
while (<GITLOG>) {
    if (/structured_networking step (\d*)/) {
        $current = $1;
    }
}

$current = int($current);

sub usage() {
    die("usage: $0 (prev|next|goto <num>");
}

sub switch_commit($) {
    my $num = shift;
    printf("switch(%d)\n", $num);
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
if ($ARGV[0] eq "next") {
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
