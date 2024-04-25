#!/usr/bin/perl -w

# Output to stdout a random NFA with a random alphabet.
# Optional command line arg sets the initial seed for srand

#sub main
{
    if ($#ARGV != 5) {
	usage();
	die "\n";
    }
    print STDERR "Setting random seed to $ARGV[0]\n";
    srand($ARGV[0]);
    $ns = $ARGV[1];
    die "Number of states ($ns) out of range (1-64)\n"
	if $ns < 1 || $ns > 64;
    print STDERR "$ns states\n";
    $as = $ARGV[2];
    die "Alphabet size ($as) out of range (1-26)\n"
	if $as < 1 || $as > 26;
    print STDERR "Alphabet size = $as\n";
    $ap = $ARGV[3];
    die "Accepting state probability ($ap) out of range (0-1)\n"
	if $ap < 0 || $ap > 1;
    print STDERR "Accepting state probability = $ap\n";
    $ep = $ARGV[4];
    die "Epsilon-transition probability ($ep) out of range (0-1)\n"
	if $ep < 0 || $ep > 1;
    print STDERR "Epsilon-transition probability = $ep\n";
    $sp = $ARGV[5];
    die "Non-epsilon-transition probability ($sp) out of range (0-1)\n"
	if $sp < 0 || $sp > 1;
    print STDERR "Non-epsilon-transition probability = $sp\n";

    print "Number of states: $ns\n";
    print "Alphabet size: $as\n";
    print "Accepting states:";
    for ($i=0; $i<$ns; $i++) {
	print " $i" if rand() < $ap;
    }
    print "\n";

    for ($i=0; $i<$ns; $i++) {
	# Handle e-transitions separately first
	$first_flag = 1;
	print "{";
	for ($s=0; $s<$ns; $s++) {
	    if (rand() < $ep) {
		print ","
		    unless $first_flag;
		$first_flag = 0;
		print "$s";
	    }
	}
	print "}";
	# Now handle non-e-transitions
	for ($j=0; $j<$as; $j++) {
	    $first_flag = 1;
	    print " {";
	    for ($s=0; $s<$ns; $s++) {
		if (rand() < $sp) {
		    print ","
			unless $first_flag;
		    $first_flag = 0;
		    print "$s";
		}
	    }
	    print "}"
	}
	print "\n";
    }
}

sub usage {
    print STDERR "Usage: $0 seed ns as ap ep sp\n";
    print STDERR "where\n";
    print STDERR "    seed is seed for the pseudorandom generator (an integer)\n";
    print STDERR "    ns is the number of states (1-64)\n";
    print STDERR "    as is the alphabet size (1-26)\n";
    print STDERR "    ap is the probability that a state is accepting (in [0,1])\n";
    print STDERR "    ep is the probability of a given e-transition (in [0,1])\n";
    print STDERR "    sp is the probability of a given symbol-transition (in [0,1])\n";
}


