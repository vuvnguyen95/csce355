#!/usr/bin/perl -w

# Produce the given number of random strings over the alphabet of the given
# size.  Output is to stdout.

# optional command line arg sets the initial seed for srand

#sub main
{
    if ($#ARGV != 3) {
	usage();
	die "\n";
    }

    print STDERR "Setting random seed to $ARGV[0]\n";
    srand($ARGV[0]);

    $ns = $ARGV[1];
    print STDERR "Generating $ns strings\n";
    $as = $ARGV[2];
    die "Alphabet size ($as) out of range (1-26) -- quitting\n"
	if $as<1 || $as>26;
    print STDERR "Alphabet size = $as\n";
    $el = $ARGV[3];
    die "Average string length ($el) is negative -- quitting\n"
	if $el < 0.0;
    print STDERR "Average string length = $el\n";

    $prob = $el/(1.0+$el);

    for ($i=0; $i<$ns; $i++) {
	$rand_val = rand();
	while ($rand_val < $prob) {
	    $letter = chr(int($rand_val*$as/$prob) + ord('a'));
	    print $letter;
	    $rand_val = rand();
	}
	print "\n";
    }
}

sub usage {
    print STDERR "Usage: $0 seed ns as el\n";
    print STDERR "where\n";
    print STDERR "    seed is the seed for the pseudorandom number generator (an integer)\n";
    print STDERR "    ns is the number of strings to be generated\n";
    print STDERR "    as is the size of the alphabet (1-26)\n";
    print STDERR "    el is the expected length of a string (real number >= 0)\n";
    print STDERR "        (string length is geometrically distributed)\n";
}
