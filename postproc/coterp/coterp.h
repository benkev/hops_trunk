#define FALSE 0
#define TRUE  1

#define SRCH_COTIME 0
#define NOLOSS_COTIME 1

#define MAXSTAT 20

#include "adata.h"

typedef struct
    {
    int			order;
    int			lastorder;
    int			keyval;
    fringesum		fdata;
    } fringedata;

typedef struct
    {
    char		stn_id;
    float		elevation;
    float		srch_cotime;
    float		noloss_cotime;
    } stsum;

typedef struct
    {
    char		baseline[3];
    float		srch_cotime;
    float		noloss_cotime;
    } basesum;

typedef struct
    {
    int 		scantime;
    int			nb;
    basesum		basetimes[MAXSTAT * (MAXSTAT-1) / 2];
    int 		nstat;
    stsum		station[MAXSTAT];
    } scansum;

typedef struct
    {
    int 		scantime;
    int 		nstat;
    stsum		station[MAXSTAT];
    } scanstsum;
