#!/bin/bash
#
# average output from several invocations
#
awk '
    $2 ~ /param1/ {p1 += $4; pp1 += ($4)*($4); cp1++;}
    $2 ~ /param2/ {p2 += $4; pp2 += ($4)*($4); cp2++;}
    $2 ~ /param3/ {p3 += $4; pp3 += ($4)*($4); cp3++;}
    $2 ~ /param4/ {p4 += $4; pp4 += ($4)*($4); cp4++;}
    $2 ~ /param5/ {p5 += $4; pp5 += ($4)*($4); cp5++;}
    END {print "Average of",(cp1+cp2+cp3+cp4+cp5)/5,"inputs";
         mp1 = p1/cp1; vp1 = sqrt(pp1/cp1 - mp1*mp1);
         mp2 = p2/cp2; vp2 = sqrt(pp2/cp2 - mp2*mp2);
         mp3 = p3/cp3; vp3 = sqrt(pp3/cp3 - mp3*mp3);
         mp4 = p4/cp4; vp4 = sqrt(pp4/cp4 - mp4*mp4);
         mp5 = p5/cp5; vp5 = sqrt(pp5/cp5 - mp5*mp5);
         printf "mean(param1):   %9f +/- %9f [Dx (m)]\n", mp1, vp1;
         printf "mean(param2):   %9f +/- %9f [Dy (m)]\n", mp2, vp2;
         printf "mean(param3):   %9f +/- %9f [Dz (m)]\n", mp3, vp3;
         printf "mean(param4):   %9f +/- %9f [cko (us)]\n", mp4,vp4;
         printf "mean(param5):   %9f +/- %9f [ckr (ps/s)]\n",mp5,vp5;
    }'

#
# eof
#
