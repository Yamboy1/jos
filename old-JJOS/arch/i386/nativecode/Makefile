.KEEP_STATE:

NASM = nasm
NASMOPTIONS = -f elf 

CC = g++
IFLAGS = -I. -I../../../common/nativecode -I../../../common/decaf
# The problem here is if anything aside from
# entry.cc depends on ETHERBOOT or MULTIBOOT
# being defined; this current solution is a hack,
# because it also requires manually undefining
# all the other alternatives.  *sigh*
CFLAGS = \
 -g -Wall -fno-builtin -fno-exceptions \
 -DPARANOID \
 -DHANDLE_CLOCK -DNATIVE_CLOCK \
 -DNEW_INTERRUPT_HANDLING \
 -DTEST_A20 $(IFLAGS)

# -DETHERBOOT \

ETHER_SOURCES = \
 head_ether.s86 \
 jjetherboot.cc
ETHER_HEADERS = \
 jjetherboot.h

GRUB_SOURCES = \
 head_grub.s86 \
 jjmultiboot.cc
GRUB_HEADERS = \
 jjmultiboot.h

ETHER_PRE_OBJECTS = \
 head_ether.o \
 jjidt.o \
 entry.o
ETHER_POST_OBJECTS = \
 jjetherboot.o

GRUB_PRE_OBJECTS = \
 head_grub.o \
 jjidt.o
GRUB_POST_OBJECTS = \
 jjmultiboot.o

ASM_SOURCE = \
 jjidt.s86
ASM_OBJECTS = 

TARGET_HEADERS = \
 jj386int.h \
 jjconsole.h \
 jjeflags.h \
 jjidtent.h \
 jjramdisk.h \
 jjmachin.h \
 jjtimer.h
TARGET_SOURCES = \
 jj386int.cc \
 jjconsol.cc \
 jjeflags.cc \
 jjidtent.cc \
 jjmachin.cc \
 jjramdisk.cc \
 jjfinfo.cc \
 jjvinfo.cc \
 jjtimer.cc

TARGET_OBJECTS = \
 jj386int.o \
 jjconsol.o \
 jjeflags.o \
 jjidtent.o \
 jjmachin.o \
 jjramdisk.o \
 jjfinfo.o \
 jjvinfo.o \
 jjtimer.o

TARGET_SOURCE_FILES = Makefile $(ASMHEADERS) $(ASMSOURCES) $(TARGET_SOURCES) $(TARGET_HEADERS)
ALL_SOURCE_FILES = $(TARGET_SOURCE_FILES) $(COMMON_SOURCES) $(DECAF_SOURCES) $(GC_SOURCES)

OBJCOPY_OPTIONS = -O binary
OBJDUMP_OPTIONS = -b binary --architecture=i386 --disassemble-all 

MKNBI = ../../../tools/mknbi-jos/mknbi
MKNBI_OPTIONS = -k -leom -a "init arg0 arg1" ./$(BINARYIMAGE)

NBIIMAGE = jjos-nbi
BINARYIMAGE = jjos-binary
ELFIMAGE = jjos-elf
GRUBIMAGE = jjos-grub

TARGET_ZIP = ../bytecode/jjos.zip
JAVAC = javac

all: $(GRUBIMAGE) $(TARGET_ZIP)

include ../bytecode/Makefile
include ../../../common/nativecode/Makefile
include ../../../common/decaf/Makefile
include ../../../common/nativecode/gc/Makefile

OBJECTS = $(ASM_OBJECTS) $(TARGET_OBJECTS) $(COMMON_OBJECTS) $(DECAF_OBJECTS) $(GC_OBJECTS)

###########
# ETHERBOOT
ETHER_OBJECTS = $(ETHER_PRE_OBJECTS) $(OBJECTS) $(ETHER_POST_OBJECTS)

$(NBIIMAGE): $(BINARYIMAGE) Makefile
	$(MKNBI) $(MKNBI_OPTIONS) -o /tftpboot/$(NBIIMAGE)

$(BINARYIMAGE): $(ELFIMAGE) Makefile
	objcopy $(OBJCOPY_OPTIONS) $(ELFIMAGE) $(BINARYIMAGE)

$(ELFIMAGE): $(ETHER_OBJECTS) $(TARGET_ZIP) Makefile
	@echo compiling entry.cc ...
	$(CC) $(CFLAGS) -UMULTIBOOT -DETHERBOOT -c -o entry.o entry.cc
	@echo ... done.  building etherboot image...
	ld -Bstatic -m elf_i386 -Ttext 11000 -e _start -o $(ELFIMAGE) $(ETHER_OBJECTS)
	@echo ... done.


###########
# MULTIBOOT

GRUB_OBJECTS = $(GRUB_PRE_OBJECTS) $(OBJECTS) $(GRUB_POST_OBJECTS)

$(GRUBIMAGE): $(GRUB_OBJECTS) entry.cc $(TARGET_ZIP) Makefile
	@echo compiling entry.cc ...
	$(CC) $(CFLAGS) -UETHERBOOT -DMULTIBOOT -c -o entry.o entry.cc	
	@echo ... done.  building grub image...
	ld -Bstatic -m elf_i386 -Ttext 100000 -e _start -o $(GRUBIMAGE) entry.o $(GRUB_OBJECTS)
	@echo ... done.

##############
# MISCELANEOUS

depend:
	makedepend $(IFLAGS) $(ALL_SOURCE_FILES)

native : $(OBJECTS) $(ETHER_PRE_OBJECTS) $(GRUB_PRE_OBJECTS) $(ETHER_POST_OBJECTS) $(GRUB_POST_OBJECTS)

java_clean:
	rm -f $(ALL_JAVA_OBJECTS) $(TARGET_ZIP)

native_clean:
	rm -f $(OBJECTS) $(ETHER_PRE_OBJECTS) $(GRUB_PRE_OBJECTS) $(ETHER_POST_OBJECTS) $(GRUB_POST_OBJECTS)

java : $(TARGET_ZIP)

clean:
	rm -f $(NBIIMAGE) $(BINARYIMAGE) $(ELFIMAGE) $(GRUBIMAGE) $(OBJECTS) $(TARGET_ZIP) $(ALL_JAVA_OBJECTS) $(ETHER_PRE_OBJECTS) $(GRUB_PRE_OBJECTS) $(ETHER_POST_OBJECTS) $(GRUB_POST_OBJECTS)

.SUFFIXES:
.SUFFIXES: .o .s86 .s .cc


.s86.o:
	$(NASM) $(NASMOPTIONS) -o $*.o -l $*.txt $*.s86

.cc.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.cc

.s.cc:
	$(CCOPTIONS) -c -S $*.cc

