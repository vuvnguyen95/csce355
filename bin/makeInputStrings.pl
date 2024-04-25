#!/usr/bin/perl -w

# Produce the given number of random strings over the alphabet of the given
# size.  Output is to stdout.

# optional command line arg sets the initial seed for srand

#sub main
{
    if ($#ARGV != 3) {
	usage();
	exit(1);
    }

    print STDERR "Setting random seed to $ARGV[0]\n";
    srand($ARGV[0]);

    $ns = $ARGV[1];
    print STDERR "Generating $ns strings\n";
    $as = $ARGV[2];
    print STDERR "Alphabet size = $as\n";
    $el = $ARGV[3];
    die "Average string length ($el) is negative (quitting)\n"
	if $el < 0.0;
    print STDERR "Average string length = $el\n";

print STDERR "Alphabet: $alphabet\n";
$alphaLen = length($alphabet);
print STDERR "Alphabet length = $alphaLen\n";
$numStrs = int(rand(10)) + 1;
print STDERR "Generating $numStrs strings\n";
for ($i=0; $i<$numStrs; $i++) {
    $len = int(rand(1060));    # 1060 is about 200 x ln(200)
    for ($j=0; $j<$len; $j++) {
	$letter = substr $alphabet, int(rand($alphaLen)), 1;
	print $letter;
    }
    print "\n";
}

sub usage {
    print STDERR "Usage: $0 seed ns as el\n";
    print STDERR "where\n";
    print STDERR "    seed is the seed for the pseudorandom number generator (an integer\n";
    print STDERR "    ns is the number of strings to be generated\n";
    print STDERR "    as is the size of the alphabet (1-26)\n";
    print STDERR "    el is the expected length of a string (real number >= 0)\n";
    print STDERR "        (string length is geometrically distributed)\n";
}
