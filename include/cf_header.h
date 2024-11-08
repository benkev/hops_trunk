struct cf_header                /* Definition of 240 bit correlator frame header packet*/
    {
    short int identity;         /*    bits 15..11: SU ID# (0-31)                       */
                                /*      "  10..06: channel ID# (0-31)                  */
                                /*      "  05..00: reserved for future use             */
    short unsigned frame_serial_num; /* incremental serial# for this correlator frame  */
    long int delay_error;       /* initial delay error, in fractional samples   (*2^32)*/
    long int delay_error_rate;  /*    "    delay error rate, in samples / sample   (") */
    long int phase;             /*    "    phase, in rotations                     (") */
    long int phase_rate;        /*    "    phase rate, in rotations / k samples    (") */
    long int phase_acc;         /*    "    phase acceleration, in rots/ k*n samp^2 (") */
    long int phase_periods;     /*    bits 31    : unused                              */
                                /*     "   30    : sideband 0=USB 1=LSB                */
                                /*     "   29..27: oversampling factor {1, 2, 4}       */
                                /*     "   26..24: k -- log2 of phase inc. period      */
                                /*     "   23..00: n -- const. acc. segment length     */
    short unsigned checkword;   /* Fletcher checksum of previous 14 unsigned words     */
    };

