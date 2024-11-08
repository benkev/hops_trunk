#!/bin/bash
#
#

# builddir is not defined in older automakes
[ -n "${builddir}" -a -x ${builddir}/position_dsolv3 ] || {
    echo no executable; exit 0
}
[ -f ${srcdir}/dsolv3.out ] || {
   echo no dsolv3.out comparison ; exit 0
}

rm -f dsolv3.cmp dsolv2.cmp
${builddir}/position_dsolv2 ${srcdir}/hiband.XL.AR.dsolvin2 > dsolv2.cmp
${builddir}/position_dsolv3 ${srcdir}/hiband.XL.AR.dsolvin2 > dsolv3.cmp

cmp ${srcdir}/dsolv3.out dsolv3.cmp

tail -8 dsolv2.cmp | head -7 | cut -c 1-19 > dsolv2.cmp.cut
tail -9 dsolv3.cmp | head -7 | cut -c 1-19 > dsolv3.cmp.cut

# so we just check the most significant bits
cmp dsolv2.cmp.cut dsolv3.cmp.cut

#
# eof
#
