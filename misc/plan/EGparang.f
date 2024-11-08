      program EGparang
c
c  Compute parallactic angles at Westford and GGAO.
c
c  Source name, DOY, and epoch interval, in minutes, are specified
c   in command line.
c
c  Stdout.
c
c  This is the linux version, which differs from hp-ux in how the
c   command line arguments get passed to the program.
c
      character stw,stv
      character source*8,doych*8,intervalch*3
      character myrach*18,mydech*18
c               xx(4)
      real*4 myra,mydc
      common /diddle/ source,myra,mydc
c  Default station names
      stw = 'W'
      stv = 'V'
c
c  Get DOY and source name from command line.
c
      if( iargc() .ge. 2 ) then
         call getarg(1,source)
         call getarg(2,doych)
         call getarg(3,intervalch)
         if( iargc() .eq. 5 ) then
             call getarg(4,myrach)
             read(myrach,*) myra
             call getarg(5,mydech)
             read(mydech,*) mydc
         else
           if( iargc() .ne. 3 ) then
             write(6,'("Use no arguments for help")')
             stop
           endif
         endif
         read(doych,*) idoy
         read(intervalch,*) interval
      else
         write(6,'("Command line: EGparang  ",
     &             "source_name  day-of-year  interval(min<60)",
     &             " [ra dec]",/,
     &             "  The source name must be <= 8 characters.",/,
     &             "  For non-catalog sources, provide hours R.A.",/,
     &             "  and deg of Decl. as 2 extra real arguments.")')
         stop
      endif
c
c  Check whether source name is in catalog.
c
      patest = parang2(idoy,0,0,0,stw,source,3,ierr,haE,azE,elE)
      if( ierr .eq. -2 ) then
         write(6,'("Source ",a8," is not in catalog.  Quitting.")')
     &             source
         write(6,'("Catalog includes: 3C84, 1308+326, OQ208, 4C39.25",/,
     &         "  0059+581, 0104-408, 0528+134, 0552+398, 0727-115",/,
     &         "  1308+326, 1741-038, 1921-293, 2145+067, 3C273B",/,
     &         "  3C279, 2134+004, 3C454.3, 3C144, 3C345")')
         stop
      endif
c
c  Check whether DOY and minute interval are valid.
c
      if( idoy .lt. 1 .or. idoy .gt. 366 ) then
         write(6,'(" DOY",i6," is an invalid entry.  Quitting.")') idoy
         stop
      endif
      if( interval .le. 0 ) then
         write(6,'("Interval",i5," is invalid.  Quitting.")') interval
         stop
      endif
c
c  Write header.
c
      write(6,'("#                    ","<-------Westford------->",
     &   "  ","<----------GGAO-------->"," PAdiff")')
      write(6,'("#ddd hh mm source       HA     az   el     PA ",
     &   "    HA     az   el     PA")')
c
c  Write PAs and PA diff every 'interval' minutes over the portion
c   of the day when the source is above the horizon at both stations.
c
      do ihr = 0,23
         do imin = 0,59,interval
            paE = parang2(idoy,ihr,imin,0,stw,source,3,ierr,haE,azE,elE)
            paG = parang2(idoy,ihr,imin,0,stv,source,3,ierr,haG,azG,elG)
            padiff = paE - paG
            if( elE .ge. 0. .and. elG .ge. 0. )
     &       write(6,'(i4.3,2i3.2,1x,a8,2(f7.2,f7.1,f5.1,f7.1),f7.1)')
     &       idoy,ihr,imin,source,haE,azE,elE,paE,haG,azG,elG,paG,padiff
         enddo
      enddo
c
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      function parang2(idoy,ihr,imin,isec,sta,source,axis,ierr,
     &                 ha,az,el)
c
c  Same as original (2006) parang but with GGAO/MV3 (station V) added.
c
c  Compute parallactic angle for a given epoch, station, and source.
c  ***Assume year is 2010.***
c  Parts cribbed from hazel.f.
c
      character sta,source*8,sta_name(21),source_name(20)*8
      real*4 sta_lat(21),sta_long(21),ra(20),dec(20)
c
      integer*4 axis,sta_axis(21),kaxis(2)
      real*8 azr(2,2),elr(2,2),decr,sitlatr(2),pangld(2)
c
      real*4 myra,mydc
      character foobar*8
      common /diddle/ foobar,myra,mydc
      data kaxis/2*0/,azr/4*0./,elr/4*0./,sitlatr/2*0./
