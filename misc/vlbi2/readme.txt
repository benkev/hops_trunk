Date: Thu, 03 Mar 2011 11:36:09 -0500
From: Dan Smythe <dsmythe@haystack.mit.edu>
To: Geoff Crew <gbc@haystack.mit.edu>
Cc: Jason SooHoo <jsoohoo@haystack.mit.edu>, arogers@haystack.mit.edu,
        Shep Doeleman <dole@haystack.mit.edu>, vfish@haystack.mit.edu,
        Chris Beaudoin <cbeaudoin@haystack.mit.edu>
Subject: vlbi2 - which version

I have discovered that there are 2 or 3 versions of the vlbi2 software
correlator floating around.  See list below.

But the one we were using yesterday,
in ~oper/bin,
should be OK.
I don't know why it was so slow.
Maybe it needs to be re-compiled.
The high-res version C on wfmark5-09 (Mark5-689) seems to be OK.
Try
ssh -X oper@wfmark5-09
and try
cd /data
xcorr sample sample

Version A
On -669 (GB), -691 (GC), -713, -737, :
21910 bytes 2007-01-19 14:48 bin/vlbi2 - the original?,
                                   304KB PostScript file
                                   some debug output
Version B
On Mark5-734 (GD):
23366 bytes 2008-02-07 16:23 bin/vlbi2 - same PostScript file as above,
                                   but no debug output

Version C
On Mark5-689 (WA), -714 (WC), -713 (WD):
19015 bytes 2009-01-17 17:33 bin/vlbi2 - high-resolution version
                                   5.2MB PostScript file
                                   some debug output
Version D
On Mark5-736 (GA):
19693 bytes 2009-08-25 15:47 bin/vlbi2 - high-resolution version
                                   5.2MB PostScript file
                                   some debug output


/usr2/oper/source/vlbi2
/usr2/oper/corr/

wfmark5-16 is some flavor of A or B
wfmark5-09 is some flavor of C or D

original hacking:

  # Mark5-737
  scp -p oper@192.52.61.197:/usr2/data/sample_if?.m5b .

  # times screwed up shift by one second
  make hack
  ./hack 62361149 sample_if1.m5b sample_if2.m5b

The original if1 was svn committed, then removed.

  wfmark5-16/vlbi2-b data/sample_if0.m5b data/sample_if2.m5b -2bit 1
  mv dd1.pos wfmark5-16-if0-if2.ps
  gs wfmark5-16-if0-if2.ps &

  ./wfmark5-09/vlbi2-a data/sample_if0.m5b data/sample_if2.m5b -2bit 1
  mv dd1.pos wfmark5-09-if0-if2.ps
  gs wfmark5-09-if0-if2.ps &

Note that 10000000 bytes only gets you 998.4 frames.

mk5header format is 16+10000 bytes:
 word0   0xabaddeed
 word1   year-2000 (4 bits) (...) frame# (14 bits)
 word2   VLBA BCD Time code JJJSSSSS  (JJJ is lowest 3 digits of 
 word3   (subsecond time code + CRCC)

renamed hack.c to m5b_tshift.c

using wfmark5-16 versions as it doesn't have the CRLF...

