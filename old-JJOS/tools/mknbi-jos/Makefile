# Generated automatically from Makefile.in by configure.
#
# Makefile  -  makefile for mknbi program for Linux
#
# Copyright (C) 1995,1998 Gero Kuhlmann   <gero@gkminix.han.de>
# Copyright (C) 1996,1997 Gero Kuhlmann   <gero@gkminix.han.de>
#		 and Markus Gutschke <gutschk@math.uni-muenster.de>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#


# Directory names
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
mandir = ${prefix}/man
netboot = ../../../etherboot/netboot-0.7.3

# CMDAD defines the vendor tag ID to be used for additions to the
# kernel command line. It must be in the range of 128 - 254 as per
# RFC 1533 for site specific tag IDs.
CMDAD  = -DBOOTP_RFC_CMDAD=129


# MANEXT is the manual section number
MANEXT = 8


# ADEBUG define the flags for compiling the boot loader with the
# debugging code enabled:
# ASM_DEBUG             -- enable debugging output while loading and
#                          initializing the kernel and ramdisk image.
# ASM_DEBUG_VERBOSE     -- enable even more debugging output.
# ASM_FREEZE_AFTER_INIT -- freeze the system after successful initiali-
#                          zation; this is right before the actual kernel
#                          is launched. Enable this flag for debugging
#                          purposes only!
ADEBUG = # -DASM_DEBUG -DASM_DEBUG_VERBOSE -DASM_FREEZE_AFTER_INIT


############# Do not touch anything below this line #####################

CC	= gcc
BCC	= 
CPP	= gcc -E
AS86	= /usr/bin/as86
LD86	= 
INSTALL	= /usr/bin/install -c
MAKE=gmake


# these get overriden by the master Makefile if that's used
CDEBUG	= -O2 -Wall # -g
INCLUDE	= -I. -I$(netboot) -I$(netboot)/include
CFLAGS	= $(CDEBUG) -DHAVE_CONFIG_H $(INCLUDE)
LDFLAGS	= -s
LIBS	= 
NO_ASM=true

AFLAGS	= $(CMDAD) $(ADEBUG)


SRCS	= mknbi.c first_c.c makec.c
OBJS	= mknbi.o first_c.o
TARGET	= mknbi-linux


.PHONY:		all

all:		$(TARGET)

$(TARGET):	version.h mknbi

mknbi:		$(OBJS)
		$(CC) -o mknbi $(LDFLAGS) $(OBJS) $(LIBS)


first_c.c:	first.S first.inc makec
ifndef NO_ASM
		$(CPP) $(AFLAGS) first.S -o first.s
		$(AS86) -0 -b first.b first.s
		./makec first <first.b >first_c.c
		@rm -f first.s first.b
else
		touch first_c.c
endif


makec:		makec.c
		$(CC) -o makec makec.c


version.h:	version.h.in
		sed "s/%VER_MAJOR%/0/;s/%VER_MINOR%/7/;s/%PATCHLEVEL%/3/" version.h.in >version.h


install:	$(TARGET)
		$(INSTALL) -d $(bindir)
		$(INSTALL) -d $(mandir)/man$(MANEXT)
		@rm -f $(bindir)/$(TARGET)
		$(INSTALL) -m 511 mknbi $(bindir)/$(TARGET)
		$(INSTALL) -m 644 -o bin -g bin mknbi.man $(mandir)/man$(MANEXT)/$(TARGET).$(MANEXT)


dep:		version.h
		sed '/\#\#\# Dependencies/q' <Makefile >tmp_make
		$(CPP) $(CFLAGS) -MM $(SRCS) >>tmp_make
		mv tmp_make Makefile


.PHONY:		clean distclean realclean
clean:
		rm -f *.o *.s *.b *.map makec mknbi tmp_make core

distclean:	clean
		rm -f Makefile version.h

realclean:	distclean
		rm -f first_c.c

### Dependencies
mknbi.o: mknbi.c $(netboot)/include/common.h \
 $(netboot)/config.h \
 $(netboot)/include/exitcodes.h \
 $(netboot)/include/version.h mknbi.h version.h
first_c.o: first_c.c
makec.o: makec.c
