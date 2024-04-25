#!/usr/bin/perl -w

# Output to stdout an NFA that accepts inputs that contain a given text string.

#sub main
{
    if ($#ARGV != 1) {
	usage();
	die "\n";
    }
    $as = $ARGV[0];
    die "Alphabet size ($as) out of range (1-26)\n"
	if $as < 1 || $as > 26;
    print STDERR "$as-letter alphabet\n";
    $ss = $ARGV[1];
    die "Search string \"$ss\" too long (must be 0-63 chars)--quitting\n"
	if (length $ss) > 63;
    @chars = split //, $ss;
    $base_ord = ord('a');
    for ($i=0; $i<@chars; $i++) {
	if (ord($chars[$i]) < $base_ord || ord($chars[$i]) >= $base_ord+$as) {
	    die "Search string \"$ss\" not over the given alphabet--quitting\n";
	}
    }
    print STDERR "Search string: \"$ss\"\n";
    $ns = @chars + 1;   # number of states

    print "Number of states: $ns\n";
    print "Alphabet size: $as\n";
    $acc_state = $ns-1;
    print "Accepting states: $acc_state\n";

    if ($ns > 1) {    # If the search string is nonempty

	# Handle transitions out of the start state
	print "{}";         # no e-transitions
	$next_char = ord(shift @chars);
	for ($j=0; $j<$as; $j++) {
	    print " {0";
	    if ($base_ord + $j == $next_char) {
		print ",1";
	    }
	    print "}";
	}
	print "\n";

	# Handle transitions out of intermediate states
	for ($i=1; $i<$acc_state; $i++) {
	    print "{}";
	    $next_char = ord(shift @chars);
	    $next_state = $i+1;
	    for ($j=0; $j<$as; $j++) {
		print " {";
		if ($base_ord + $j == $next_char) {
		    print "$next_state";
		}
		print "}";
	    }
	    print "\n";
	}
    }

    #Handle transitions out of the accepting state
    print "{}";
    for ($j=0; $j<$as; $j++) {
	print " {$acc_state}";
    }
    print "\n";
}

sub usage {
    print STDERR "Usage: $0 as ss\n";
    print STDERR "where\n";
    print STDERR "    as is the size of the alphabet (1-26)\n";
    print STDERR "    ss is the search string (0-63 chars over the alphabet)\n";
}


