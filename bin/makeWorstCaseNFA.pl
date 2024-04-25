#!/usr/bin/perl -w

# Output to stdout an n-state NFA whose minimum equivalent DFA has 2^n states.

#sub main
{
    if ($#ARGV != 0) {
	usage();
	die "\n";
    }
    $ns = $ARGV[0];
    die "Number of states ($ns) out of range (1-64)\n"
	if $ns < 1 || $ns > 64;
    print STDERR "$ns states\n";

    print "Number of states: $ns\n";
    print "Alphabet size: 2\n";
    $as = $ns-1;
    print "Accepting states: $as\n";

    for ($i=0; $i<$ns-1; $i++) {
	print "{}";     # no e-transitions
	# Now handle non-e-transitions
	$j = $i+1;
	print " {$j} ";
	print " "
	    if $j<10;
	print "{0,$j}\n";
    }
    # handle last state
    print "{} {}   {0}\n";
}

sub usage {
    print STDERR "Usage: $0 ns\n";
    print STDERR "where\n";
    print STDERR "    ns is the number of states (1-64)\n";
}


