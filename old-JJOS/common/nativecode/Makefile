COMMON_HEADERS = \
 ../../../common/nativecode/builtins.h \
 ../../../common/nativecode/jbconsole.h \
 ../../../common/nativecode/jbdebug.h \
 ../../../common/nativecode/jbfifo.h \
 ../../../common/nativecode/jbhash.h \
 ../../../common/nativecode/jbheap.h \
 ../../../common/nativecode/jbmachin.h \
 ../../../common/nativecode/jbramdisk.h \
 ../../../common/nativecode/jbrefcnt.h \
 ../../../common/nativecode/jbzipfile.h \
 ../../../common/nativecode/jjtypes.h \
 ../../../common/nativecode/stdlib.h \
 ../../../common/nativecode/jbtimer.h

COMMON_SOURCES = \
 ../../../common/nativecode/builtins.cc \
 ../../../common/nativecode/jbconsol.cc \
 ../../../common/nativecode/jbfifo.cc \
 ../../../common/nativecode/jbheap.cc \
 ../../../common/nativecode/jbmachin.cc \
 ../../../common/nativecode/jbramdisk.cc \
 ../../../common/nativecode/jbrefcnt.cc \
 ../../../common/nativecode/jbtypes.cc \
 ../../../common/nativecode/jbzipfile.cc \
 ../../../common/nativecode/stdlib.cc \
 ../../../common/nativecode/jbtimer.cc

COMMON_OBJECTS = \
 builtins.o \
 jbconsol.o \
 jbfifo.o \
 jbheap.o \
 jbmachin.o \
 jbramdisk.o \
 jbrefcnt.o \
 jbtypes.o \
 jbzipfile.o \
 stdlib.o \
 jbtimer.o

builtins.o:: ../../../common/nativecode/builtins.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbconsol.o:: ../../../common/nativecode/jbconsol.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbfifo.o:: ../../../common/nativecode/jbfifo.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbheap.o:: ../../../common/nativecode/jbheap.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbmachin.o:: ../../../common/nativecode/jbmachin.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbramdisk.o:: ../../../common/nativecode/jbramdisk.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbrefcnt.o:: ../../../common/nativecode/jbrefcnt.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbtypes.o:: ../../../common/nativecode/jbtypes.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbzipfile.o:: ../../../common/nativecode/jbzipfile.cc
	$(CC) $(CFLAGS) -o $@ -c $?

stdlib.o:: ../../../common/nativecode/stdlib.cc
	$(CC) $(CFLAGS) -o $@ -c $?

jbtimer.o:: ../../../common/nativecode/jbtimer.cc
	$(CC) $(CFLAGS) -o $@ -c $?
