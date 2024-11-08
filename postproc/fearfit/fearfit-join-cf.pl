#!/usr/bin/perl
#
use warnings;           # turns on optional warnings
use diagnostics;        # and makes them not terse
use strict;             # makes unsafe constructs illegal
use Getopt::Std;        # for standard option processing

my %opts;
my $verb = 0;
my $comm = 0;
my $VERSION='$Id$'."\n";
my $USAGE="Usage: $0 [options] [control-files]
  where the options are

    -c          include the comments
    -v          increase verbosity

  and the script joins several control files into one, discarding
  directives that fourfit would be ignoring.  It outputs all the
  comments, then all the global declarations, then the if clauses.

  The parsing is rather simplistic at present and assumes rather
  liberal use of linefeeds in the source (i.e. only one type of
  thing on a line) ....
  
  Caveat Emptor: there is guarantee that the result is useful.
";

# some help
if ( $#ARGV < 0 || $ARGV[0] eq "--help" ) { print "$USAGE"; exit(0); }
if ( $ARGV[0] eq "--version" ) { print "$VERSION" . "\n"; exit(0); }

# set default values
$opts{'c'} = 0;
$opts{'v'} = 0;
my @args = @ARGV;

# get and parse args
getopts('cv', \%opts);
$comm = $opts{'c'};
$verb = $opts{'v'};

my $comments = '';      # comments as they appear
my %seencomm = ();
my %keywords = ();      # global keyword statements

my %ifthings = ();      # statements qualified by if ...
my $ifprefix = '';      # current if qualifier
my $ifword = '';        # current if keyword
my %ifclause = ();
my %ifsubcls = ();

# some working variables
my @word = ();
my ($key, $iky, $file);

# header
print "*\n";
print "* merged control file from:\n";
for $key (sort(@ARGV)) {
    print "* $key\n";
}
print "*\n";

# Scan the control files
while (<>) {
    if (defined($file) and $file ne $ARGV) {
        $ifword = '';
        $ifprefix = '';
        if ($verb) { print "** new file \n" ; }
    }

    @word = split;
    if (/^\*/) {
        if ($verb) { print '**comment: ' . $_ ; }
        if (not defined($seencomm{$_})) {
            $comments .= $_;
            $seencomm{$_} = $_;
        }
    } elsif (/^\S*$/) {
        if ($verb) { print '**whitesp: ' . $_ ; }
        # ignore
    } elsif (/^\S*if/) {
        if ($verb) { print '**ifthing: ' . $_ ; }
        $ifprefix = join(' ',@word);
        $ifclause{$ifprefix} = $.;
        $ifword = '';
    } else  {
        if ($verb) { print '**keyword: ' . $_ ; }
        if ($ifprefix eq '') {
            $keywords{$word[0]} = $_;
            next;
        }

        if ($ifword eq '' or ($word[0] =~ m/[a-zA-Z]+/)) {
            $ifword = $word[0];
            if ($verb) { print "** $ifprefix / $ifword \n"; }
            $ifsubcls{$ifprefix . '-' . $ifword} = $.;
            $key = $ifprefix . '-' . $ifword;
            $ifthings{$key} = '';
        }

        $key = $ifprefix . '-' . $ifword;
        if (defined($ifthings{$key})) { $ifthings{$key} .= $_ ; }
        # else { $ifthings{$key} = $ifprefix . "\n" . $_; }

    }

    $file = $ARGV;
}

# dump comments
if ($verb) { print "**printing comments\n"; }
if ($comm) { print $comments; }

# dump keywords
if ($verb) { print "**printing keywords\n"; }
for $key (sort(keys(%keywords))) {
    print $keywords{$key};
}

# dump if clauses
if ($verb) { print "**printing ifthings\n"; }
for $key (sort {$ifclause{$a} <=> $ifclause{$b}} keys(%ifclause)) {
    if ($verb) { print "**printing clause $key from line $ifclause{$key}\n"; }
    print $key . "\n";
    for $iky (sort {$ifsubcls{$a} <=> $ifsubcls{$b}} keys(%ifsubcls)) {
        if ($iky =~ /$key/) { print $ifthings{$iky}; }
    }
}

#
# eof
#
