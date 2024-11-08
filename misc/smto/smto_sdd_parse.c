/*
 * $Id$
 *
 * Parse the damn file
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "smto_sdd.h"

int bootstrap(char *file, SddBoots *bs)
{
    printf("### Bootstrap of %s contains:\n", file);
    printf("###  numrec %d\n", bs->numrec);
    printf("###  numdata %d\n", bs->numdata);
    printf("###  bytes_per_rec %d\n", bs->bytes_per_rec);
    printf("###  bytes_per_index %d\n", bs->bytes_per_index);
    printf("###  num_used %d\n", bs->num_used);
    printf("###  counter %d\n", bs->counter);
    printf("###  sddtyp %d\n", bs->sddtyp);
    printf("###  sddver %d\n", bs->sddver);
    printf("### sizeof(SddBoots) = %d\n", sizeof(SddBoots));
    printf("### sizeof(SddIndex) = %d\n", sizeof(SddIndex));
    printf("### sizeof(SddTypeDesc) = %d\n", sizeof(SddTypeDesc));
    printf("### sizeof(SddPreamble) = %d\n", sizeof(SddPreamble));
    if (bs->sddver != 1)
	fprintf(stderr, "Only version 1 is supported\n");
    if (bs->bytes_per_rec != 512)
	fprintf(stderr, "Record size is not 512\n");
    if (bs->bytes_per_index != 64)
	fprintf(stderr, "Bytes per index is not 64\n");
    return(bs->sddver == 1 ? 0 : 1);
}

int parse_file(char *file, char *plot)
{
    void *sdd;
    struct stat sb;
    int	fd;

    if (stat(file, &sb)) { perror("stat"); return(1); }
    fd = open(file, O_RDONLY);
    if (fd < 0) { perror("open"); return(2); }

    sdd = mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (sdd <= 0) { perror("mmap"); return(3); }

    if (bootstrap(file, (SddBoots*)sdd)) return(4);

    munmap(sdd, sb.st_size);
    return(0);
}

/*
 * eof
 */
