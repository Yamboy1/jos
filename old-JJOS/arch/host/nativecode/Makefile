.KEEP_STATE:

CC = g++
IFLAGS = -I. -I../../../common/nativecode -I../../../common/decaf
# definable options include:
# -DTARGET_UNIX			;
# -DPARANOID			; include all sorts of consistency checks.  Slow.
# -DNATIVE_CLOCK		; should Java or native code handle clock interrupts?
# -DUSE_CURSES			; use the curses library for console i/o.
#
# development options.
# -DFAST_NATIVE_CODE		; only check for native code when a method's signature is native.
# -DNEW_INTERRUPT_HANDLING	; sane interrupt handling.  Will become the default soon.
# -DUSING_CLASSPATH		; try to load native methods out of
#				; classpath's shared library.  Probably
#				; most useful with -DFAST_NATIVE_CODE.
CFLAGS = -g -Wall -fno-builtin -DTARGET_UNIX -DPARANOID \
 -DNEW_INTERRUPT_HANDLING -DUSE_CURSES $(IFLAGS)

# -lcurses is optional, see above
LDFLAGS = -g -lm -g++ -ldl -lncurses

.SUFFIXES:
.SUFFIXES: .o .cc

TARGET = jjos
TARGET_ZIP = ../bytecode/jjos.zip

# jikes is faster (it seems), and it's what
# classpath uses, but it puts jos/util/Set's
# array in a different place than javac does --
# which gives scheduler.cc fits.

# JAVAC = jikes
JAVAC = javac

HOST_HEADERS = \
 jjconsole.h \
 jjmachin.h \
 jjtimer.h \
 jjramdisk.h
HOST_SOURCES = \
 jjconsole.cc \
 jjmachin.cc \
 jjramdisk.cc \
 jjtimer.cc \
 main.cc
HOST_OBJECTS = \
 jjconsole.o \
 jjmachin.o \
 jjramdisk.o \
 jjtimer.o \
 main.o

HOST_SOURCE_FILES = $(HOST_HEADERS) $(HOST_SOURCES)
ALL_SOURCE_FILES = $(HOST_SOURCE_FILES) $(COMMON_SOURCES) $(DECAF_SOURCES) $(GC_SOURCES)

JAVA_CLASS = init

all: $(TARGET) target_zip

include ../bytecode/Makefile
include ../../../common/nativecode/Makefile
include ../../../common/decaf/Makefile
include ../../../common/nativecode/gc/Makefile

OBJECTS = $(DECAF_OBJECTS) $(COMMON_OBJECTS) $(HOST_OBJECTS) $(GC_OBJECTS)

#
# Cumulative flags
#

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(MORE_LDFLAGS) $(LIBS)

depend:
	makedepend $(IFLAGS) $(ALL_SOURCE_FILES)

clean:
	rm -f $(TARGET) $(OBJECTS) $(TARGET_ZIP) $(ALL_JAVA_OBJECTS) core 

native : $(COMMON_OBJECTS) $(HOST_OBJECTS)

java : $(TARGET_ZIP)

java_clean:
	rm -f $(ALL_JAVA_OBJECTS) $(TARGET_ZIP)

native_clean:
	rm -f $(DECAF_OBJECTS) $(HOST_OBJECTS)

java_list:
	- for file in $(JAVA_SOURCES); \
	       do \
	               ( echo $$file ) ; \
	       done

test-decaf: $(TARGET)
	cd /working/test-decaf; make zipfile
	cp /working/test-decaf/decaf-test.zip ../bytecode/jjos.zip

RCS_OPTS=-u

etags:
	etags -t $(ALL_SOURCE_FILES)

checkin:
	- for file in $(HOST_SOURCE_FILES) Makefile; \
	       do \
	               ( ci $(RCS_OPTS) $$file < /dev/null ) ; \
	       done

checkout:
	- for file in $(HOST_SOURCE_FILES) Makefile; \
	       do \
	               ( co $(RCS_OPTS) $$file < /dev/null ) ; \
	       done

%.o : %.cc
	$(CC) $(CFLAGS) -c $<