c
c                 geodetic coord
c  station names   Wlong    lat    axis    sessions
c   E  FD-VLBA    103.94   30.64   AZEL   2689 & 2690
c   I  HN-VLBA     71.99   42.93   AZEL   2689 & 2690
c   K  KP-VLBA    111.61   31.96   AZEL   2688 & 2690
c   L  LA-VLBA    106.25   35.78   AZEL   2688 & 2690
c   P  OV-VLBA    118.28   37.23   AZEL   2689 & 2690
c   R  PIETOWN    108.12   34.30   AZEL   2689 & 2690
c   A  GILCREEK   147.50   64.98   XYNS   2688
c   W  WESTFORD    71.49   42.61   AZEL   2688
c   G  ALGOPARK    78.07   45.96   AZEL   2688
c   N  NL-VLBA     91.57   41.77   AZEL   2688
c   X  YLOW7296   114.48   62.48   AZEL   2688
c   B  BR-VLBA    119.68   48.13   AZEL   2689
c   C  SC-VLBA     64.58   17.76   AZEL   2689
c   F  FORTLEZA    38.43   -3.88   AZEL   2689
c   H  HOBART26   212.56  -42.80   XYEW   2689
c   J  HARTRAO    332.31  -25.89   HADC   2689
c   M  DSS65        4.25   40.43   AZEL   2689
c   T  MK-VLBA    155.46   19.80   AZEL   2689
c   O  NYALES20   348.13   78.93   AZEL   2689
c   Q  KASHIMA    219.34   35.95   AZEL   2689
c   V  GGAO        76.49   39.01   AZEL   ----
c
c  Geodetic coordinates (deg) from sked catalog file 'position.cat'.
c
      data sta_name/'E','I','K','L','P','R',
     &              'A','W','G','N','X',
     &              'B','C','F','H','J','M',
     &              'T','O','Q','V'/
      data sta_long/103.94,71.99,111.61,106.25,118.28,108.12,
     &              147.50,71.49,78.07,91.57,114.48,
     &              119.68,64.58,38.43,212.56,332.31,4.25,
     &              155.46,348.13,219.34,76.49/
      data sta_lat/30.64,42.93,31.96,35.78,37.23,34.30,
     &             64.98,42.61,45.96,41.77,62.48,
     &             48.13,17.76,-3.88,-42.80,-25.89,40.43,
     &             19.80,78.93,35.95,39.01/
      data sta_axis/3,3,3,3,3,3,
     &              2,3,3,3,3,
     &              3,3,3,4,1,3,
     &              3,3,3,3/
c
c  Source J2000 coordinates (RA in hours, dec in deg) from sked
c    catalog 'source.cat'.
c
c  0316+413 3C84     03 19 48.16010742   41  30  42.1026825
c  1308+326 $        13 10 28.66384424   32  20  43.7829568
c  1404+286 OQ208    14 07 .39441078     28  27  14.6899551
c  0923+392 4C39.25  09 27 3.01391312    39  02  20.8519602
c  0059+581 $        01 02 45.76238297   58  24  11.1366430
c  0104-408 $        01 06 45.10796350  -40  34  19.9602051
c  0528+134 $        05 30 56.41674545   13  31  55.1495463
c  0552+398 $        05 55 30.80560918   39  48  49.1650002
c  0727-115 $        07 30 19.11246873  -11  41  12.6004928
c  1308+326 $        13 10 28.66384424   32  20  43.7829568
c  1741-038 $        17 43 58.85614027  -03  50  4.6167340
c  1921-293 $        19 24 51.05595818  -29  14  30.1210139
c  2145+067 $        21 48 5.45867976    06  57  38.6041487
c  1226+023 3C273B   12 29 6.69973194    02  03  8.5981848 
c  1253-055 3C279    12 56 11.16654206  -05  47  21.5244119
c  2134+004 2134+00  21 36 38.58632352   00  41  54.2132326
c  2251+158 3C454.3  22 53 57.74793975   16  08  53.5608217
c  0534+220 3C144    05 34 31.           22   0   0.   DAGreen SNR catalog
c
      data source_name/'3C84    ','1308+326','OQ208   ','4C39.25 ',
     &                 '0059+581','0104-408','0528+134','0552+398',
     &                 '0727-115','1308+326','1741-038','1921-293',
     &                 '2145+067','3C273B  ','3C279   ','2134+004',
     &                 '3C454.3 ','3C144   ','3C345   ','        '/
      data ra/3.330044,13.174629,14.116776,9.450837,
     &        1.046045,1.112530,5.515671,5.925224,
     &        7.505309,13.174629,17.733016,19.414182,
     &        21.801516,12.485194,12.936435,21.610718,
