AM_CFLAGS = $(GLOBAL_CFLAGS) -I$(top_srcdir)/src -I$(top_srcdir)/include
AM_LDFLAGS = $(GLOBAL_LDFLAGS)

check_PROGRAMS = \
	test_bpatch \
	test_mbdb_record \
	test_macho \
	test_backup

TESTS = $(check_PROGRAMS)

test_bpatch_SOURCES = test_bpatch.c \
	../src/bpatch.c \
	../src/file.c \
	../src/common.c

test_mbdb_record_SOURCES = test_mbdb_record.c \
	../src/mbdb_record.c \
	../src/mbdb.c \
	../src/common.c

test_macho_SOURCES = test_macho.c \
	../src/macho.c \
	../src/macho_segment.c \
	../src/macho_section.c \
	../src/macho_command.c \
	../src/macho_symtab.c \
	../src/macho_symbol.c \
	../src/file.c \
	../src/common.c

test_backup_SOURCES = test_backup.c \
	../src/backup.c \
	../src/backup_file.c \
	../src/mbdb.c \
	../src/mbdb_record.c \
	../src/file.c \
	../src/common.c
