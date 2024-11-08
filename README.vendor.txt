#
# Delivery of the vendor branch to DiFX
#

### The Vendor Branch is slated to be removed from DiFX
### when HOPS4 goes public.  difx2mark4 was merged into
### 3.25 and made to build properly in 3.26 with the
### dependency on difxio (and gsl and fftw).

### The process begins with a test merge to a branch off dev using 3.26

The "Release" tarball is intended to be available for use as a vendor
branch to DiFX (or to anyone else, for that matter).  After a normal
nightly, the tarball is delivered to /swc/tarpit and the exact svn
version information can be found with, e.g.

    /swc/tarpit/copy2ftp.sh true
    /swc/tarpit/copy2ftp.sh false

With the false (dryrun) argument (default is true), it copies the
tarball to the ftp area on gemini which you should do at this time.
(This script renames hops-tc-${XXX}swc.tar.gz to the ftp area on
gemini with the canonical name hops-${XXX}-${RRRR}.tar.gz in the
notation below.  The "development" version goes to hops/devhops
which is not publically exposed, but available if you have the link.)

These instructions redirect output into temporary files so that you
can monitor what you do as you do it.  They all need to be removed
when done (see the last step).  And you'll want to edit this file to
document what you did.  The steps create a number of temporary files
in /swc/difx/difx-svn/applications/hops that are uniformly ?????.txt
or ?????.sh.  You should always review these files as they are created
or before executing them so that you will have a clue how to undo bad
things that might happen.

The first step is deciding what tarball is to be distributed and putting
it to the old FTP site (gemini)--some folks pick it up from there, not DiFX.

Since version numbers change, define a hops version number, an svn
revsion number, and note the date of the build.  You might as well
store the values here in a short table for future reference, and you
will set these in the shell you use on the master node as you work.

XXX is the hops release, RRR is the svn version number and BLD is
the timestamp of the the hops readme file:

    XXX=3.4  RRR=334  BLD='Jun 13 02:33'    # 2011
    XXX=3.5  RRR=563  BLD='Dec  7 03:33'    # 2011
    # version 3.6 was not released
    XXX=3.7  RRR=708  BLD='Apr 23 03:33'    # 2012
    XXX=3.8  RRR=805  BLD='May 29 03:33'    # for DiFX 2.2:
    XXX=3.9  RRR=890  BLD='Dec 12 03:33'    # 2013
    XXX=3.10 RRR=1009 BLD='Nov  4 03:33'    # 2014
    XXX=3.11 RRR=1302 BLD='Feb 11 02:51'    # renumbered to 3.12
    XXX=3.12 RRR=1417 BLD='Oct 17 02:52'    # 2016
    XXX=3.13 RRR=1431 BLD='Nov 21 02:59'    # 2016
    XXX=3.14 RRR=1729 BLD='May 10 14:36'    # 2017
    XXX=3.15 RRR=1828 BLD='Jun 30 02:24'    # required config/makefile patch
    XXX=3.15 RRR=1832 BLD='Jun 30 14:11'    # not released
    XXX=3.16 RRR=1897 BLD='Jul 25 02:55'    # however, leaving it 3.15 in
                                            # DiFX as we're just patching
    XXX=3.16 RRR=2002 BLD='Sep 14 11:31'    # this is for DiFX 2.5.1 tag
    XXX=3.17 RRR=2160 BLD='Dec 13 02:56'    # 2017 for EHT 2017 Rev3 production
    XXX=3.18 RRR=2251 BLD='Mar  1 02:57'    # 2018 fixing zzzzzz rollover
    XXX=3.19 RRR=2512 BLD='Sep  5 02:52'    # this is for DiFX 2.5.3 tag
    XXX=3.20 RRR=2790 BLD='Jul 31 02:58'    # 2019 for trunk, later DiFX 2.6.x
    XXX=3.21 RRR=2936 BLD='Mar  3 02:57'    # 2020 for trunk, later DiFX 2.6.2
                 2937                       # manually for configure_ac patch
    XXX=3.22 RRR=3226 BLD='Mar 22 02:57'    # 2021 for trunk, 2.5 for (2.5.4)
    XXX=3.23 RRR=3383 BLD='Oct 14 03:00'    # 2021
    XXX=3.24 RRR=3744 BLD='Oct 27 03:01'    # 2021 for trunk, 2.8
                                            # errors in chops...
    XXX=3.24 RRR=3753 BLD='Oct 31 13:08'    # 2021 dropped 3753 instead
    XXX=3.25 RRR=4036 BLD='Sep 20 02:55'    # not delivered to DiFX
                                            # 2022
                                            # DIFX SVN->GIT transition
