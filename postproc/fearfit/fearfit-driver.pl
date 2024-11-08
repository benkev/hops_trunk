#!/usr/bin/perl
#
use warnings;           # turns on optional warnings
use diagnostics;        # and makes them not terse
use strict;             # makes unsafe constructs illegal
use Getopt::Std;        # for standard option processing

my %opts;
my $VERSION='$Id: fearfit-driver.pl 610 2012-01-31 20:31:53Z gbc $'."\n";
my $USAGE="Usage: $0 [options]
  where

    -n <double>	    minimum factor (us/s)
    -x <double>	    maximum factor (us/s)
    -s <int>	    number of steps+1 (at least 2)

    -b <string>	    baseline (e.g. SC)
    -r <string>	    root timestamp (six-letters)
    -c <string>	    fourfit control file (e.g. cf_3361_low)
    -t <string>     source target (e.g. 3C279)

    -p              preserve existing fringe files
    -v              increase verbosity

  does a binary search to find the value of the phase factor
  (between min/max values specified by -n and -x).  If one of these
  appears extremal, the search will re-center on that edge.
";

# some help
if ( $#ARGV < 0 || $ARGV[0] eq "--help" ) { print "$USAGE"; exit(0); }
if ( $ARGV[0] eq "--version" ) { print "$VERSION" . "\n"; exit(0); }

# set default values
$opts{'b'} = '';
$opts{'c'} = '';
$opts{'n'} = '0';
$opts{'p'} = 0;
$opts{'r'} = '';
$opts{'s'} = '0';
$opts{'t'} = '';
$opts{'v'} = 0;
$opts{'x'} = '0';
my @args = @ARGV;

# get and parse args
getopts('b:c:n:pr:s:t:vx:', \%opts);

my $fmt       = "%.6e";
my $baseline  = $opts{'b'};
my $control   = $opts{'c'};
my $minfactor = sprintf($fmt,$opts{'n'});
my $preserve  = $opts{'p'};
my $root      = $opts{'r'};
my $numsteps  = $opts{'s'};
my $target    = $opts{'t'};
my $verb      = $opts{'v'};
my $maxfactor = sprintf($fmt,$opts{'x'});

# the main command
my $ffcmd  = 'fearfit -b ' . $baseline . ' -c ' . $control;
   $ffcmd .= ' ' . $target . '.' . $root;

$ENV{'HOPS_FEARFIT_BEFORE'} = 0;
$ENV{'HOPS_FEARFIT_AFTER'}  = 0;
$ENV{'HOPS_FEARFIT_FREQLO'} = 'ref_freq';

my $last = 0;
my $minlast = 0;
my $maxlast = 0;
my $step = 1;
my $minsnr = 0;
my $maxsnr = 0;
my $snr = 0;
my $ssnr = 0;
my ($nmul,$xmul) = ( 0.5, 0.5 );
my $line;
my @ans;
my @files;
my $sdir;

#
# Must have correlation data or it's all for naught.
#
die "No $baseline..$root correlation\n" if ( ! -f "$baseline..$root" );
( $sdir = ' # ' . `pwd`) =~ s@/.*/(\d\d\d\d)/@$1/@;

#
# Get starting SNR
#
print "# verbose \n" if ( $verb );
system("fourfit -b $baseline -c $control $target.$root");
@files = glob "$baseline.U.*.$root";
die "Fourfit failed\n" if ( $#files < 0 );
$line = `CorAsc2 208 < $files[$#files] | grep snr`;
@ans = split(' ',$line);
$ssnr = $ans[2];
print "# starting SNR of $ssnr (" . $files[$#files] . ")\n";
print "# cd " . `pwd`;
# shift @args;
print "# fearfit-driver.pl " . join(' ', @args) . "\n";

#
# Work out timing
#
my $times = `CorAsc2 100 < $baseline..$root | grep start`;
my ($year,$doy,$sod,$dur,$hmstart,$hmstop) = scantime($times);
print "###\n";
print "# $baseline..$root $year $doy $sod ($hmstart + $dur = $hmstop)\n";
print "#\n";

#
# Save any fringe files in the current directory
#
if ( $preserve ) {
    print "# Preserving fringe files *.U.*.$root\n";
    mkdir "fearfit-save" if ( ! -d 'fearfit-save' );
    system("mv *.U.*.$root fearfit-save 2>&-");
    @files = glob "*.U.*.$root";
    die "preserve failed" if ( $#files >= 0 );
} else {
    print "# Unlinking fringe files *.U.*.$root\n";
    unlink <*.U.*.$root>;
    @files = glob "*.U.*.$root";
    die "unlink failed" if ( $#files >= 0 );
}

mkdir 'fearfit-work' if ( ! -d 'fearfit-work' );

#
# Binary search loop -- assuming there is ONE maximum between
# $minfactor and $maxfactor this will find it.  If there are
# 2 or more maxima, one will be found.
#
while ($step < $numsteps) {

    if    ($step == 1) { $ENV{'HOPS_FEARFIT_FACTOR'} = $minfactor; }
    elsif ($step == 2) { $ENV{'HOPS_FEARFIT_FACTOR'} = $maxfactor; }
    else  {
	$ENV{'HOPS_FEARFIT_FACTOR'} =
	    sprintf($fmt, $nmul*$minfactor+$xmul*$maxfactor);
    }

    printf("#$step $minfactor(%.2e)", $minsnr);
    printf(" & $maxfactor(%.2e)", $maxsnr);
    print " " . $ENV{'HOPS_FEARFIT_FACTOR'} . " ...";

    print "\n#   " . $ffcmd . " ..." if ( $verb );
    system($ffcmd);
    rename('fearfit.data','fearfit-work/fearfit.' . $step);

    die "fourfit failure\n" if ( ! -f "$baseline.U.$step.$root" );
    $line = `CorAsc2 208 < $baseline.U.$step.$root | grep snr`;
    @ans = split(' ',$line);
    $snr = $ans[2];
    print " $snr\n";

    $last = $step;
    if      ($step == 1) {
	print "#   Step 1\n" if ( $verb );
	$minsnr = $snr; $minlast = $last;
    } elsif ($step == 2) {
	print "#   Step 2\n" if ( $verb );
	$maxsnr = $snr; $maxlast = $last;
    } else {
	if ($minsnr < $snr && $minsnr < $maxsnr) {
	    print "#   Replacing $minfactor\n" if ( $verb );
	    $minsnr = $snr; $minlast = $last;
	    $minfactor = $ENV{'HOPS_FEARFIT_FACTOR'};
	    $nmul = $xmul = 0.5;
	} elsif ($maxsnr < $snr && $maxsnr < $minsnr) {
	    print "#   Replacing $maxfactor\n" if ( $verb );
	    $maxsnr = $snr; $maxlast = $last;
	    $maxfactor = $ENV{'HOPS_FEARFIT_FACTOR'};
	    $nmul = $xmul = 0.5;
	} elsif ($maxsnr == $snr && $minsnr == $snr) {
	    print "#   Plateau\n" if ( $verb );
	    $step = $numsteps;
	} elsif ($minsnr < $maxsnr) {
	    print "#   TryMaxSide\n" if ( $verb );
	    $minsnr = $snr; $minlast = $last;
	    $minfactor = $ENV{'HOPS_FEARFIT_FACTOR'};
	    $nmul = -1.0; $xmul =  2.0;
	} elsif ($minsnr > $maxsnr) {
	    print "#   TryMinSide\n" if ( $verb );
	    $maxsnr = $snr; $maxlast = $last;
	    $maxfactor = $ENV{'HOPS_FEARFIT_FACTOR'};
	    $nmul =  2.0; $xmul = -1.0;
	} else {    # these cases probably do not arise, now
	    if ($minsnr < $maxsnr) {
		print " Bailing with greater extrema\n" if ( $verb );
		$ENV{'HOPS_FEARFIT_FACTOR'} = $maxfactor;
		$snr = $maxsnr;
		$last = $maxlast;
	    } else {
		print "#   Bailing with lesser extrema\n" if ( $verb );
		$ENV{'HOPS_FEARFIT_FACTOR'} = $minfactor;
		$snr = $minsnr;
		$last = $minlast;
	    }
	    $step = $numsteps;
	}
    }

    $step += 1;
}

# final numbers
my $answers = `CorAsc2 208 < $baseline.U.$last.$root | grep = | sed 's/^/# /'`;
print "$answers";

# verbal summary
print "# Final result is in fearfit-work/$baseline.U.$last.$root\n";
print "# using factor " . $ENV{'HOPS_FEARFIT_FACTOR'};
print " for final snr of $snr\n";
# one-liner for plotting
print $sod .' '. $ENV{'HOPS_FEARFIT_FACTOR'} .' '. $ssnr .' '. $snr . $sdir;
print "\n";

#
# Restore saved fringe files to the current directory
#
if ( $preserve ) {
    system("mv fearfit-save/* . 2>&-");
    system("rm -rf fearfit-save");
}
# move the temporary fringes aside
system("mv $baseline.U.*.$root fearfit-work 2>&-");

#
# support function
#
sub sod {   # hhmmss....
    my $hh = substr($_[0],0,2);
    my $mm = substr($_[0],2,2);
    my $ss = substr($_[0],4);
    return($hh * 3600 + $mm * 60 + $ss);
}

#
# Get the scan time
#
sub scantime { # start/stop-line-from-CorAsc2-output
    my $times = $_[0];
    my @sss = split(' ',$times);
    my $scanstart = join(' ',@sss[2..4]);
    my $scanstop  = join(' ',@sss[7..9]);
    my $year = $sss[2];
    my $doy = $sss[3];
    my $sodstart  = sod($sss[4]);
    my $sodstop   = sod($sss[9]);
    my $sod = ($sodstart + $sodstop) / 2;
    my $duration  = $sodstop - $sodstart;
    return($year,$doy,$sod,$duration,$sodstart,$sodstop);
}

#
# eof
#
