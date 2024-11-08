1999/12/28, 11:19; cjl; genaroot; clock_adjust; Fixed logic error, do_rate was always false
1999/12/28, 11:52; cjl; genaroot; vex_calc; put zatm in nanosec, not sec
1999/12/28, 15:43; cjl; genaroot; clock_adjust; changed units on clockrate adjustment
2000/01/05, 14:01; rjc; genaroot; vex_calc.c,$CALC/calc8/catmm.f,$CALC/calc8/control_init.f; Enable atmosphere, ignores zenatm input values, uses Niell dry model, Saastamoinen zenith value
2000/03/06, 15:52; rjc; genaroot; doy_mmdd; Fixed bug that gave wrong result for February 29th of any leapyear.
2000/03/07, 14:06; rjc; genaroot; lp_year; Cleaned up code; also fixed to accept year as 1900 relative.
2000/05/18, 13:19; rjc; genaroot; get4,vex_calc,calc_data.h; Fix axis offset types by intercepting calc's database call.
2000/08/10, 15:14; rjc; genaroot; generate_task,vex_calc; Removed some debug printout messages.
2001/04/24, 12:02; cjl; genaroot; generate_task; support for LVEX pass override, proper support for mode and ap_length override
2001/05/03, 15:55; rjc; genaroot; vex_calc, model_pts; Inserted code to correctly handle LO offsets
2001/05/11, 11:38; rjc; genaroot; generate_task; insert correct units (sec) on ap_length override
2008/07/23, 09:27; rjc; genaroot; generate_task; Fixed bug that prevented ovex edit while staying in genaroot