c     &        22.899374,5.5753,16.716336,0./  3C345 precessed to 2012-019 in next line
     &        22.899374,5.5753,16.7229,0./  
      data dec/41.511695,32.345495,28.454081,39.039126,
     &         58.403094,-40.572211,13.531986,39.813657,
     &         -11.686833,32.345495,-3.834616,-29.241700,
     &         6.960723,2.052388,-5.789312,0.698393,
c     &        16.148211,22.0,39.7850,0./
     &        16.148211,22.0,39.810276,0./
      data rtod/57.29578/
      ierr = 0
c
c  Find matching station in array.
c
      jsta = 0
      do i=1,21
         if( sta .eq. sta_name(i) ) jsta = i
      enddo
      if( jsta .eq. 0 ) then
         parang = 1.e9
         ierr = -1
         return
      endif
c
c  Find matching source in array.
c
      jsrc = 0
      do i=1,20
         if( source .eq. source_name(i) ) jsrc = i
      enddo
      if( jsrc .eq. 0 .and. myra .ne. 0.0 .and. mydc .ne. 0.0 ) then
         jsrc = 20
         source_name(jsrc) = source
         ra(jsrc) = myra
         dec(jsrc) = mydc
         write(6,'("# Using source ",a8," at RA ",f9.6," Dec ",f9.6)')
     &      source_name(jsrc),ra(jsrc),dec(jsrc)
      endif
      if( jsrc .eq. 0 ) then
         parang = 1.e9
         ierr = -2
         return
      endif
c
c  Get axis type from table if value in subroutine call is <= 0.
c
      if( axis .le. 0 ) axis = sta_axis(jsta)
c
c  Compute parallactic angle, after first calculating UT, GST, & HA.
c
      ut = ihr + imin/60. + isec / 3600.
c
c                  |||||| GST(hr) at 0h UT on 0 Jan 2012
      gst0 = amod( 6.6050 + idoy * 24. / 365.2422, 24.)
      gst = amod( gst0 + ut * 1.002738, 24.)
      ha = amod( gst - sta_long(jsta)/15. - ra(jsrc), 24.)
      if( ha .lt. -12. ) then
         ha = ha + 24.
      else if( ha .gt. 12.) then
         ha = ha - 24.
      endif
      clat = cos( sta_lat(jsta) / rtod )
      slat = sin( sta_lat(jsta) / rtod )
      cha  = cos( 15. * ha / rtod )
      sha  = sin( 15. * ha / rtod )
      cdec = cos( dec(jsrc) / rtod )
      sdec = sin( dec(jsrc) / rtod )
      parang_azel = atan2( clat*sha, slat*cdec - clat*sdec*cha )
      parang_azel = parang_azel * rtod
c
      sel = sdec * slat + cdec * clat * cha
      cel = sqrt( 1. - sel * sel )
      el  = atan( sel / cel ) * rtod
      saz = -cdec * sha
      caz = sdec * clat - cdec * slat * cha
      az  = atan2( saz, caz ) * rtod
      if( az .lt. 0. ) az = az + 360.
c
      azr(1,1) = az / rtod
      elr(1,1) = el / rtod
      decr = dec(jsrc) / rtod
      sitlatr(1) = sta_lat(jsta) / rtod
      kaxis(1) = axis
      call pang(azr,elr,decr,sitlatr,kaxis,pangld)
      parang = pangld(1)
      parang2 = parang
c
c      write(6,'(i4.3,3i3.2,2x,a1,2x,a8,4f8.2)')
c     &       idoy,ihr,imin,isec,sta,source,ha,az,el,parang
c      write(6,'(i4.3,3i3.2,2x,a1,2x,a8,3i4,f9.2)')
c     &    idoy,ihr,imin,isec,sta,source,jsta,jsrc,axis,parang
c      write(6,'(8f12.6)') ut,gst,ha,az,el,parang_azel,parang
c      parang2 = asin( clat * sha / cel ) * rtod
c
c      write(6,'(8f12.7)') ut,gst,ha,az,el,parang,parang2
c
      return
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      SUBROUTINE PANG (AZ, ELEV, DEC , SITLAT, KAXIS, PANGL)
      IMPLICIT NONE
