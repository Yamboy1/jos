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
# -DDONT_USE_DC			; don't use dynamic_cast<>()
CFLAGS = -g -Wall -fno-builtin -DTARGET_UNIX -DPARANOID \
 -DDONT_USE_DC -DNEW_INTERRUPT_HANDLING -DUSE_CURSES $(IFLAGS)

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
# DO NOT DELETE

jjconsole.o: ../../../common/nativecode/jbconsol.h
jjconsole.o: ../../../common/nativecode/jjtypes.h
jjmachin.o: ../../../common/nativecode/jbmachin.h
jjmachin.o: ../../../common/nativecode/jjtypes.h
jjmachin.o: ../../../common/nativecode/jbheap.h
jjmachin.o: ../../../common/nativecode/jbconsol.h
jjmachin.o: ../../../common/nativecode/jbfifo.h /usr/include/sys/types.h
jjmachin.o: /usr/include/features.h /usr/include/sys/cdefs.h
jjmachin.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
jjmachin.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
jjmachin.o: /usr/include/time.h /usr/include/endian.h
jjmachin.o: /usr/include/bits/endian.h /usr/include/sys/select.h
jjmachin.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
jjmachin.o: /usr/include/sys/sysmacros.h
jjramdisk.o: ../../../common/nativecode/jbramdisk.h
jjramdisk.o: ../../../common/nativecode/jjtypes.h
jjramdisk.o: ../../../common/nativecode/stdlib.h
jjconsole.o: /usr/include/stdio.h /usr/include/features.h
jjconsole.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
jjconsole.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
jjconsole.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stdarg.h
jjconsole.o: /usr/include/bits/types.h /usr/include/libio.h
jjconsole.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
jjconsole.o: jjconsole.h ../../../common/nativecode/jbconsol.h
jjconsole.o: ../../../common/nativecode/jjtypes.h
jjmachin.o: ../../../common/nativecode/stdlib.h
jjmachin.o: ../../../common/nativecode/jjtypes.h
jjmachin.o: ../../../common/nativecode/jbfifo.cc
jjmachin.o: ../../../common/nativecode/jbfifo.h jjmachin.h
jjmachin.o: ../../../common/nativecode/jbmachin.h
jjmachin.o: ../../../common/nativecode/jbheap.h
jjmachin.o: ../../../common/nativecode/jbconsol.h /usr/include/sys/types.h
jjmachin.o: /usr/include/features.h /usr/include/sys/cdefs.h
jjmachin.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
jjmachin.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
jjmachin.o: /usr/include/time.h /usr/include/endian.h
jjmachin.o: /usr/include/bits/endian.h /usr/include/sys/select.h
jjmachin.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
jjmachin.o: /usr/include/sys/sysmacros.h ../../../common/nativecode/jbdebug.h
jjmachin.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
jjmachin.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
jjmachin.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
jjmachin.o: /usr/include/bits/confname.h /usr/include/getopt.h
jjramdisk.o: /usr/include/stdio.h /usr/include/features.h
jjramdisk.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
jjramdisk.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
jjramdisk.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stdarg.h
jjramdisk.o: /usr/include/bits/types.h /usr/include/libio.h
jjramdisk.o: /usr/include/_G_config.h /usr/include/bits/stdio_lim.h
jjramdisk.o: /usr/include/sys/mman.h /usr/include/bits/mman.h
jjramdisk.o: /usr/include/sys/stat.h /usr/include/bits/stat.h jjramdisk.h
jjramdisk.o: ../../../common/nativecode/jbramdisk.h
jjramdisk.o: ../../../common/nativecode/jjtypes.h
jjramdisk.o: ../../../common/nativecode/stdlib.h
jjtimer.o: jjtimer.h
main.o: ../../../common/nativecode/stdlib.h
main.o: ../../../common/nativecode/jjtypes.h /usr/include/sys/types.h
main.o: /usr/include/features.h /usr/include/sys/cdefs.h
main.o: /usr/include/gnu/stubs.h /usr/include/bits/types.h
main.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
main.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
main.o: ../../../common/nativecode/jbheap.h jjconsole.h
main.o: ../../../common/nativecode/jbconsol.h jjmachin.h
main.o: ../../../common/nativecode/jbmachin.h
main.o: ../../../common/nativecode/jbfifo.h jjramdisk.h
main.o: ../../../common/nativecode/jbramdisk.h
main.o: ../../../common/nativecode/jbrefcnt.h ../../../common/decaf/dmain.h
main.o: ../../../common/decaf/jvm.h ../../../common/decaf/classloader.h
main.o: ../../../common/nativecode/jbhash.h
main.o: ../../../common/decaf/javastring.h ../../../common/decaf/d_types.h
main.o: ../../../common/decaf/javaclass.h ../../../common/decaf/typed.h
main.o: ../../../common/decaf/constantpool.h ../../../common/decaf/cpentry.h
main.o: ../../../common/decaf/istream.h ../../../common/decaf/javaword.h
main.o: ../../../common/decaf/javathread.h
main.o: ../../../common/decaf/dynamicstack.h
main.o: ../../../common/decaf/interfacelist.h
main.o: ../../../common/decaf/fieldlist.h
main.o: ../../../common/decaf/attributelist.h
main.o: ../../../common/decaf/methodlist.h ../../../common/decaf/javaobject.h
main.o: ../../../common/nativecode/jbarray.h
main.o: ../../../common/decaf/scheduler.h ../../../common/nativecode/jbsll.h
main.o: ../../../common/decaf/frame.h ../../../common/decaf/staticstack.h
main.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/builtins.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbconsol.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbconsol.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbconsol.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/jbconsol.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/jbconsol.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/jbfifo.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbfifo.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbfifo.o: ../../../common/nativecode/jbfifo.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/jbheap.o: jjmachin.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jbfifo.h
../../../common/nativecode/jbheap.o: /usr/include/sys/types.h
../../../common/nativecode/jbheap.o: /usr/include/features.h
../../../common/nativecode/jbheap.o: /usr/include/sys/cdefs.h
../../../common/nativecode/jbheap.o: /usr/include/gnu/stubs.h
../../../common/nativecode/jbheap.o: /usr/include/bits/types.h
../../../common/nativecode/jbheap.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/nativecode/jbheap.o: /usr/include/time.h
../../../common/nativecode/jbheap.o: /usr/include/endian.h
../../../common/nativecode/jbheap.o: /usr/include/bits/endian.h
../../../common/nativecode/jbheap.o: /usr/include/sys/select.h
../../../common/nativecode/jbheap.o: /usr/include/bits/select.h
../../../common/nativecode/jbheap.o: /usr/include/bits/sigset.h
../../../common/nativecode/jbheap.o: /usr/include/sys/sysmacros.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/jbheap.o: ../../../common/nativecode/gc/heap.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/jbmachin.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/jbramdisk.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbramdisk.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbramdisk.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/jbramdisk.o: ../../../common/nativecode/jbzipfile.h
../../../common/nativecode/jbramdisk.o: ../../../common/nativecode/jbramdisk.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/jbrefcnt.o: ../../../common/nativecode/jbrefcnt.h
../../../common/nativecode/jbtypes.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbtypes.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbzipfile.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/jbzipfile.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/jbzipfile.o: ../../../common/nativecode/jbzipfile.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/stdlib.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/jjtypes.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/jbmachin.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/jbheap.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/jbconsol.h
../../../common/nativecode/stdlib.o: ../../../common/nativecode/jbdebug.h
../../../common/nativecode/stdlib.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stdarg.h
../../../common/nativecode/jbtimer.o: ../../../common/nativecode/jbtimer.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/istream.h
../../../common/decaf/interfacelist.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/interfacelist.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/interfacelist.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/d_types.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/javaclass.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/typed.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/constantpool.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/cpentry.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/javastring.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/javaword.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/javathread.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/classloader.h
../../../common/decaf/interfacelist.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/attributelist.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/methodlist.h
../../../common/decaf/interfacelist.o: ../../../common/decaf/javaobject.h
../../../common/decaf/interfacelist.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/attributelist.o: ../../../common/decaf/attributelist.h
../../../common/decaf/attributelist.o: ../../../common/decaf/d_types.h
../../../common/decaf/attributelist.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/attributelist.o: ../../../common/decaf/istream.h
../../../common/decaf/attributelist.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/attributelist.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/attributelist.o: ../../../common/decaf/javaclass.h
../../../common/decaf/attributelist.o: ../../../common/decaf/typed.h
../../../common/decaf/attributelist.o: ../../../common/decaf/constantpool.h
../../../common/decaf/attributelist.o: ../../../common/decaf/cpentry.h
../../../common/decaf/attributelist.o: ../../../common/decaf/javastring.h
../../../common/decaf/attributelist.o: ../../../common/decaf/javaword.h
../../../common/decaf/attributelist.o: ../../../common/decaf/javathread.h
../../../common/decaf/attributelist.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/attributelist.o: ../../../common/decaf/classloader.h
../../../common/decaf/attributelist.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/attributelist.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/attributelist.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/attributelist.o: ../../../common/decaf/methodlist.h
../../../common/decaf/attributelist.o: ../../../common/decaf/javaobject.h
../../../common/decaf/attributelist.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/attributelist.o: ../../../common/decaf/assert.h
../../../common/decaf/constantpool.o: ../../../common/decaf/constantpool.h
../../../common/decaf/constantpool.o: ../../../common/decaf/cpentry.h
../../../common/decaf/constantpool.o: ../../../common/decaf/javastring.h
../../../common/decaf/constantpool.o: ../../../common/decaf/d_types.h
../../../common/decaf/constantpool.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/constantpool.o: ../../../common/decaf/istream.h
../../../common/decaf/constantpool.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/constantpool.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/constantpool.o: ../../../common/decaf/javaword.h
../../../common/decaf/constantpool.o: ../../../common/decaf/javaclass.h
../../../common/decaf/constantpool.o: ../../../common/decaf/typed.h
../../../common/decaf/constantpool.o: ../../../common/decaf/javathread.h
../../../common/decaf/constantpool.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/constantpool.o: ../../../common/decaf/classloader.h
../../../common/decaf/constantpool.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/constantpool.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/constantpool.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/constantpool.o: ../../../common/decaf/attributelist.h
../../../common/decaf/constantpool.o: ../../../common/decaf/methodlist.h
../../../common/decaf/constantpool.o: ../../../common/decaf/javaobject.h
../../../common/decaf/constantpool.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/classloader.o: ../../../common/decaf/classloader.h
../../../common/decaf/classloader.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/classloader.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/classloader.o: ../../../common/decaf/javastring.h
../../../common/decaf/classloader.o: ../../../common/decaf/d_types.h
../../../common/decaf/classloader.o: ../../../common/decaf/javaclass.h
../../../common/decaf/classloader.o: ../../../common/decaf/typed.h
../../../common/decaf/classloader.o: ../../../common/decaf/constantpool.h
../../../common/decaf/classloader.o: ../../../common/decaf/cpentry.h
../../../common/decaf/classloader.o: ../../../common/decaf/istream.h
../../../common/decaf/classloader.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/classloader.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/classloader.o: ../../../common/decaf/javaword.h
../../../common/decaf/classloader.o: ../../../common/decaf/javathread.h
../../../common/decaf/classloader.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/classloader.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/classloader.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/classloader.o: ../../../common/decaf/attributelist.h
../../../common/decaf/classloader.o: ../../../common/decaf/methodlist.h
../../../common/decaf/classloader.o: ../../../common/decaf/javaobject.h
../../../common/decaf/classloader.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/javastring.o: ../../../common/decaf/javastring.h
../../../common/decaf/javastring.o: ../../../common/decaf/d_types.h
../../../common/decaf/javastring.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/javastring.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/javathread.o: ../../../common/decaf/javathread.h
../../../common/decaf/javathread.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/javathread.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/javathread.o: ../../../common/decaf/d_types.h
../../../common/decaf/javathread.o: ../../../common/decaf/frame.h
../../../common/decaf/javathread.o: ../../../common/decaf/javaword.h
../../../common/decaf/javathread.o: ../../../common/decaf/staticstack.h
../../../common/decaf/javathread.o: ../../../common/decaf/javastring.h
../../../common/decaf/javathread.o: ../../../common/decaf/attributelist.h
../../../common/decaf/javathread.o: ../../../common/decaf/istream.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/javathread.o: ../../../common/decaf/javaclass.h
../../../common/decaf/javathread.o: ../../../common/decaf/typed.h
../../../common/decaf/javathread.o: ../../../common/decaf/constantpool.h
../../../common/decaf/javathread.o: ../../../common/decaf/cpentry.h
../../../common/decaf/javathread.o: ../../../common/decaf/classloader.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/javathread.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/javathread.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/javathread.o: ../../../common/decaf/methodlist.h
../../../common/decaf/javathread.o: ../../../common/decaf/javaobject.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/javathread.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/javathread.o: /usr/include/sys/types.h
../../../common/decaf/javathread.o: /usr/include/features.h
../../../common/decaf/javathread.o: /usr/include/sys/cdefs.h
../../../common/decaf/javathread.o: /usr/include/gnu/stubs.h
../../../common/decaf/javathread.o: /usr/include/bits/types.h
../../../common/decaf/javathread.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/javathread.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/javathread.o: /usr/include/bits/endian.h
../../../common/decaf/javathread.o: /usr/include/sys/select.h
../../../common/decaf/javathread.o: /usr/include/bits/select.h
../../../common/decaf/javathread.o: /usr/include/bits/sigset.h
../../../common/decaf/javathread.o: /usr/include/sys/sysmacros.h
../../../common/decaf/javathread.o: ../../../common/decaf/scheduler.h
../../../common/decaf/javathread.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/javaobject.o: ../../../common/decaf/javaobject.h
../../../common/decaf/javaobject.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/javaobject.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/javaobject.o: ../../../common/decaf/javathread.h
../../../common/decaf/javaobject.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/javaobject.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/javaobject.o: ../../../common/decaf/d_types.h
../../../common/decaf/javaobject.o: ../../../common/decaf/javaclass.h
../../../common/decaf/javaobject.o: ../../../common/decaf/typed.h
../../../common/decaf/javaobject.o: ../../../common/decaf/constantpool.h
../../../common/decaf/javaobject.o: ../../../common/decaf/cpentry.h
../../../common/decaf/javaobject.o: ../../../common/decaf/javastring.h
../../../common/decaf/javaobject.o: ../../../common/decaf/istream.h
../../../common/decaf/javaobject.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/javaobject.o: ../../../common/decaf/javaword.h
../../../common/decaf/javaobject.o: ../../../common/decaf/classloader.h
../../../common/decaf/javaobject.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/javaobject.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/javaobject.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/javaobject.o: ../../../common/decaf/attributelist.h
../../../common/decaf/javaobject.o: ../../../common/decaf/methodlist.h
../../../common/decaf/methodlist.o: ../../../common/decaf/methodlist.h
../../../common/decaf/methodlist.o: ../../../common/decaf/d_types.h
../../../common/decaf/methodlist.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/methodlist.o: ../../../common/decaf/istream.h
../../../common/decaf/methodlist.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/methodlist.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/methodlist.o: ../../../common/decaf/attributelist.h
../../../common/decaf/methodlist.o: ../../../common/decaf/javaclass.h
../../../common/decaf/methodlist.o: ../../../common/decaf/typed.h
../../../common/decaf/methodlist.o: ../../../common/decaf/constantpool.h
../../../common/decaf/methodlist.o: ../../../common/decaf/cpentry.h
../../../common/decaf/methodlist.o: ../../../common/decaf/javastring.h
../../../common/decaf/methodlist.o: ../../../common/decaf/javaword.h
../../../common/decaf/methodlist.o: ../../../common/decaf/javathread.h
../../../common/decaf/methodlist.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/methodlist.o: ../../../common/decaf/classloader.h
../../../common/decaf/methodlist.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/methodlist.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/methodlist.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/methodlist.o: ../../../common/decaf/javaobject.h
../../../common/decaf/methodlist.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/methodlist.o: ../../../common/decaf/assert.h
../../../common/decaf/javaclass.o: ../../../common/decaf/javaclass.h
../../../common/decaf/javaclass.o: ../../../common/decaf/typed.h
../../../common/decaf/javaclass.o: ../../../common/decaf/d_types.h
../../../common/decaf/javaclass.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/javaclass.o: ../../../common/decaf/constantpool.h
../../../common/decaf/javaclass.o: ../../../common/decaf/cpentry.h
../../../common/decaf/javaclass.o: ../../../common/decaf/javastring.h
../../../common/decaf/javaclass.o: ../../../common/decaf/istream.h
../../../common/decaf/javaclass.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/javaclass.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/javaclass.o: ../../../common/decaf/javaword.h
../../../common/decaf/javaclass.o: ../../../common/decaf/javathread.h
../../../common/decaf/javaclass.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/javaclass.o: ../../../common/decaf/classloader.h
../../../common/decaf/javaclass.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/javaclass.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/javaclass.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/javaclass.o: ../../../common/decaf/attributelist.h
../../../common/decaf/javaclass.o: ../../../common/decaf/methodlist.h
../../../common/decaf/javaclass.o: ../../../common/decaf/javaobject.h
../../../common/decaf/javaclass.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/javaclass.o: ../../../common/decaf/assert.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/d_types.h
../../../common/decaf/fieldlist.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/istream.h
../../../common/decaf/fieldlist.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/fieldlist.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/attributelist.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/javaclass.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/typed.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/constantpool.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/cpentry.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/javastring.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/javaword.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/javathread.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/classloader.h
../../../common/decaf/fieldlist.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/methodlist.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/javaobject.h
../../../common/decaf/fieldlist.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/fieldlist.o: ../../../common/decaf/assert.h
../../../common/decaf/scheduler.o: ../../../common/decaf/scheduler.h
../../../common/decaf/scheduler.o: ../../../common/decaf/d_types.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/scheduler.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/scheduler.o: /usr/include/sys/types.h
../../../common/decaf/scheduler.o: /usr/include/features.h
../../../common/decaf/scheduler.o: /usr/include/sys/cdefs.h
../../../common/decaf/scheduler.o: /usr/include/gnu/stubs.h
../../../common/decaf/scheduler.o: /usr/include/bits/types.h
../../../common/decaf/scheduler.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/scheduler.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/scheduler.o: /usr/include/bits/endian.h
../../../common/decaf/scheduler.o: /usr/include/sys/select.h
../../../common/decaf/scheduler.o: /usr/include/bits/select.h
../../../common/decaf/scheduler.o: /usr/include/bits/sigset.h
../../../common/decaf/scheduler.o: /usr/include/sys/sysmacros.h
../../../common/decaf/scheduler.o: ../../../common/decaf/classloader.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/scheduler.o: ../../../common/decaf/javastring.h
../../../common/decaf/scheduler.o: ../../../common/decaf/javaclass.h
../../../common/decaf/scheduler.o: ../../../common/decaf/typed.h
../../../common/decaf/scheduler.o: ../../../common/decaf/constantpool.h
../../../common/decaf/scheduler.o: ../../../common/decaf/cpentry.h
../../../common/decaf/scheduler.o: ../../../common/decaf/istream.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/scheduler.o: ../../../common/decaf/javaword.h
../../../common/decaf/scheduler.o: ../../../common/decaf/javathread.h
../../../common/decaf/scheduler.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/scheduler.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/scheduler.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/scheduler.o: ../../../common/decaf/attributelist.h
../../../common/decaf/scheduler.o: ../../../common/decaf/methodlist.h
../../../common/decaf/scheduler.o: ../../../common/decaf/javaobject.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/scheduler.o: ../../../common/decaf/frame.h
../../../common/decaf/scheduler.o: ../../../common/decaf/staticstack.h
../../../common/decaf/scheduler.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/scheduler.o: ../../../common/decaf/assert.h
../../../common/decaf/javaword.o: ../../../common/decaf/javaword.h
../../../common/decaf/javaword.o: ../../../common/decaf/d_types.h
../../../common/decaf/javaword.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/javaword.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/cpentry.o: ../../../common/decaf/cpentry.h
../../../common/decaf/cpentry.o: ../../../common/decaf/javastring.h
../../../common/decaf/cpentry.o: ../../../common/decaf/d_types.h
../../../common/decaf/cpentry.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/cpentry.o: ../../../common/decaf/istream.h
../../../common/decaf/cpentry.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/cpentry.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/cpentry.o: ../../../common/decaf/javaword.h
../../../common/decaf/cpentry.o: ../../../common/decaf/javaclass.h
../../../common/decaf/cpentry.o: ../../../common/decaf/typed.h
../../../common/decaf/cpentry.o: ../../../common/decaf/constantpool.h
../../../common/decaf/cpentry.o: ../../../common/decaf/javathread.h
../../../common/decaf/cpentry.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/cpentry.o: ../../../common/decaf/classloader.h
../../../common/decaf/cpentry.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/cpentry.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/cpentry.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/cpentry.o: ../../../common/decaf/attributelist.h
../../../common/decaf/cpentry.o: ../../../common/decaf/methodlist.h
../../../common/decaf/cpentry.o: ../../../common/decaf/javaobject.h
../../../common/decaf/cpentry.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/cpentry.o: ../../../common/decaf/assert.h
../../../common/decaf/istream.o: ../../../common/decaf/istream.h
../../../common/decaf/istream.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/istream.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/istream.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/decaf.o: ../../../common/decaf/frame.h
../../../common/decaf/decaf.o: ../../../common/decaf/javaword.h
../../../common/decaf/decaf.o: ../../../common/decaf/d_types.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/decaf.o: ../../../common/decaf/staticstack.h
../../../common/decaf/decaf.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/decaf.o: ../../../common/decaf/javastring.h
../../../common/decaf/decaf.o: ../../../common/decaf/attributelist.h
../../../common/decaf/decaf.o: ../../../common/decaf/istream.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/decaf.o: ../../../common/decaf/javaclass.h
../../../common/decaf/decaf.o: ../../../common/decaf/typed.h
../../../common/decaf/decaf.o: ../../../common/decaf/constantpool.h
../../../common/decaf/decaf.o: ../../../common/decaf/cpentry.h
../../../common/decaf/decaf.o: ../../../common/decaf/javathread.h
../../../common/decaf/decaf.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/decaf.o: ../../../common/decaf/classloader.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/decaf.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/decaf.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/decaf.o: ../../../common/decaf/methodlist.h
../../../common/decaf/decaf.o: ../../../common/decaf/javaobject.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/decaf.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/decaf.o: /usr/include/sys/types.h
../../../common/decaf/decaf.o: /usr/include/features.h
../../../common/decaf/decaf.o: /usr/include/sys/cdefs.h
../../../common/decaf/decaf.o: /usr/include/gnu/stubs.h
../../../common/decaf/decaf.o: /usr/include/bits/types.h
../../../common/decaf/decaf.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/decaf.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/decaf.o: /usr/include/bits/endian.h
../../../common/decaf/decaf.o: /usr/include/sys/select.h
../../../common/decaf/decaf.o: /usr/include/bits/select.h
../../../common/decaf/decaf.o: /usr/include/bits/sigset.h
../../../common/decaf/decaf.o: /usr/include/sys/sysmacros.h
../../../common/decaf/decaf.o: ../../../common/decaf/scheduler.h
../../../common/decaf/decaf.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/decaf.o: ../../../common/decaf/assert.h
../../../common/decaf/frame.o: ../../../common/decaf/frame.h
../../../common/decaf/frame.o: ../../../common/decaf/javaword.h
../../../common/decaf/frame.o: ../../../common/decaf/d_types.h
../../../common/decaf/frame.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/frame.o: ../../../common/decaf/staticstack.h
../../../common/decaf/frame.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/frame.o: ../../../common/decaf/javastring.h
../../../common/decaf/frame.o: ../../../common/decaf/attributelist.h
../../../common/decaf/frame.o: ../../../common/decaf/istream.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/frame.o: ../../../common/decaf/javaclass.h
../../../common/decaf/frame.o: ../../../common/decaf/typed.h
../../../common/decaf/frame.o: ../../../common/decaf/constantpool.h
../../../common/decaf/frame.o: ../../../common/decaf/cpentry.h
../../../common/decaf/frame.o: ../../../common/decaf/javathread.h
../../../common/decaf/frame.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/frame.o: ../../../common/decaf/classloader.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/frame.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/frame.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/frame.o: ../../../common/decaf/methodlist.h
../../../common/decaf/frame.o: ../../../common/decaf/javaobject.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/frame.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/frame.o: /usr/include/sys/types.h
../../../common/decaf/frame.o: /usr/include/features.h
../../../common/decaf/frame.o: /usr/include/sys/cdefs.h
../../../common/decaf/frame.o: /usr/include/gnu/stubs.h
../../../common/decaf/frame.o: /usr/include/bits/types.h
../../../common/decaf/frame.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/frame.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/frame.o: /usr/include/bits/endian.h
../../../common/decaf/frame.o: /usr/include/sys/select.h
../../../common/decaf/frame.o: /usr/include/bits/select.h
../../../common/decaf/frame.o: /usr/include/bits/sigset.h
../../../common/decaf/frame.o: /usr/include/sys/sysmacros.h
../../../common/decaf/frame.o: ../../../common/decaf/scheduler.h
../../../common/decaf/frame.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/dmain.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/dmain.o: /usr/include/sys/types.h
../../../common/decaf/dmain.o: /usr/include/features.h
../../../common/decaf/dmain.o: /usr/include/sys/cdefs.h
../../../common/decaf/dmain.o: /usr/include/gnu/stubs.h
../../../common/decaf/dmain.o: /usr/include/bits/types.h
../../../common/decaf/dmain.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/dmain.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/dmain.o: /usr/include/bits/endian.h
../../../common/decaf/dmain.o: /usr/include/sys/select.h
../../../common/decaf/dmain.o: /usr/include/bits/select.h
../../../common/decaf/dmain.o: /usr/include/bits/sigset.h
../../../common/decaf/dmain.o: /usr/include/sys/sysmacros.h
../../../common/decaf/dmain.o: ../../../common/decaf/classloader.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/dmain.o: ../../../common/decaf/javastring.h
../../../common/decaf/dmain.o: ../../../common/decaf/d_types.h
../../../common/decaf/dmain.o: ../../../common/decaf/javaclass.h
../../../common/decaf/dmain.o: ../../../common/decaf/typed.h
../../../common/decaf/dmain.o: ../../../common/decaf/constantpool.h
../../../common/decaf/dmain.o: ../../../common/decaf/cpentry.h
../../../common/decaf/dmain.o: ../../../common/decaf/istream.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/dmain.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/dmain.o: ../../../common/decaf/javaword.h
../../../common/decaf/dmain.o: ../../../common/decaf/javathread.h
../../../common/decaf/dmain.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/dmain.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/dmain.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/dmain.o: ../../../common/decaf/attributelist.h
../../../common/decaf/dmain.o: ../../../common/decaf/methodlist.h
../../../common/decaf/dmain.o: ../../../common/decaf/javaobject.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/dmain.o: ../../../common/decaf/scheduler.h
../../../common/decaf/dmain.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/dmain.o: ../../../common/decaf/frame.h
../../../common/decaf/dmain.o: ../../../common/decaf/staticstack.h
../../../common/decaf/dmain.o: jjramdisk.h
../../../common/decaf/jvm.o: ../../../common/decaf/jvm.h jjmachin.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbmachin.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jjtypes.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbheap.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbconsol.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbfifo.h
../../../common/decaf/jvm.o: /usr/include/sys/types.h /usr/include/features.h
../../../common/decaf/jvm.o: /usr/include/sys/cdefs.h
../../../common/decaf/jvm.o: /usr/include/gnu/stubs.h
../../../common/decaf/jvm.o: /usr/include/bits/types.h
../../../common/decaf/jvm.o: /usr/lib/gcc-lib/i386-slackware-linux/egcs-2.91.66/include/stddef.h
../../../common/decaf/jvm.o: /usr/include/time.h /usr/include/endian.h
../../../common/decaf/jvm.o: /usr/include/bits/endian.h
../../../common/decaf/jvm.o: /usr/include/sys/select.h
../../../common/decaf/jvm.o: /usr/include/bits/select.h
../../../common/decaf/jvm.o: /usr/include/bits/sigset.h
../../../common/decaf/jvm.o: /usr/include/sys/sysmacros.h
../../../common/decaf/jvm.o: ../../../common/decaf/classloader.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbhash.h
../../../common/decaf/jvm.o: ../../../common/decaf/javastring.h
../../../common/decaf/jvm.o: ../../../common/decaf/d_types.h
../../../common/decaf/jvm.o: ../../../common/decaf/javaclass.h
../../../common/decaf/jvm.o: ../../../common/decaf/typed.h
../../../common/decaf/jvm.o: ../../../common/decaf/constantpool.h
../../../common/decaf/jvm.o: ../../../common/decaf/cpentry.h
../../../common/decaf/jvm.o: ../../../common/decaf/istream.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbramdisk.h
../../../common/decaf/jvm.o: ../../../common/nativecode/stdlib.h
../../../common/decaf/jvm.o: ../../../common/decaf/javaword.h
../../../common/decaf/jvm.o: ../../../common/decaf/javathread.h
../../../common/decaf/jvm.o: ../../../common/decaf/dynamicstack.h
../../../common/decaf/jvm.o: ../../../common/decaf/interfacelist.h
../../../common/decaf/jvm.o: ../../../common/decaf/fieldlist.h
../../../common/decaf/jvm.o: ../../../common/decaf/attributelist.h
../../../common/decaf/jvm.o: ../../../common/decaf/methodlist.h
../../../common/decaf/jvm.o: ../../../common/decaf/javaobject.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbarray.h
../../../common/decaf/jvm.o: ../../../common/decaf/scheduler.h
../../../common/decaf/jvm.o: ../../../common/nativecode/jbsll.h
../../../common/decaf/jvm.o: ../../../common/decaf/frame.h
../../../common/decaf/jvm.o: ../../../common/decaf/staticstack.h
