#anoise -v -d 0.0064 -e 4.0 Aa:Aa.vdif:alma62.5x1 Vv:Vv.vdif:vlbi32.0x2
echo '' >> fakit.out
date >> fakit.out
#anoise -v -d 0.0064 -e 4.0 \
#    Aa:Aa.vdif:alma62.5x1 Vv:Vv.vdif:vlbi32.0x2 \
#    Bb:Bb.vdif:alma62.5x1 Ww:Ww.vdif:vlbi32.0x2 2>&1 |\
#anoise -v -d 0.0064 -e 4.0 \
#    Aa:Aa.vdif:alma62.5x1 Vv:Vv.vdif:vlbi32.0x1 \
#    Bb:Bb.vdif:alma62.5x1 Ww:Ww.vdif:vlbi32.0x1 2>&1 |\
#anoise -v -d 0.064 -e 4.0 \
#    A1:A1.vdif:alma62.5x1 V3:V3.vdif:vlbi32.0x1 \
#    B2:B2.vdif:alma62.5x1 W4:W4.vdif:vlbi32.0x1 2>&1 |\
#anoise -v -d 0.64 -e 4.0 \
#    A5:A5.vdif:alma62.5x1 V7:V7.vdif:vlbi32.0x1 \
#    B6:B6.vdif:alma62.5x1 W8:W8.vdif:vlbi32.0x1 2>&1 |\
#anoise -v -d 0.064 -e 4.0 -n honest:4 -n corr:0.1 \
#    Ax:Ax.vdif:alma62.5x1 Vx:Vx.vdif:vlbi32.0x1 \
#    Bx:Bx.vdif:alma62.5x1 Wx:Wx.vdif:vlbi32.0x1 2>&1 |\
#anoise -v -d $d -e 4.0 -n honest:4 -n corr:0.05 -n tone:$t,$a \
#    A${y}:A${y}.vdif:alma62.5x1 V${y}:V${y}.vdif:vlbi32.0x1 \
#    B${y}:B${y}.vdif:alma62.5x1 W${y}:W${y}.vdif:vlbi32.0x1 2>&1 |\
#   A${y}:A${y}.vdif:alma62.5x1 V${y}:V${y}.vdif:vlbi32.0x1  2>&1 |\
y=y t=13 a=0.1 d=0.064
y=z t=17 a=0.001 d=0.016
y=w t=19 a=0.01 d=0.064
y=v t=11 a=0.0025 d=0.064
y=u t=18 a=0.005 d=0.032
y=t t=14 a=0.004 d=0.032
anoise -v -d $d -e 4.0 -n honest:4 -n corr:0.05 -n tone:$t,$a \
    A${y}:A${y}.vdif:alma62.5x1 V${y}:V${y}.vdif:vlbi32.0x1 \
    B${y}:B${y}.vdif:alma62.5x1 W${y}:W${y}.vdif:vlbi32.0x1 2>&1 |\
tee -a fakit.out
