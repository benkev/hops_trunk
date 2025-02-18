/************************************************************************/
/*									*/
/* Parses the default control file, which gives FOURFIT reasonably	*/
/* sensible behaviour for the inexperienced user			*/
/*									*/
/* Created 9 April 1992 by CJL						*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "control.h"
#include "param_struct.h"
#include "ffcontrol.h"
#include "msg.h"

static char def_control_null[] = "/dev/null";

int
set_defaults()
    {
    char *default_cfile;
    extern char* getenv(const char *);
    extern struct c_block *cb_head;
    extern struct type_param param;

    //for lack of a better place to initialize this, we set these to NULL here
    param.control_file_buff = NULL;
    param.set_string_buff = NULL;

    cb_head = NULL;                     /* signifies no c_blocks yet */

                                        /* Set up defaults through prepared */
                                        /* default control file.  Place the */
                                        /* behaviour under shell script control by */
                                        /* using environment variable */
    default_cfile = getenv("DEF_CONTROL");
    if (default_cfile == NULL)
        {
        msg ("DEF_CONTROL undefined; using /dev/null", 2);
        default_cfile = def_control_null;
        }
    if (parse_control_file(default_cfile, &(param.control_file_buff),
        &(param.set_string_buff), 3) != 0)
        {
        msg ("Fatal error parsing default control file '%s'", 3, default_cfile);
        msg ("You must point $DEF_CONTROL (or -c) to a valid fourfit", 3);
        msg ("control file name", 3);
        return (1);
        }
    return (0);
    }
