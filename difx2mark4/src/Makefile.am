bin_PROGRAMS = difx2mark4

# flags are handled in configure, however:
AM_CPPFLAGS = -I$(srcdir)/../../include

# testing was never implemented
# TESTS_ENVIRONMENT = srcdir=$(srcdir) HOPS_STATION_CODE=...

difx2mark4_SOURCES = \
	difx2mark4.c \
	createRoot.c \
	createType1s.c \
    new_type1.c \
	createType3s.c \
	root_id.c \
	single_code.c \
	conv2date.c \
	get_vis.c \
	swabr.c \
	write_t100.c \
	write_t101.c \
	write_t120.c \
	write_t300.c \
	write_t301.c \
	write_t302.c \
	write_t303.c \
	write_t309.c \
	other.c \
    normalize.c \
    fill_fblock.c \
	type_writes.h \
	difx2mark4.h \
	other.h \
	d2m4_pcal_copy_record.c \
	d2m4_pcal_count_unique_polarizations.c \
	d2m4_pcal_create_list.c \
	d2m4_pcal_dump_record.c \
	d2m4_pcal_free_list.c \
	d2m4_pcal_free_record.c \
	d2m4_pcal_init_record.c \
	d2m4_pcal_merge_datastreams_in_list.c \
	d2m4_pcal_merge_record.c \
	d2m4_pcal_populate_record.c \
	d2m4_pcal_record.h

# CLEANFILES = *debug*

# eof
