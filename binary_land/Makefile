#
# Created by gmakemake (Ubuntu Jul 25 2014) on Fri Jan 15 22:54:05 2021
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

########## Default flags (redefine these with a header.mak file if desired)
CXXFLAGS =	
CFLAGS =	-O2 -Wall -Wextra -pedantic -std=c99 
CLIBFLAGS =	-lm -lncurses
CCLIBFLAGS =	
########## End of default flags

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	binary_land.c binary_land_hash_set.c binary_land_solve.c binary_land_stage.c get_line.c priority_queue.c
PS_FILES =	
S_FILES =	
H_FILES =	binary_land_hash_set.h binary_land_solve.h binary_land_stage.h get_line.h priority_queue.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	binary_land_hash_set.o binary_land_solve.o binary_land_stage.o get_line.o priority_queue.o 

#
# Main targets
#

all:	binary_land 

binary_land:	binary_land.o $(OBJFILES)
	$(CC) $(CFLAGS) -o binary_land binary_land.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

binary_land.o:	binary_land_solve.h binary_land_stage.h
binary_land_hash_set.o:	binary_land_hash_set.h
binary_land_solve.o:	binary_land_hash_set.h binary_land_solve.h binary_land_stage.h priority_queue.h
binary_land_stage.o:	binary_land_stage.h get_line.h
get_line.o:	get_line.h
priority_queue.o:	priority_queue.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) binary_land.o core

realclean:        clean
	-/bin/rm -f binary_land 
