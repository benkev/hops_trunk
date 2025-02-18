#!/usr/bin/perl
#
use warnings;           # turns on optional warnings
use diagnostics;        # and makes them not terse
use strict;             # makes unsafe constructs illegal
use Getopt::Std;        # for standard option processing

# set default values
my %opts;
$opts{'b'} = '';
$opts{'c'} = '';
$opts{'d'} = 60;
$opts{'o'} = '';
$opts{'s'} = 1;
$opts{'v'} = 0;
$opts{'w'} = '';

my $VERSION='$Id: fearfit-brutal.pl 638 2012-02-09 22:36:24Z gbc $'."\n";
my $USAGE="Usage: $0 [options] root-files

  where

    -b <string>	    baselines (e.g. SC,JC,DC)
    -c <string>	    input control file (see fearfit-join-cf.pl)
    -d <int>        degrees separation on initial sampling
    -o <file>       output control file name
    -s <int>	    number of steps
    -v              increase verbosity
    -w <string>     additional control arguments

  takes a control file such as is generated by fearfit-join-cf.pl
  and the baselines used to build it, and then for every root-file
  supplied (full path: expn/scan/source.stamp) it works out the
  appropriate phase shift between the upper and lower bands to make
  a control file that maximizes the signal to noise for all baselines.
  The reference station in each of the baseline pairs will be adjusted.
  (The same logic should have been used for fearfit-join-cf.pl.)

  The SNR is sampled at intervals as set by the -d option; a binary
  search for some number of steps (-s) is then carried out to find
  the best SNR.

  This information is used to produce a new control file with an
  if clause for this scan with updated pc_phases and some plots, all
  of which are placed in the output directory (derived from the output
  control file name).  The output control file should not exist.
";

