/************************************************************************/
/*                                                                      */
/* This fills in a type 222 record with the set-string and white-space  */
/* stripped control file record                                         */
/*                                                                      */
/*      Inputs:         param_struct and pointer to type_222 to fill    */
/*                                                                      */
/*      Output:         return value    0=OK, else bad                  */
/*                                                                      */
/* Created Jan 24 2017 JPB                                              */
/*                                                                      */
/************************************************************************/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "msg.h"
#include "mk4_data.h"
#include "mk4_dfio.h"
#include "param_struct.h"
#include "adler32_checksum.h"

int
fill_222(
struct type_param *param,
struct type_222 **t222)
    {
    int setstr_len, cf_len, setstr_pad, cf_pad, full_size, i;
    unsigned int setstr_hash = 0;
    unsigned int cf_hash = 0;
    //needed for stripping leading and trailing whitespace before computing cf hash
    int j, cf_start, cf_stop;  
    
    //now allocate the necessary amount of memory
    if(param->set_string_buff != NULL)
        {
        setstr_len = strlen(param->set_string_buff);
        }
    else
        {
        setstr_len = 0;
        }

    if(param->control_file_buff != NULL)
        {
        cf_len = strlen(param->control_file_buff);
        }
    else
        {
        cf_len = 0;
        }

    //find next largest multiple of 8 bytes
    setstr_pad = (( setstr_len + 7 ) & ~7) + 8;
    cf_pad = ( (cf_len + 7 ) & ~7) + 8;
    full_size = sizeof(struct type_222) + setstr_pad + cf_pad; 
    
    /* Allocate space for output record */
    *t222 = (struct type_222*) malloc ( full_size );
    if (*t222 == NULL)
        {
        msg ("Memory allocation failure in fill_222()", 2);
        return (-1);
        }

    //figure out the stop/stop of the cf tokens without the
    //leading/trailing white space since the control file parser
    //is not consistent about how many whitespace tokens may appear
    //at the beginning or end of the processed text
    cf_start = 0;
    cf_stop = cf_len;
    for(j=0; j<cf_len;j++)
    {
        cf_start = j;
        if( (param->control_file_buff[j] != ' ') && (param->control_file_buff[j] != '\t') && (param->control_file_buff[j] != '\n') )
        { break; }
    }
    for(j=cf_len-1; j>=0; j--)
    {
        cf_stop = j;
        if( (param->control_file_buff[j] != ' ') && (param->control_file_buff[j] != '\t') && (param->control_file_buff[j] != '\n') )
        { break; }
    }

    //now do the hashing
    setstr_hash = adler32_checksum( (unsigned char*) param->set_string_buff, setstr_len);
    cf_hash = adler32_checksum( (unsigned char*) &(param->control_file_buff[cf_start]), cf_stop-cf_start);

    /* Fill it in */
    strncpy ( (*t222)->record_id, "222", 3);
    strncpy ( (*t222)->version_no, "00", 2);
    (*t222)->padded = 0;
    (*t222)->setstring_hash = setstr_hash;
    (*t222)->control_hash = cf_hash;
    (*t222)->setstring_length = setstr_len;
    (*t222)->cf_length = cf_len;
    
    memcpy ( (*t222)->control_contents, param->set_string_buff, setstr_len );
    for(i=setstr_len; i<setstr_pad; i++){ ((*t222)->control_contents)[i] = '\0';}
    
    //set the starting position of the control contents to the right place
    memcpy ( &( ((*t222)->control_contents)[setstr_pad] ),  param->control_file_buff, cf_len);
    for(i=setstr_pad+cf_len; i<setstr_pad+cf_pad; i++){ ((*t222)->control_contents)[i] = '\0';}
    
    return 0;
    }
