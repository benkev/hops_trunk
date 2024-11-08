#ifndef MK4_BFIO_H
#define MK4_BFIO_H

#include "data.h"
#include "type_4000.h"
#include "type_4100.h"
#include "type_4200.h"
#include "type_4300.h"
#include "type_4400.h"
#include "type_4500.h"

extern int  check_corel_record (struct type_corel *);
extern void check_reverse (void);
extern void clear_4000 (struct type_4000 *);
extern void clear_4100 (struct type_4100 *);
extern void clear_4200 (struct type_4200 *);
extern void clear_4300 (struct type_4300 *);
extern void clear_4400 (struct type_4400 *);
extern void clear_4500 (struct type_4500 *);
extern void clear_fringe (struct data_fringe *);
extern void copybuf (short *, short *, int);
extern void fill_baserecs (struct data_root *, int *);
extern void free_matrix (char **);
extern char **make_matrix (int, int);
extern int  read_corel (char *, struct type_corel *[], int *);
extern int  read_fringe (char *, struct data_fringe *);
extern int  read_fringe_quick (char *, struct data_fringe *);
extern int  read_root (char *, struct data_root *);
extern void reverse_datec (struct datec *);
extern void reverse_binfo (struct binfo *);
extern void reverse_crossref (struct crossref *);
extern void reverse_loffset (struct loffset *);
extern void reverse_datef (struct datef *);
extern void reverse_sbands (struct sbands *);
extern void reverse_istats (struct istats *);
extern void reverse_stsband (struct stsband *);
extern void reverse_pcinfo (struct pcinfo *);
extern void reverse_polar (struct polar *);
extern void reverse_fstats (struct fstats *);
extern void reverse_accrec (struct accrec *);
extern void reverse_corel (struct type_corel *);
extern void reverse_1000 (struct type_1000 *);
extern void reverse_2000 (struct type_2000 *);
extern void reverse_2100 (struct type_2100 *);
extern void reverse_2200 (struct type_2200 *);
extern void reverse_2300 (struct type_2300 *);
extern void reverse_2400 (struct type_2400 *);
extern void reverse_2500 (struct type_2500 *);
extern void reverse_2600 (struct type_2600 *);
extern void reverse_2700 (struct type_2700 *);
extern void reverse_2800 (struct type_2800 *);
extern void reverse_4000 (struct type_4000 *);
extern void reverse_4100 (struct type_4100 *);
extern void reverse_4200 (struct type_4200 *);
extern void reverse_4300 (struct type_4300 *);
extern void reverse_4400 (struct type_4400 *);
extern void reverse_4500 (struct type_4500 *);
extern void reverse_5000 (struct type_5000 *);
extern int  write_fringe (struct data_fringe *, char *);
extern int  write_root (struct data_root *, char *);
extern int  write_root_crunched (struct data_root *root, char *filename);

#endif
