      program dsolv
c
c Solve for Bx, By, Bz, clock offset, and clock rate
c  from delay time series.
c
c Units:
c   param(1) = Bx (meters)
c   param(2) = By (meters)
c   param(3) = Bz (meters)
c   param(4) = clock offset (microsec)
c   param(5) = clock rate (ps/s)
c        
c Input data must be in same format as for bsolv/csolv,
c  although delayrate isn't used.
c
c                            bec    2016 feb 26
c
c as usual, hacked up in minor ways by gbc while grokking it.
c
c this version accepts source position as hrs and degs (float)
c
      parameter(maxnpts=1000)
      character string*200,infile*100
      integer hhmm,doy(maxnpts),hour(maxnpts),minute(maxnpts)
      logical eof
      real*4 delay(maxnpts),x(maxnpts,5),em(5,5),covar(5,5),param(5),
     &       delaymod(maxnpts),delayres(maxnpts),
     &       gharad(maxnpts),decrad(maxnpts),
     &       rahhmm(maxnpts),decddmm(maxnpts)
      real*8 covar8(10,10),determ8
      character*40 pdefs(5)
      real*4 slt
      data pi/3.1415926536/,rtod/57.29578/
      data pdefs/"delta Dx(m)","delta Dy(m)","delta Dz(m)",
     & "clock offset(us)","clock rate(ps/s)"/
      data slt/299.279458/
c
c Get input file name.
c
      if( iargc() .lt. 1 ) then
        write(0,'("Command line: dsolv INFILE",/,
     &   "Input file must have >5 free-field data lines in format:",/,
     &   "  DOY,UT_epoch(HHMM),RA(HHMM),dec(DDMM),"
     &      "rate(ps/s),delay(us)",/,
     &   "Rates are not used in fit, so values can be dummies.")')
        stop
      endif
      call getarg(1,infile)
      open(10,file=infile,status='old',iostat=ios,err=110)
c
c Read input data.
c
      npts = 0
      write(6,'("#n HH MM design matrix row elements")')
 10   call getstring(10,string,lenstr,eof)
      if( eof ) goto 20
      if( string(1:1) .eq. '*' .or. string(1:1) .eq. '#' ) goto 10
      npts = npts + 1
      read(string,*) doy(npts),hhmm,rahhmm(npts),decddmm(npts),
     &               rate,delay(npts)
c
c Compute design matrix elements.
c   follows from Thompson, Moran & Swenson (1986), Eq. (4.27)
c   with phi_in errors from clock offset and rate.
c
      hour(npts)   = hhmm / 100
      minute(npts) = hhmm - 100*hour(npts)
      utchr = dangg(hhmm)
      rahr  = rahhmm(npts)
      decdg = decddmm(npts)
      gst0 = ( doy(npts) - 264 ) * 24. / 365.25
      ghahr = gst0 + utchr - rahr
      if( npts .eq. 1 ) utchr_ref = utchr
      gharad(npts) = ghahr*15./rtod
      decrad(npts) = decdg/rtod
      x(npts,1) = - cos( decdg/rtod ) * cos( ghahr*15./rtod ) / slt
      x(npts,2) =   cos( decdg/rtod ) * sin( ghahr*15./rtod ) / slt
      x(npts,3) = - sin( decdg/rtod ) / slt
      x(npts,4) = 1.
      x(npts,5) = ( utchr - utchr_ref ) * 3600 * 1.e-6
      write(6,'(3i3,5f10.5)')
     &   npts, hour(npts),minute(npts),(x(npts,j),j=1,5)
      goto 10
c
c Solve for 5 parameters.
c
 20   if( npts .le. 5 ) then
         write(0,'("Too few data points -- must be >5.")')
         stop
      endif
      write(6,'("#Read a total of " i5 " points")') npts
      do j=1,5
         do k=1,5
            em(j,k) = 0.
            do i=1,npts
               em(j,k) = em(j,k) + x(i,j) * x(i,k)
            enddo
            covar8(j,k) = em(j,k)
         enddo
c         write(6,'(5f15.8)') (em(j,k),k=1,5)
      enddo
      call minv(covar8,5,determ8)
      do j=1,5
c         write(6,'(5f15.8)') (covar8(j,k),k=1,5)
         param(j) = 0.d0
         do k=1,5
            do i=1,npts
               param(j) = param(j) +
     &                    covar8(j,k) * delay(i) * x(i,k)
            enddo
         enddo
      enddo