!
! 6.    PANG
!
! 6.1   PANG PROGRAM SPECIFICATION
!
! 6.1.1 PANG is the geometry section of the feedhorn rotation angle
!       module. PANG computes the feedhorn rotation angle due to feed
!       box rotation for each site depending on its antenna type.
!
! 6.1.2 RESTRICTIONS - NONE
!
! 6.1.3 REFERENCES - NONE
!
! 6.2   PANG PROGRAM INTERFACE
!
! 6.2.1 CALLING SEQUENCE -
!
!         INPUT VARIABLES:
!
!         1. AZ(2,2)      - The azimuth angle of the source and its
!                           CT time derivative at each observing
!                           site (RAD, RAD/SEC).
!
!         2. ELEV(2,2)    - The elevation angle of the source and its
!                           CT time derivative at each observing
!                           site (RAD, RAD/SEC).
!
!         3. SITLAT(2)    - The site geodetic latitudes (RAD).
!
!         4. KAXIS(2)     - The antenna axis types for each
!                           observing site (unitless).
!
!         5. DEC          - 2000 source declination
!         replaced 5. STAR(3)      - The 2000.0 source unit vector. (unitless)
!
!
!         OUTPUT VARIABLES:
!
!         1. PANGL(2)     - The feedhorn rotation angle for each site (DEG!).
!
! 6.2.2 COMMON BLOCKS -
!
      REAL*8         PI, TWOPI, HALFPI, CONVD
!
! 6.2.3  PROGRAM SPECIFICATIONS -
!
      REAL*8 AZ(2,2), ELEV(2,2), SITLAT(2), PANGL(2),
     &       PANGL_DOT(2), ARG1, ARG1_DOT, ARG2, ARG2_DOT,
     &       CDEC, SDEC, CEL, SEL, SLAT, CLAT, CAZ, SAZ, SX, CX,
     &       X_ANGLE_DOT, X_ANGLE,
     &       DEC
!
      INTEGER*4 I, KAXIS(2)
!
      data CONVD/0.0174533d0/,PI/3.14159265/
!
!
! 6.2.4  DATA BASE ACCESS -
!
!              'PUT' VARIABLES:
!
!               1.  PANGL(2)  - Feedbox rotation angle at site 1 and 2
!                               in degree of phase.
!
!               ACCESS CODES:
!
!               1. 'PARANGLE' -  The database access code of the
!                                feedbox rotaion angles.
!
! 6.2.5 EXTERNAL INPUT/OUTPUT -
!
!              INPUT VARIABLES: NONE
!
!              OUTPUT VARIABLES:
!
!              1.  POSSIBLE DEBUG OUTPUT
!
! 6.2.6 SUBROUTINE INTERFACE -
!
!             CALLER SUBROUTINES: DRIVC
!
!             CALLED SUBROUTINES: PUT4_S
!
! 6.2.7 CONSTANTS USED - TWOPI
!
! 6.2.8 PROGRAM VARIABLES -
!
! 6.2.9 PROGRAMMER - GREGG COOKE   05/20/89   (CREATION)
!                    89.07.07 Jim Ryan - Comments simplified.
!                    Jim Ryan 89.12.12 UNIX-like database interface
!                    implimented.
!                    89.12.28 Jim Ryan Feed box rotation rate code added.
!
!     PANG PROGRAM STRUCTURE
!
      HALFPI = PI/2.0
      TWOPI = 2.0*PI
!
!     Compute SIN and COSINE of source declination.
      SDEC = dsin(dec)       !
      CDEC = dcos(dec)
!
!     Calculate the feedhorn rotation angle for each site.  Calculate
!     SIN and COS of obs azimuth and elevation, and site geodetic
!     latitude first.
!
      DO I = 1, 2
        SAZ = DSIN (AZ(I,1))
        CAZ = DCOS (AZ(I,1))
        CEL = DCOS (ELEV(I,1))
        SEL = DSIN (ELEV(I,1))
        SLAT = DSIN (SITLAT(I))
        CLAT = DCOS (SITLAT(I))
!
!       Calculate feedhorn rotation angles for 5 different axis types:
!
!       1 - Equatorial mount
!       2 - XY mount, fixed axis N-S (MOJAVE)
!       3 - Ax-El mount
!       4 - XY mount, fixed axis E-W
!       5 - Richmond, Fla., special case.
!
!       Initialize feedbox rotation angles to zero, which is the
!       feedbox angle for equatorial mount (IAXIS = 1).
        PANGL(I)     = 0.D0
        PANGL_DOT(I) = 0.D0
