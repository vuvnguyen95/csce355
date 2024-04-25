#!/usr/bin/perl -w

# Output to stdout an n-state worst-case NFA the complete set of test strings.
# Each string starts with a prefix of length n over the alphabet {a,b},
# followed by a suffix of a's (from 0 a's to n-1 many a's).
# All possible strings of this form are generated (n2^n strings in total).

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

    @symbols = ();
    @suffixes = ();
    $suffix = '';
    for ($i=0; $i<$ns; $i++) {
	push @suffixes, $suffix;
	push @symbols, 'a';
	$suffix .= 'a';
    }

    $flag = 1;
    while ($flag) {
	$prefix = join '', @symbols;
	for ($i=0; $i<$ns; $i++) {
	    print($prefix . $suffixes[$i]);
	    print "\n";
	}
	$flag = increment();
    }
}

# Increment the @symbols array, interpreted as a string of binary digits,
# with 'a' representing 0 and 'b' representing 1.
sub increment {
    my $p = $#symbols;
    while ($p>=0 && $symbols[$p] eq 'b') {
	$symbols[$p--] = 'a';
    }
    if ($p >= 0) {
	$symbols[$p] = 'b';
	return 1;
    }
    return 0;
}

sub usage {
    print STDERR "Usage: $0 ns\n";
    print STDERR "where\n";
    print STDERR "    ns is the number of states (1-64)\n";
    print STDERR "WARNING: output size is exponential in the number of states.\n";
}