# some help
if ( $#ARGV < 0 || $ARGV[0] eq "--help" ) { print "$USAGE"; exit(0); }
if ( $ARGV[0] eq "--version" ) { print "$VERSION" . "\n"; exit(0); }

# get and parse args
getopts('b:c:d:o:s:vw:', \%opts);
my @args = @ARGV;

my @baselines = split(/,/, $opts{'b'});
my $icontrol  = $opts{'c'};
my $degrees   = $opts{'d'};
my @tpath     = split(/\//, $opts{'o'});
my $numsteps  = $opts{'s'};
my $verb      = $opts{'v'};
my $words     = $opts{'w'};

# starting values
$ENV{'HOPS_FEARFIT_PCPHASE_DERATE'} = 0.0;
$ENV{'HOPS_FEARFIT_PCPHASE_DELTA'}  = 0.0;
$ENV{'HOPS_FEARFIT_PCPHASE_SWAP'}   = 0;

# starting checks
die "No baselines\n" if ( $#baselines < 0 );
die "No control file\n" if ( $icontrol eq '' or not -r $icontrol );
@tpath = ( 'output.cf' ) if ( $#tpath < 0 );
unshift (@tpath,'.') if ( $#tpath == 0 );
my $output = join('/', @tpath);
pop(@tpath);
my $outdir = join('/', @tpath);
system("mkdir $outdir") if ( not -d $outdir );
die "No output directory\n" if ( not -d $outdir );
# system("cp $icontrol $output");
open(OUTPUT, ">$output");
OUTPUT->autoflush(1);
open(INPUT, "<$icontrol");
while (<INPUT>) { print OUTPUT; } close(INPUT);
die "No output ctl file\n" if ( not -s $output );
die "-d option must between 0 and 360\n" if ( $degrees < 0 or $degrees > 360 );

# main loop working variables
my ($rpath,$scan,$expn,$root,$target,$stamp,$UTepoch);
my ($bl,$bs,$ref,$rem,$ffcmd,$delta);
my (%snrs,%adjs);
my ($maxdel,$mindel,$maxsnr,$snr,$minsnr,$step,$status);

#
# Loop over root paths
#
for $rpath (@args) {
    @tpath = split(/\//, $rpath);
    $expn = shift(@tpath);
    $scan = shift(@tpath);
    $UTepoch = $scan . '000000';
    $UTepoch =~ s/([0-9]{3}-[0-9]{6}).*/$1/;
    $root = shift(@tpath);
    ($target,$stamp) = split(/\./,$root);
    print "Working expn $expn scan $scan root $target.$stamp\n";    
    for $bl (@baselines) {
        $ffcmd = "fearfit -t -c$icontrol ";
        ($ref,$rem) = split(//,$bl);
        if ( -f "$expn/$scan/$ref$rem..$stamp" ) {
            $ENV{'HOPS_FEARFIT_PCPHASE_SWAP'} = 0;
            $bs = $ref;
            $ffcmd .= "-b$ref$rem $expn/$scan/$target.$stamp";
            print "  (no swap adj $bs) $ffcmd\n";
        } elsif ( -f "$expn/$scan/$rem$ref..$stamp" ) {
            $ENV{'HOPS_FEARFIT_PCPHASE_SWAP'} = 1;
            $bs = $ref;
            $ffcmd .= "-b$rem$ref $expn/$scan/$target.$stamp";
            print "  (swapped adj $bs) $ffcmd\n";
        } else {
            print "  No baseline $bl data, continuing.\n";
            next;
        }
        
        # delta scan, accumulating two largest snr values (max & min)
        $delta = 0.0;
        $maxdel = $mindel = 0.0;
        $maxsnr = $minsnr = 0.0;
        while ($delta < 360.0) {
            # get the adjustment
            $adjs{$delta} = &run_ff($ffcmd, $delta, $words, '', 0);
            print "$delta-adj $adjs{$delta}\n" if ($verb);
            # get the new snr
            $snrs{$delta} = &run_ff($ffcmd, $delta, $words, $adjs{$delta}, 1);
            print "$delta-snr $snrs{$delta}\n" if ($verb);
            $snr = &get_snr($snrs{$delta});
            print "$delta-snr " . $snr . "\n" if ($verb);
            if ($snr > $maxsnr) {
                $mindel = $maxdel;
                $minsnr = $maxsnr;
                $maxdel = $delta;
                $maxsnr = $snr;
            } elsif ($snr > $minsnr) {
                $mindel = $delta;
                $minsnr = $snr;
            }
            print "$mindel ($minsnr) and $maxdel ($maxsnr)\n" if ($verb);
            $delta += $degrees;
        }

        # if crosses 0..360 boost 0 by 360
        $delta = ($maxdel - $mindel);
        if      ($delta < -180) {
            $maxdel += 360.0;
            $snrs{$maxdel} = $maxsnr;
        } elsif ($delta >  180) {
            $mindel -= 360.0;
            $snrs{$mindel} = $minsnr;
        }
        if ($maxdel < $mindel) {
            ($mindel,$maxdel) = &swap($mindel,$maxdel);
            ($minsnr,$maxsnr) = &swap($minsnr,$maxsnr);
        }
        if ($mindel > $maxdel) {
            print "$mindel > $maxdel which is a problem\n";
            next;
        }
        print "$mindel ($minsnr) and $maxdel ($maxsnr)\n" if ($verb);

        # binary search on two largest snrs
        $delta = ($mindel + $maxdel) / 2.0;
        $step = 0;
        while ($step < $numsteps) {
            # get the adjustment
            $adjs{$delta} = &run_ff($ffcmd, $delta, $words, '', 0);
            print "$delta-adj $adjs{$delta}\n" if ($verb);
            # get the new snr
            $snrs{$delta} = &run_ff($ffcmd, $delta, $words, $adjs{$delta}, 1);
            print "$delta-snr $snrs{$delta}\n" if ($verb);
            $snr = &get_snr($snrs{$delta});
            ($mindel,$delta,$maxdel,$minsnr,$snr,$maxsnr,$status) =
                &bsearch($mindel,$delta,$maxdel,$minsnr,$snr,$maxsnr);
            $step += 1;
            last if ($status == 1);
        }

        # final pass
        $adjs{$delta} = &run_ff($ffcmd, $delta, $words, '', 0);
        print "$delta-adj $adjs{$delta}\n" if ($verb);
        $snrs{$delta} = &run_ff($ffcmd, $delta, $words, $adjs{$delta}, 1);
        print "$delta-snr $snrs{$delta}\n" if ($verb);

        # append report and if scan and station adjustments to ctl file
        print OUTPUT "\n* from baseline $bl\n";
        for $delta (sort {$a <=> $b} (keys %snrs)) {
            print OUTPUT "* $delta $bl $snrs{$delta}\n";
        }
        print OUTPUT "if station $bs and scan $UTepoch\n";
        print OUTPUT "  $adjs{$delta}\n";
    }
}

print OUTPUT "*\n* eof\n*\n";
close(OUTPUT);

#
# support routine: run ffcmd for delta with words with adjustment
# if last argument is nonzero we want the snr, otherwise the phases.
#
sub run_ff {
    my ($cmd, $del, $wds, $adj, $x) = @_;
    my ($ok,$set,$ans) = (0, '', 0);
    $ENV{'HOPS_FEARFIT_PCPHASE_DELTA'}=$del;
    if ($wds ne '') { $adj = $wds . ' ' . $adj; }
    if ($adj ne '') { $cmd .= ' set ' . $adj; }
    $cmd =~ tr{\n}{ };
    print "($del) " . $cmd . "\n" if ($verb);

    open(FFRUN, $cmd . ' |');
    while (<FFRUN>) {
        # print 'ffrun: ' . $_ if ($verb);
        if (/applying sampler phase delta/) {
            s/\*.*$//;
            $set = $_;
            $ok = 1;
        } elsif (/snr/) {
            chomp($ans = $_);
        } elsif ($ok < 3) {
            $set .= $_ if (not /^\S*\*/);
            $ok ++ ;
        }
    }
    close(FFRUN);

    return $ans if ($x);
    return $set;
}

#
# swap two floats
#
sub swap {
    my ($a,$b) = @_;
    return($b,$a);
}

#
# get the snr out of '* snr 17.378 amp ...'
#
sub get_snr {
    my @toys = split(' ',$_[0]);
    return($toys[2]);
}

#
# do the binary search
#
sub bsearch {
    my ($nd,$d,$xd,$ns,$s,$xs) = @_;
    my ($nm,$xm,$st) = (0.5,0.5,0);

    if ($ns < $s && $ns < $xs) {
        print "#   Replacing $nd\n" if ($verb);
        $ns = $s; $nd = $d;
    } elsif ($xs < $s && $xs < $ns) {
        print "#   Replacing $xd\n" if ($verb);
        $xs = $s; $xd = $d;
    } elsif ($xs == $s && $ns == $s) {
        print "#   Plateau\n" if ($verb);
        $st = 1;
    } elsif ($ns < $xs) {
        print "#   TryMaxSide\n" if ($verb);
        $ns = $s; $nd = $d;
        $nm = -1.0; $xm = 2.0;
    } elsif ($ns > $xs) {
        print "#   TryMinSide\n" if ($verb);
        $xs = $s; $xd = $d;
        $nm = 2.0; $xm = -1.0;
    } else {
        if ($ns < $xs) {
            print " Bailing with greater extrema\n" if ($verb);
            $s = $xs; $d = $xd;
        } else {
            print "#   Bailing with lesser extrema\n" if ($verb);
            $s = $ns; $d = $nd;
        }
        $st = 1;
    }

    $d = $nm * $nd + $xm * $xd;
    return($nd,$d,$xd,$ns,$s,$xs,$st);
}

#
# eof
#
