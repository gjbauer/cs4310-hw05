#!/usr/bin/perl
use 5.30.0;
use autodie qw(:all);
use warnings FATAL => 'all';

my %counts = ();

open my $doc, "<", shift;

while (my $line = <$doc>) {
    my @words = split(/[^A-Za-z]+/, $line);
    for my $word (@words) {
        $word = lc($word);
        next unless $word =~ /[a-z]/;
        $counts{$word} ||= 0;
        $counts{$word} += 1;
    }
}

close $doc;

for my $word (sort(keys(%counts))) {
    my $count = $counts{$word};
    say("$count\t$word") if ($count > 1);
}