1   XXX=3.26 RRR=4058 BLD='Sep 29 12:38'    # built on gebeor as a test
2   XXX=3.26 RRR=4065 BLD='Sep 29 16:50'    # built on gebeor as a test

# working on gebeor in this git:
    GIT=/dat1/DiFX/swc/difx-git/difx

Check ($BLD should match this date):

    ls -l /swc/tarpit/hops-${XXX}-README.txt | grep "$BLD"

For rebuilds we temporarily save trunk builds and roots (step 6 below):

    #NOW=16Oct17 WAS=16Oct1X
    #NOW=16Nov21 WAS=16Nov2X
    #NOW=17May10 WAS=17May1X
    #NOW=17Jun30 WAS=17Jun3X
    #NOW=17Sep14 WAS=17Sep1X
    #NOW=17Dec13 WAS=17Dec1X
    #NOW=18Mar01 WAS=18Mar0X
    #NOW=18Sep05 WAS=18Sep05X
    #NOW=19Jul31 WAS=19Jul31X
    #NOW=20Mar03 WAS=20Mar03X
    #NOW=21Oct14 WAS=21Oct14X
    #NOW=22Oct27 WAS=22Oct2X

==== insert temporary notes below this line ====
# last official delivery:

/swc/tarpit/copy2ftp.sh false
scp -p /swc/tarpit/hops-dv-tc-3.24swc.tar.gz \
    gemini:/var/ftp/pub/hops/devhops/hops-3.24-3744-dev.tar.gz
scp -p /swc/tarpit/hops-tc-3.24swc.tar.gz \
    gemini:/var/ftp/pub/hops/hops-3.24-3744.tar.gz
# 3.25 delivered ad hoc

==== insert temporary notes above this line ====
delete your temporary notes next cycle.

0. If you're chicken/dilligent, you may wish to review what changed:

 a. review the ***difx*** log to see if anyone has changed anything
    use the temporary notes section above.

    cd $GIT
    git checkout dev (if feature or main for hotfix)
    git checkout -b feature/hops-3.26-vendor
    git log -- applications/hops

    Note that git doesn't share information about files modified, so
    you are more-or-less on your own for finding changes.

 b. conduct a review of what is in difx relative to what is in hops

    cd $GIT/applications
    bld=/home/gbc/HOPS/build-3.26-difx
    cp -p $bld/hops-${XXX}.tar.gz .
    tar ztf ./hops-${XXX}.tar.gz | sort > tball.txt
    find ./hops | sort > trunk.txt
    sed -e s+hops-${XXX}+./hops+ -e 's+/$++' ./tball.txt |\
        diff trunk.txt - > tdiff.txt
    less tdiff.txt

    should allow you to review the existing files in DiFX (a) or
    relative to deleted (d) in the new tarball.

1. Make a script to identify and remove files no longer in the HOPS svn
   src branch:

    cd $GIT/applications
    find /swc/hops/trunk -name .svn -prune -o -print |\
        sed s+/swc/hops/trunk+./hops+ > shops.txt
    while read l ; do grep -s $l'$' shops.txt > /dev/null || echo $l ;
        done < trunk.txt | sed 's/^/rm /' > purge.sh
    chmod +x purge.sh

2. If you see something sensible, you will *eventually* want to first purge
   missing files (see step 3), and then untar this tarball onto the SVN dir;
   but first, see what differs:

    cd $GIT/applications
    rm -rf hops-${XXX}
    ln -s ./hops hops-${XXX}
    tar zdf ./hops-${XXX}.tar.gz > tdelt.txt 2>&1
    less tdelt.txt
    grep -v 'Mod time differs' tdelt.txt |\
    grep -E Size.differs\|Warning tdelt.txt | grep -v Makefile.in | less

   and review the differences and see if you agree.  (tar -d finds
   differences between the tape archive and the file system.)

3. Ok, if you're statisfied, you can extract the current version into
   the DiFX tree.  In theory, if you screw up, you can svn revert to
   the HEAD of the (DiFX) trunk to recover.  Pay attention to which
   tar you are using here.   PO is 1.23, THE has 1.26

    cd $GIT/applications
    ./purge.sh > tpurg.txt 2>&1
    cd hops
    tar --strip-components=1 -zxf ../hops-${XXX}.tar.gz > ../textr.txt 2>&1
    ./autogen.sh wipe

   Both ../tpurg.txt and ../textr.txt should be empty (no errors).

   Note that some of the hacking around in chops will install sources
   that are found elsewhere in the tarball....

