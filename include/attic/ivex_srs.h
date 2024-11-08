/* ivex.h
Tue Feb  2 11:35:07 EST 1999
*/

typedef unsigned short U16;
typedef unsigned long  U32;

enum	{ P_G, Mark4 };
enum	{ HSC0, HSC1, HSC2, HSC3, MAX_HSC };
enum	{ REVERSE, FORWARD };

#define	MAX_CHAN_GRP	4

struct bad_frame_struct
	{
	short	CRC_error;		/* off|on			*/
	short	PE_limit_exceeded;	/* off|on			*/
	short	lost_sync;		/* off|on			*/
	short	unexpected_sync;	/* off|on			*/
	};

struct TRM_config_struct
	{
	short	sign[MAX_MUX];		/* []=0|1|2|3|-1		*/
	short	mag[MAX_MUX];		/* []=0|1|2|3|-1		*/
	double	parity_error_limit;	/* rx parity error threshold	*/
	struct	bad_frame_struct	invalid_frame_control; /* boolean flags	*/
	};

struct vacuum_struct			/* 'y = mx + b', ie linear	*/
	{
	short	inches_of_H2O;		/* eg. 5" and 15"		*/
	U16	setting;
	};

struct headtstack_DIM_connect_struct
	{
	short	headstack;		/* 1|2|3|4			*/
	short	parity;			/* EVEN|ODD|NONE		*/
	short	eqlzr;			/* equalizer (Mbps)		*/
	};

struct moni_read_struct
	{
	short	track[2];		/* REV|FWD: track value		*/
	short	equalizer;		/* 0|1|2=std,alt1,alt2		*/
	};

struct headstack_parms_struct
	{
	/* { ffspeed, fsspeed, rfspeed, rsspeed, pscale, nscale, 0.0, 0.0, fbias, rbias } */
	double	param[10];
	};

struct drive_init_struct
	{
	short	drive_type;			/* Mark4|VLBA			*/
	float	capstan_relative_diameter;	/* eg. 1.014			*/
	struct	vacuum_struct		vacuum[2];	/* eg 5" & 15" hijklmno	*/
	float	tape_acceleration;		/* Mark4 default: 67.5 ips/sec	*/
	struct	headstack_DIM_connect	DIM_connect[MAX_HSC];	/* HSC0..3     	*/
	short	nheads;				/* number of headstacks		*/
	struct	moni_read_struct	monitor_module[nheads];
	struct	headstack_parms_struct	headstack[nheads];
	};
	
struct su_connect_struct
	{
	short	channel_group[MAX_CHAN_GRP];
	short	corr_segment[MAX_CHAN_GRP];
	short	inputbd_chan_grp[MAX_CHAN_GRP];
	};

struct pbs_init_struct
	{
	short				SU_ID;
	char				*cHost;
	short				tracks_per_bitstream;
	struct	TRM_config_struct	TRM_config;
	struct	drive_init_struct	drive;
	struct	su_connect_struct	su_connect;
	};

struct ivex_struct
	{
	float	system_tempo;
	U32	bocf_period;
	short	nplayback_systems;
	struct	pbs_init_struct	pbs_init[16];
	};
