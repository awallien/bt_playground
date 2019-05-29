#
# Created by gmakemake (Ubuntu Jul 25 2014) on Tue May 28 16:49:25 2019
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

CXXFLAGS =	-ggdb
CFLAGS =	-ggdb -Wall -Wextra -pedantic -std=c99
CLIBFLAGS =	-lm
CCLIBFLAGS =	

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	binairo.c binairo_board.c binairo_bt.c display.c get_line.c hash_info.c
PS_FILES =	
S_FILES =	
H_FILES =	binairo_board.h binairo_bt.h display.h get_line.h hash_info.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	binairo_board.o binairo_bt.o display.o get_line.o hash_info.o 

#
# Main targets
#

all:	binairo 

binairo:	binairo.o $(OBJFILES)
	$(CC) $(CFLAGS) -o binairo binairo.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

binairo.o:	binairo_board.h binairo_bt.h display.h
binairo_board.o:	binairo_board.h get_line.h
binairo_bt.o:	binairo_board.h binairo_bt.h display.h hash_info.h
display.o:	display.h
get_line.o:	get_line.h
hash_info.o:	hash_info.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) binairo.o core

realclean:        clean
	-/bin/rm -f binairo 
