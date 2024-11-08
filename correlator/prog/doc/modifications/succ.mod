2000/04/12, 09:22; rjc; succ; net_app/GetC3Comms; On failed buffer allocation, retry rather than closing down socket.
2000/05/18, 09:00; rjc; succ; DefaultDelays; Inserted su_id, channel, & good checksum into default (hex) header packets.
2000/08/17, 09:11; rjc; succ; append_ptt_as_fail_params; generate a 0xbaddbadd footage code to signify a pos-tot timeout
2000/10/02, 12:39; rjc; succ; su_conf,dm_cntrl; Debug printout for array code of SU & clock msgs.
2000/11/01, 23:59; rjc; succ; proccmd; Send ABORT_SU_OPS command to pcm_cntl
2001/02/16, 09:13; rjc; succ; dm_cntrl; clear CurrentTOT so sticky pTOT value doesn't clobber servoing
2001/04/05, 16:23; rjc; succ; trm_cntl; bug fix for ROT_TOT_OFFSET in Mk4 mode (affected sync)
2001/10/01, 16:39; rjc; succ; dm_cntrl; Send true sysclk/stnclk value to header_gen (with speedup).
2001/10/29, 18:10; rjc; succ; mph.cc; For 2 headstack recordings, select correct field for positioning.
2001/11/02, 09:48; rjc; succ; dm_cntrl.cc; Fixed duration field of sampler_statistics message.
2002/08/15, 17:14; rjc; succ; dm_cntrl; Removed diagnostic stmts. used to detect ROT jumps; caused queue fail for ap other than 500 ms.
2004/03/09, 15:06; rjc; succ; mpoll; Try to avoid checking for stopped tape motion when doing Mk5.
