ifeq ("$(GC_DIR)","")
	GC_DIR=.
endif

GC_SOURCES = \
 $(GC_DIR)/freelist.c \
 $(GC_DIR)/heap.c \
 $(GC_DIR)/jassert.c \
 $(GC_DIR)/markstack.c \
 $(GC_DIR)/page.c \
 $(GC_DIR)/pager.c

GC_HEADERS = $(GC_SOURCES:.c=.h)

GC_OBJECTS = \
	freelist.o \
	heap.o \
	jassert.o \
	markstack.o \
	page.o \
	pager.o

freelist.o:: ../../../common/nativecode/gc/freelist.c
	$(CC) $(CFLAGS) -o $@ -c $?

heap.o:: ../../../common/nativecode/gc/heap.c
	$(CC) $(CFLAGS) -o $@ -c $?

jassert.o:: ../../../common/nativecode/gc/jassert.c
	$(CC) $(CFLAGS) -o $@ -c $?

markstack.o:: ../../../common/nativecode/gc/markstack.c
	$(CC) $(CFLAGS) -o $@ -c $?

page.o:: ../../../common/nativecode/gc/page.c
	$(CC) $(CFLAGS) -o $@ -c $?

pager.o:: ../../../common/nativecode/gc/pager.c
	$(CC) $(CFLAGS) -o $@ -c $?