4. The hops configure.ac mentions extra directories not delivered to DiFX
   and the replacement needs to be used.  (There is no need to distribute
   the full hops configure file.)

    cd $GIT/applications/hops
    mv ./configure_ac configure.ac

   Review configure changes for things that may cause problems:

    git diff -b -w ./configure.ac
   
5. At this point you need to inform GIT of the pending deletions or
   additions.  (Note that we do one huge commit later, when all is
   ready to spare all the developers multiple commit messages.)

   In git it is perhaps better to do incremental commits

    cd $GIT/applications/hops
    git status -s . | grep '^ D' | sed 's/^../git rm/' > ../snuke.sh
    chmod +x ../snuke.sh
    ../snuke.sh
    git status -s .
    git commit -m'pre-HOPS ${XXX} deletions'

    git status -s . | grep '^ M' | sed 's/^../git add/' > ../sfadd.sh
    chmod +x ../sfadd.sh
    ../sfadd.sh
    git status -s .
    git commit -m'pre-HOPS ${XXX} additions'

    fix permissions
    find . -name \*.c -perm 0764 -exec chmod 664 {} \;
    git status -s . | grep '^ M' | sed 's/^../git add/' > ../sperm.sh
    chmod +x ../sperm.sh
    ../sperm.sh
    git status -s .
    git commit -m'pre-HOPS ${XXX} perm.fixes'

    git status -s . | grep '^??' | sed 's/^../git add/' > ../sftoo.sh
    chmod +x ../sftoo.sh
    ../sftoo.sh
    git status -s .
    git commit -m'pre-HOPS ${XXX} new files'

   The status command is to verify that things are sane before commit.

6. Verify that the changes build properly in the DIFX nightly tree.

   It is sufficient to let nightly do it overnight (step a).  Otherwise,
   if you need to know promptly, (as gbc on po/the/eht) execute the
   appropriate crontab rule(s) (step b).  In this section, (a) is a
   useful snippet for cleaning up automake stuff if the routine nightly
   fails or if step (b) requires it. 

   In git on a branch, it is safe to commit your changes now after step (a)
   as you are just going to be testing...and a few nits can be cleaned up.

#   git commit -m'HOPS ${XXX} vendor from Haystack svn rev ${RRR} for test'

 a. Check for auto-garbage.

    cd $GIT/applications/hops
    ls aclocal.m4 autom4te.cache configure depcomp test-driver stamp-h1 \
       py-compile install-sh compile missing mkinstalldirs Makefile \
       libtool.m4 ltoptions.m4 ltsugar.m4 ltversion.m4 lt~obsolete.m4 2>&-
    ls Makefile*.in config.* *config.h.in 2>&-

    # verify that we are catching all the trash
    ./autogen.sh
    git status -s .
    ./autogen.sh wipe
    git status -s .

###
 b. If you do chose to rebuild immediately, be sure to remove the existing
    build directory and in any case use a clean environement (i.e. one
    without any existing HOPS/DiFX environment variables) or you won't
    be getting a good check.

    # in a clean shell
    cd /swc/difx
    ls -latr /swc/difx | tail -15
    # set NOW from most recent build
    NOW=22Oct27 WAS=22Oct2X

    # check:
    ls -ld difx-bld-$NOW difx-root-$NOW setup-difx.bash
    # paste the following in so that if anyone is executing on
    # trunk, they will still have a working setup command.
    mv difx-bld-$NOW difx-bld-$WAS
    mv difx-root-$NOW difx-root-$WAS
    rm setup-difx.bash 
    ln -s difx-root-$WAS/setup-difx.bash .
    rm difx-root-trunk
    ln -s difx-root-$WAS difx-root-trunk
    # if you get error messages -- figure it out NOW!!!

    export SWC_CRON=/swc/admin
    $SWC_CRON/cronswitch.sh difxmaster

    Go off and do something else for about 30min.
###

 c. or do a local rebuild (if we do not have a difx nightly):

    cd $GIT/applications/hops
    ./autogen.sh
    temprefix=`pwd`
    mkdir ../build && cd ../build
    ../hops/configure --prefix $temprefix
    make all install check > ../maic.out 2>&1