!
!       feedhorn rotation angle for XY mount, N-S axis fixed (IAXIS = 2).
        IF (KAXIS(I).EQ.2) THEN
          X_ANGLE =  DATAN2 (SAZ*CEL, SEL)
          SX = DSIN (X_ANGLE)
          CX = DCOS (X_ANGLE)
          X_ANGLE_DOT =  (CX**2)
     &                 * ( CAZ*CEL*AZ(I,2)/SEL
     &                    -SAZ*SEL*ELEV(I,2)/SEL
     &                    -SAZ*(CEL**2)*ELEV(I,2)/(SEL**2) )
!
          ARG1     = -(SLAT * SX)       / CDEC
          ARG1_DOT = -(SLAT * CX * X_ANGLE_DOT)/ CDEC
!
          ARG2     = -(SX * SDEC * CEL * CAZ - SX * CLAT) /
     &                (CDEC * SAZ * CEL)
          ARG2_DOT = -(
     &                 ( CX * SDEC * CEL * CAZ * X_ANGLE_DOT
     &                  -SX * SDEC * SEL * CAZ * ELEV(I,2)
     &                  -SX * SDEC * CEL * SAZ * AZ  (I,2)
     &                  -CX * CLAT *             X_ANGLE_DOT)/
     &                  (CDEC * SAZ * CEL)
     &                + (SX * SDEC * CEL * CAZ - SX * CLAT)
     &                  *(-1D0/(CDEC * SAZ * CEL)**2) *
     &                  ( CDEC * CAZ * CEL * AZ(I,2)
     &                   -CDEC * SAZ * SEL * ELEV(I,2))   )
!
          PANGL(I)     = -DATAN2 (ARG1, ARG2)
          PANGL_DOT(I) = -(DCOS(PANGL(I))**2) *
     &                   (ARG1_DOT*ARG2-ARG1*ARG2_DOT)/(ARG2**2)
        ENDIF
!
!       feedhorn rotation angle for AZ-EL mount (IAXIS = 3).
        IF (KAXIS(I).EQ.3) THEN
          ARG1     = -CLAT * SAZ / CDEC
          ARG1_DOT = -CLAT * CAZ * AZ(I,2) /CDEC
!
          ARG2     = - (SDEC * SEL - SLAT) / (CDEC * CEL)
          ARG2_DOT = - (SDEC/CDEC)*ELEV(I,2)
     &               - (SDEC*SEL-SLAT)*SEL*ELEV(I,2)/(CDEC*CEL**2)
!
          PANGL(I) = DATAN2 (ARG1, ARG2)
          PANGL_DOT(I) = (DCOS(PANGL(I))**2) *
     &                   (ARG1_DOT*ARG2-ARG1*ARG2_DOT)/(ARG2**2)
        ENDIF
!
!       feedhorn rotation angle for XY mount  E-W fixed axis (IAXIS = 4).
!       (from Bob Potash, 8/26/93)
        IF (KAXIS(I).EQ.4) THEN
           ARG1=SDEC*SLAT-SEL
           ARG2=SDEC*CEL*SAZ*CLAT
!
        PANGL(I) = DATAN2(ARG1,ARG2)
        ENDIF
!
!       Feedhorn rotation angle for Richmond, Fla. (IAXIS = 5).
        IF (KAXIS(I).EQ.5) THEN
          SLAT = DSIN ( HALFPI - 39.06*CONVD + SITLAT(I) )
          CLAT = DCOS ( HALFPI - 39.06*CONVD + SITLAT(I) )
!
          ARG1     = -CLAT * SAZ / CDEC
          ARG1_DOT = -CLAT * CAZ * AZ(I,2) /CDEC
!
          ARG2     = -(SDEC * SEL - SLAT) / (CDEC * CEL)
          ARG2_DOT = - (SDEC/CDEC)*ELEV(I,2)
     &               - (SDEC*SEL-SLAT)*SEL*ELEV(I,2)/(CDEC*CEL**2)
!
          PANGL(I) = DATAN2 (ARG1, ARG2)
          PANGL_DOT(I) = (DCOS(PANGL(I))**2) *
     &                   (ARG1_DOT*ARG2-ARG1*ARG2_DOT)/(ARG2**2)
        ENDIF
!
!
!       Convert feedhorn rotation angle to degrees and flip the sign
!       per Jim Ray's empirical tests.
        PANGL(I) = -PANGL(I) * 360.D0 / TWOPI
c
c  And flip the sign again, to agree with astronomical definition of
c    parallactic angle!
c
        pangl(i) = -pangl(i)
!
!       Convert the feedhorn rotation rate to sec/sec and flip sign.
!D      PANGL_DOT(I) = (-PANGL_DOT(I)/TWOPI)/REF_FREQ
      ENDDO
!
!
!     Normal conclusion.
  500 RETURN
      END
