#!/bin/bash
#
#

# builddir is not defined in older automakes
[ -n "${builddir}" -a -x ${builddir}/position_dsolv ] || {
    echo no executable; exit 0
}
[ -f ${srcdir}/dsolv.out ] || {
    echo no dsolv.out comparison ; exit 0
}

rm -f dsolv.cmp dsolv2.cmp
${builddir}/position_dsolv  ${srcdir}/hiband.XL.AR.dsolvin  > dsolv.cmp
${builddir}/position_dsolv2 ${srcdir}/hiband.XL.AR.dsolvin2 > dsolv2.cmp

# serious precision problems
tail -8 dsolv.cmp | cut -c 1-19 > dsolv.cmp.cut
tail -8 ${srcdir}/dsolv.out | cut -c 1-19 > dsolv.out.cut

tail -8 dsolv2.cmp | cut -c 1-19 > dsolv2.cmp.cut
tail -8 ${srcdir}/dsolv2.out | cut -c 1-19 > dsolv2.out.cut

# so we just check the most significant bits
cmp dsolv.cmp.cut dsolv.out.cut &&
cmp dsolv2.cmp.cut dsolv2.out.cut

#
# eof
#