... pause to blow up the car and buy a new one ....


7. Review the log file, looking for errors, and run the fourfit tests
   (since the DIFX build doesn't normally include a make check), e.g.

    ls -l /swc/difx/setup-difx.bash
    source /swc/difx/setup-difx.bash
    cd /swc/difx/difx-bld-$NOW/applications/hops/trunk/data 
    make check
    cd /swc/difx/difx-bld-$NOW/applications/hops/trunk/chops
    make check

8. It is possible you added or deleted something erroneously.
   If so, clean up using

    cd /swc/difx/difx-svn/applications/hops
    svn status -q | less

    look for any mistakes...  if so...

	svn rm --force ...
	svn revert ...

    where ... is the relevant path.

9. Review what you are about to commit:

    cd /swc/difx/difx-svn/applications/hops
    svn status -q | less
    svn status -q | grep -v '^[ADM]'

   The -q excludes all the temporary files preceded by a ?.
   The actual mods are the ones preceded by M, A or D.

    svn status | grep '^?'

   Should only show the temporary scripts, links and outputs.

10. Clean up the mess and commit.  The commit message
    should reflect what you untarred and when, should there be
    any questions about exactly what was vended.  Additional
    comments about new functionality are also good.

    cd /swc/difx/difx-svn/applications/hops
    sed -e 's/^[ ]*//' > ctmsg.txt <<....EOF
        Delivering HOPS ${XXX} vendor branch at Haystack svn rev ${RRR}
        from tarball (${BLD})/swc/tarpit/hops-tc-${XXX}swc.tar.gz
....EOF
    cat ctmsg.txt

   Puts the commit message into a file.  You can edit to mention
   mention things from the changelog that should later go onto the wiki.  

    svn commit -F ctmsg.txt
    rm hops-$XXX ?????.txt ?????.sh
    svn update
    svn status  # which should show you are now synchronized.

   If you don't clean it up, nightly will complain every night....

11. Finally, once you're happy with the release, bump the LOCAL hops
    number in /swc/hops/trunk/configure.ac so that the old build stays
    at the DiFX version, and the new build becomes the rolling revision.

12. copy trunk (svn copy or brutal copy) to relevant branches
    define TAG=2....  REL=... (svn revision of trunk commit)

 a. brutal copy (rather than surgical merge)

    cd /swc/difx/difx-svn/master_tags/DiFX-$TAG/applications/hops
    rm -rf *
    cd /swc/difx/difx-svn/applications/hops/trunk
    rsync -a ./ /swc/difx/difx-svn/master_tags/DiFX-$TAG/applications/hops
    cd /swc/difx/difx-svn/master_tags/DiFX-$TAG/applications/hops
    svn status | less

 b. add new things

    cd /swc/difx/difx-svn/master_tags/DiFX-$TAG/applications/hops
    svn status | grep ^?
    svn status | grep ^? | cut -c 8-
    # sensible?  then
    svn status | grep ^? | cut -c 8- | xargs -i@ svn add @

 c. delete problem children

    svn status | grep -v '^[AM]'
    svn status | grep -v '^[AM]' | cut -c 8-
    # sensible? then
    svn status | grep -v '^[AM]' | cut -c 8- | xargs -i@ svn rm @

 d. everybody accounted for?

    svn status | grep -v '^[AMD]'

 e. if so, test (assuming work on THE) and commit

    /swc/scripts/difx-tag-install.sh DiFX-$TAG the
    cd /swc/difx/difx-svn
    svn commit -m"Delivering HOPS $XXX vendor branch to $TAG from r$REL"

 f. Build on the other masters:

    for m in po eht demi gefera
    do ssh $m "cd /swc/difx/difx-svn ; svn update" ; done

    /swc/scripts/difx-tag-install.sh DiFX-$TAG po eht demi gefera

 g. Assuming all goes well, you can use difx-2nd-night.sh to support
    more hosts.  One must remember to do an explicit svn update on
    all the masters (or wait for a nightly cycle, since that step is
    skipped in the master tag builds).

    skip_master=x /swc/scripts/difx-2nd-night.sh DiFX-$TAG

13. Update the DiFX wiki for changelog and release numbers at:
    https://www.atnf.csiro.au/vlbi/dokuwiki/doku.php/difx/changelog

14. Consider updates to other deliverables such as m6support or sites/Haystack.

#
# eof vim: nospell
#
