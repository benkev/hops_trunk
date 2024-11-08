/************************************************************************/
/*									*/
/* This is where the Mk4 record format version information is kept.  	*/
/* When a data format is modified, the changes must be reflected in	*/
/* this file, and the DFIO library will use this information to perform	*/
/* the proper translation between the overlay structure and the		*/
/* application structure (see the dfio documentation file for details)	*/
/*									*/
/* Created 15th August 1995 by CJL					*/
/*									*/
/************************************************************************/

#ifndef MK4_VERSION
#define MK4_VERSION

struct disk_record
    {
    short	rec_type;
    short	version;
    void	*overlay;
    };

static struct type_000 *t000;
static struct type_010 *t010;
static struct type_010_v1 *t010_v1;
static struct type_011 *t011;
static struct type_020 *t020;
static struct type_021 *t021;
static struct type_100 *t100;
static struct type_101 *t101;
static struct type_102 *t102;
static struct type_120 *t120;
static struct type_200 *t200;
static struct type_201 *t201;
static struct type_202 *t202;
static struct type_203 *t203;
static struct type_204 *t204;
static struct type_205 *t205;
static struct type_206 *t206;
static struct type_207 *t207;
static struct type_208 *t208;
static struct type_210 *t210;
static struct type_211 *t211;
static struct type_212 *t212;
static struct type_220 *t220;
static struct type_221 *t221;

struct disk_record overlays[] =
    {
    000, 0, (struct type_000 *)t000,
    010, 0, (struct type_010 *)t010,
    010, 1, (struct type_010_v1 *)t010_v1,
    011, 0, (struct type_011 *)t011,
    020, 0, (struct type_020 *)t020,
    021, 0, (struct type_021 *)t021,
    100, 0, (struct type_100 *)t100,
    101, 0, (struct type_101 *)t101,
    102, 0, (struct type_102 *)t102,
    120, 0, (struct type_120 *)t120,
    200, 0, (struct type_200 *)t200,
    201, 0, (struct type_201 *)t201,
    202, 0, (struct type_202 *)t202,
    203, 0, (struct type_203 *)t203,
    204, 0, (struct type_204 *)t204,
    205, 0, (struct type_205 *)t205,
    206, 0, (struct type_206 *)t206,
    207, 0, (struct type_207 *)t207,
    208, 0, (struct type_208 *)t208,
    210, 0, (struct type_210 *)t210,
    211, 0, (struct type_211 *)t211,
    212, 0, (struct type_212 *)t212,
    220, 0, (struct type_220 *)t220,
    221, 0, (struct type_221 *)t221
    } ;
