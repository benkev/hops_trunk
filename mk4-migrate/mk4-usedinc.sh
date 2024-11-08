#!/bin/sh
#
# script to identify if an include is used
#
c_s=`find $HOPS_ROOT/trunk/include -name \*.h -print`
c_s="$c_s "`find $HOPS_ROOT/trunk/postproc -name \*.c -print`
c_s="$c_s "`find $HOPS_ROOT/trunk/sub -name \*.c -print`
c_s="$c_s "`find $HOPS_ROOT/trunk/correlator -name \*.c -print`

verb=false
used=
junk=

for inc
do
    found=`grep -l '^.include.*"'$inc'"' $c_s`
    [ -n "$found" ] && {
	$verb && {
	    echo "#include \"$inc\""
	    echo $found |\
	    sed -e "s+$HOPS_ROOT/trunk/postproc/++g" \
		-e "s+$HOPS_ROOT/trunk/include/++g" \
		-e "s+$HOPS_ROOT/trunk/sub/++g" \
		-e "s+$HOPS_ROOT/trunk/correlator/++g" |\
	    /usr/bin/fold -sw 72 | sed 's/^/    /'
	    echo $inc is used
	    echo ''
	}
	used="$used $inc"
    } || {
	junk="$junk $inc"
    }
done

echo '#' USED includes:
echo $used | /usr/bin/fold -sw 52 | sed -e 's/^/    /' -e 's/$/\\/'

echo '#' JUNK includes:
echo $junk | /usr/bin/fold -sw 52 | sed -e 's/^/    /' -e 's/$/\\/'

#
# eof
#
