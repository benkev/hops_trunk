2000/01/13, 14:58; rjc; conductor; action_background.c; Put source & suffix both in cond_status message (for opera to display).
2000/01/19, 17:57; rjc; conductor; action_kill_station.c; fixed bug in remove_station functionality
2000/02/07, 15:22; rjc; conductor; action_read_streams.c; Implemented a new tsf command called hold, to put stream into hold state.
2000/02/07, 16:24; rjc; conductor; take_action.c,action_kill_station.c; Correct logic for remove station while awaiting valid times.
2000/03/29, 10:44; rjc; conductor; con_structs action_background identify_transition action_start_rot get_next_event iniitialize_substates log_state; log time stamp and duration of each state into /correlator/tmp/cond_state_log
2000/04/19, 12:54; rjc; conductor; update_clog,take_action,action_control_stream,initialize_substates,action_create_task,log_state,con_symbols.h,con_structs.h; Implement a general accounting facility per experiment.
2000/04/27, 09:10; rjc; conductor; update_clog; Temporary mod to trap bogus total statistics.
2000/04/27, 15:05; rjc; conductor; create_fsm; Add new state called MOUNTING for accounting purposes.
2000/05/18, 11:04; rjc; conductor; initialize_substates; Clear state_timer's to stop correlation_log corruption.
2000/06/06, 14:08; rjc; conductor; action_background; wait 3 secs after scan end before sending correlator abort
2000/06/06, 14:37; rjc; conductor; action_control_stream; send appropriate abort_flags to corrman on scan break
2000/06/26, 15:02; rjc; conductor; update_clog; fix corruption of total times in correlation_log
2000/07/25, 13:17; rjc; conductor; set_rotyear, take_action; set ROT to correct year before pos.to.TOT (VLBA day 1000 bug)
2000/07/27, 17:49; rjc; conductor; action_read_streams; Enter hold state at end of tsf (also fixes accounting bug).
2000/08/24, 09:28; rjc; conductor; take_action; shutdown correlator and suman on CU_RESOURCE_ERRRROR
2000/10/11, 16:02; rjc; conductor; action_create_task; put time stamp on task create console message
2000/11/21, 10:56; rjc; conductor; action_kill_station; Handle Remove_Station msg. properly when in INVALID_TIME_HOLD state.
2001/03/08, 15:38; rjc; conductor; action_read_streams; fixed bug in slice command within a .tsf file
2001/03/29, 11:51; rjc; conductor; action_background; Wait 1 AP after scan end before forcing correlator shutdown.
2001/04/02, 15:19; rjc; conductor; action_background; fix immediate event code when two streams are active
2001/04/04, 09:56; rjc; conductor; action_background; Fix bug in units of ap_length.
2001/05/16, 10:02; rjc; conductor; action_read_streams; clean up support of fork/fend construct in tsf files
2001/05/18, 09:32; rjc; conductor; action_configure_tapes; Save speedup_factor for rotman's use
2001/05/25, 15:14; rjc; conductor; action_read_streams; skip over fork/fend statements when resuming ts file
2001/06/13, 13:59; rjc; conductor; action_control_stream, action_read_streams; Clear out mode & ekey overrides, so they don't stick around.
2001/09/17, 10:26; rjc; conductor; action_create_task,action_control_stream,update_clog; Fixed bug that caused acct'g. for only the last task in a stream.
2001/09/25, 10:50; rjc; conductor; update_clog; fix acct'g. bugs
2002/09/10, 14:45; rjc; conductor; update_clog; limit MOUNT and REFIND times to 15 mins.; excess to STREAM HOLD
2003/08/26, 14:16; rjc; conductor; action_read_streams; spool forked threads into different streams to avoid collisions
2004/02/26, 09:48; rjc; conductor; message_structs.h; Added realtime_mode to Configure_Sus message.
2004/02/27, 09:18; rjc; conductor; calculate_times,action_create_task,action_read_streams,action_configure_tapes,con_structs.h,action_control_stream,action_start_rot,take_action; Various mods necessary to support realtime correlation.
