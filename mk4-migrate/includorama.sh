#!/bin/sh
#
# script to hunt down include usage
#

src=$HOPS_ROOT/trunk
inc=$src/include

hs=`find $src -name \*.h -print`
cs=`find $src -name \*.c -print`

[ -f h-inc.deps ] && mv h-inc.deps h-inc.back
true &&
for h in $hs
do
    n=${h/$src\//}
    echo -n $n : ''
    grep 'include.*"' $h |\
    cut -d\" -f2 |\
    tr -s \\012 ' '
    echo ''
done > h-inc.deps

[ -f c-inc.deps ] && mv c-inc.deps c-inc.back
true &&
for h in $cs
do
    n=${h/$src\//}
    echo -n $n : ''
    grep 'include.*"' $h |\
    cut -d\" -f2 |\
    tr -s \\012 ' '
    echo ''
done > c-inc.deps

for i in $inc/*h
do
    inc=`basename $i`
    his=`grep ' '$inc' ' h-inc.deps | wc -l`
    cis=`grep ' '$inc' ' c-inc.deps | wc -l`
    local=''
    [ "$his" -eq 0 ] && conly=code-only || conly=''
    [ "$cis" -eq 0 ] && honly=head-only || honly=''
    [ "$his" -eq 0 -a "$cis" -eq 0 ] && honly='' conly='' local=unused
    echo $inc : $his $cis $honly $conly $local
done > include.counts

#
# eof
#