c
c Compute delay residuals to model and write out results.
c Also compute and write out delay rate model.
c
      write(6,'("#DOY HH MM    RAhhmm   decddmm   delay"
     &          "     dlymod    dlyres    GHA(hr) ratemod")')
      do i=1,npts
         delaymod(i) = 0.
         do j=1,5
            delaymod(i) = delaymod(i) + param(j) * x(i,j)
         enddo
         delayres(i) = delay(i) - delaymod(i)
         ratemod = cos(decrad(i)) * ( param(1) * sin(gharad(i))
     &                              + param(2) * cos(gharad(i)) )
         ratemod = ratemod * 7.29e-5 * 1.e6 / slt + param(5)
         write(6,'(1x,i3.3,2i3.2,2x,f8.4,2x,f8.4,3f10.5,
     &             f10.3,f8.3)')
     &   doy(i),hour(i),minute(i),rahhmm(i),decddmm(i),
     &   delay(i),delaymod(i),delayres(i),gharad(i)*rtod/15.,
     &   ratemod
      enddo
      write(6,'("#")')
      write(6,'("# Model fit parameters:")')
      do i=1,5
         write(6,'("# param",i1," = ",f12.6," ",a40)')
     &           i,param(i),pdefs(i)
      enddo
      write(6,'("#")')
      stop
c
c Error messages.
c
 110  write(*,'("Error ",i6," opening file ",a)') ios,infile
      stop
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      real*4 function dangg(hhmm)
      integer hhmm
      hh = abs(hhmm)
      sign = 1.
      if( hhmm .lt. 0. ) sign = -1.
      ih1 = hh / 100.
      h1 = ih1
      dangg = ( h1 + (hh - h1*100.) / 60.) * sign
      return
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      SUBROUTINE MINV(A,N,DETERM)
      IMPLICIT real*8(A-H,O-Z)
      DIMENSION IPIVOT(10),INDEX(10,2),PIVOT(10),A(10,10)
      DETERM=1.D0
      DO 20 J=1,N
 20   IPIVOT(J)=0
      DO 550 I=1,N
      AMAX=0.D0
      DO 105 J=1,N
      IF(IPIVOT(J)-1)60,105,60
 60   DO 100 K=1,N
      IF(IPIVOT(K)-1)80,100,740
 80   IF(DABS(AMAX)-DABS(A(J,K)))85,100,100
 85   IROW=J
      ICOLUM=K
      AMAX=A(J,K)
 100  CONTINUE
 105  CONTINUE
      IPIVOT(ICOLUM)=IPIVOT(ICOLUM)+1
      IF(IROW-ICOLUM)140,260,140
 140  DETERM=-DETERM
      DO 200 L=1,N
      SWAP=A(IROW,L)
      A(IROW,L)=A(ICOLUM,L)
 200  A(ICOLUM,L)=SWAP
 260  INDEX(I,1)=IROW
      INDEX(I,2)=ICOLUM
      PIVOT(I)=AMAX
      DETERM=DETERM*PIVOT(I)
      IF(DETERM)330,325,330
 325  RETURN
 330  A(ICOLUM,ICOLUM)=1.D0
      DO 350 L=1,N
 350  A(ICOLUM,L)=A(ICOLUM,L)/PIVOT(I)
      DO 550 L1=1,N
      IF(L1-ICOLUM)400,550,400
 400  SWAP=A(L1,ICOLUM)
      A(L1,ICOLUM)=0.D0
      DO 450 L=1,N
 450  A(L1,L)=A(L1,L)-A(ICOLUM,L)*SWAP
 550  CONTINUE
      DO 710 I=1,N
      L=N+1-I
      IF(INDEX(L,1)-INDEX(L,2))630,710,630
 630  IROW=INDEX(L,1)
      ICOLUM=INDEX(L,2)
      DO 705 K=1,N
      SWAP=A(K,IROW)
      A(K,IROW)=A(K,ICOLUM)
      A(K,ICOLUM)=SWAP
 705  CONTINUE
 710  CONTINUE
 740  RETURN
      END
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      subroutine getstring(unit,string,lenstr,eof)
c
c  Get a character string from a file.
c  Return its length to the last nonblank character, and
c   check for end-of-file.
c
      character*(*) string
      integer trimlen,unit
      logical eof
c
      call clear(string,len(string))
      read(unit,'(a)',end=10) string
      lenstr = trimlen(string)
      eof = .false.
      return
c
 10   eof = .true.
      return
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      integer function trimlen(string)
c
c  Return length of used portion of string (i.e., up to last nonblank
c    character).
c
      integer len_string, i
      logical blanks
      character*(*) string
c
c    Get full length of string, and work backwards.
c 
      len_string = LEN(string)
      i = len_string
      blanks = .true.
c   Scan string from end to front
 10   if ( blanks ) then
          if( i.gt.0 ) then
              if( string(i:i).eq.' ' ) then
                  i = i - 1
              else
                  blanks = .false.
              endif
          else
              blanks = .false.
          endif
          goto 10
      endif
c
      trimlen = i
      return
      end
c
c * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
c
      subroutine clear(string,len)
      character string*(*)
      do i=1,len
         string(i:i) = ' '
      enddo
      return
      end
