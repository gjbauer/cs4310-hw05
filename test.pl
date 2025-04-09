#!/usr/bin/perl
use v5.30.0;
use warnings FATAL => 'all';
use autodie qw(:all);

use Test::Simple tests => 5;
use IO::Handle;

ok(!-e "wordcount", "no binaries");
ok(-e "report.md", "report.md file exists");

system("make 2>&1 > /dev/null");

sub data_eq {
    my ($d1, $d2) = @_;

    my @l1 = split("\n", $d1);
    my @l2 = split("\n", $d2);
    my $n1 = scalar @l1;
    my $n2 = scalar @l2;

    if ($n1 != $n2) {
        say "# length mismatch $n1 != $n2";
        return 0;
    }

    for (my $ii = 0; $ii < $n1; ++$ii) {
        my $aa = $l1[$ii];
        my $bb = $l2[$ii];
        chomp $aa; $aa =~ s/\s+/ /g;
        chomp $bb; $bb =~ s/\s+/ /g;
        if ($aa ne $bb) {
            say "# data not equal on line $ii:";
            say "# aa = [$aa]";
            say "# bb = [$bb]";
            return 0;
        }
    }

    return 1;
}

sub testcase {
    my ($path) = @_;
    my $aa = `timeout -k 10 5 ./wordcount "$path"`;
    my $bb = `perl answer.pl "$path"`;
    ok(data_eq($aa, $bb), "output correct for $path");
}


testcase("xmas.txt");
testcase("meditations.txt");


sub check_errors {
    my ($errs) = @_;
    if ($errs ne "") {
        $errs =~ s/^/# /mg;
        warn $errs;
        return 0;
    }
}

sub clang_check {
    my $errs = `clang-check -analyze *.c -- 2> /dev/null`;
    chomp $errs;
    check_errors($errs);
    return $errs eq "";
}

ok(clang_check(), "clang check");

system("make clean 2>&1 > /dev/null");
