/*
 * mknbi.h  -  mknbi constants and type definitions
 *
 * Copyright (C) 1995,1998 Gero Kuhlmann   <gero@gkminix.han.de>
 * Copyright (C) 1996,1997 Gero Kuhlmann   <gero@gkminix.han.de>
 *                and Markus Gutschke <gutschk@math.uni-muenster.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _MKNBI_H_JOS_
#define _MKNBI_H_JOS_


/*
 * Include version information
 */
#include "version.h"


#ifdef NOTDEF
/*
 * Get some Linux values if header file exists. If not, we assume default
 * values.
 */
#ifdef HAVE_LINUX_CONFIG_H
#include <linux/config.h>
#endif
#endif


/*
 * Everything in this file has to be in packed structures.
 */
#ifdef USE_PRAGMA_PACK
#pragma pack(1)
#endif
#ifdef USE_PRAGMA_ALIGN
#pragma options align=packed
#endif



/*
 * The RAM looks like this after the boot image has been loaded by the
 * Boot-ROM:
 *
 *  0x10000-0x8FFFF	512kB	kernel and part of kernel setup
 *  0x90000-0x901FF	0.5kB	linux floppy boot sector
 *  0x90200-0x911FF	  8kB	first sectors of kernel setup
 *  0x92200-0x931FF	  4kB	primary boot loader
 *  0x93200-0x933FF	0.5kB	load header
 *  0x93400-0x93BFF	  2kB	default command line
 */
#ifndef DEF_SYSSEG		/* usually defined in linux/config.h */
#  define DEF_SYSSEG	0x1100
#endif
#define SYSLSIZE	524288	/* maximum load size of kernel */
#define SYSMSIZE	524288	/* maximum memory size of kernel */

#define BIG_SYSSEG	0x10000 /* load segment for big kernels */
#define BIG_SYSLSIZE	0xF00000/* big kernels can grow up to 15MB in size */
#define BIG_SYSMSIZE	0xF00000

#ifdef NOTDEF
#ifndef DEF_INITSEG		/* usually defined in linux/config.h */
#  define DEF_INITSEG	0x9000
#endif
#endif
#define INITLSIZE	512	/* maximum load size of floppy boot sector */
#define INITMSIZE	512	/* maximum memory size of floppy boot sector */

#ifdef NOTDEF
#ifndef DEF_SETUPSEG		/* usually defined in linux/config.h */
#  define DEF_SETUPSEG	0x9020
#endif
#endif
#define SETUPLSIZE	8192	/* maximum load size of kernel setup */
#define SETUPMSIZE	40448	/* maximum memory size of kernel setup */

#ifdef NOTDEF
#ifndef DEF_BOOTLSEG		/* this is NOT defined in linux/config.h */
#  define DEF_BOOTLSEG	0x9220
#endif
#endif
#define BOOTLLSIZE	4096	/* maximum load size of primary boot loader */
#define BOOTLMSIZE	4096	/* maximum memory size of primary boot loader */

#ifndef DEF_CMDLSEG		/* this is NOT defined in linux/config.h */
#  define DEF_CMDLSEG	0x1000
#endif
#define CMDLLSIZE	512	/* maximum load size of default command line */
#define CMDLMSIZE	512	/* maximum memory size of def. command line -jm */

#ifdef NOTDEF
#ifndef DEF_HEADERSEG		/* this is NOT defined in linux/config.h */
#  define DEF_HEADERSEG	0x9320
#endif
#else
#define DEF_HEADERSEG 0xfe0
#endif
#define HEADERLSIZE	512	/* maximum load size of boot image header */
#define HEADERMSIZE	512	/* maximum memory size of boot image header */



/*
 * Default values for command line etc.
 */
#define SECTSIZE	512			/* Size of one sector */

#define BOOT_SIGNATURE	0xaa55			/* boot signature */
#define BOOT_SIG_OFF	510			/* boot signature offset */
#define SETUP_SIZE_OFF	497			/* setup size offset */

#define MAX_ADDR_SIZE	15			/* Max size of addr strings */

#define DFLT_CMDL	""

/*
 * The boot image has the following header in it's first sector
 */
struct load_header {
  struct i_long  magic		PACKED;		/* magic number */
  __u8           hlength	PACKED;		/* length of header */
  __u8           hflags1	PACKED;		/* header flags */
  __u8           hflags2	PACKED;
  __u8           hflags3	PACKED;
  struct i_addr  locn		PACKED;		/* location of this header */
  struct i_addr  execute	PACKED;		/* execution address */
  __u8           dummy[494];			/* up to full sector */
  __u16          bootsig	PACKED;		/* boot signature */
};

#define HEADER_MAGIC	0x1B031336		/* Magic no for load header */



/*
 * In the header sector are the load records located, which actually tell
 * which part of the following image has to go where in memory
 */

union vendor_data {
  __u8           rdflags	PACKED;		/* flags for rd loading */
};

struct load_record {
  __u8              rlength	PACKED;		/* length of record */
  __u8              rtag1	PACKED;		/* record tags */
  __u8              rtag2	PACKED;
  __u8              rflags	PACKED;		/* record flags */
  struct i_long     address	PACKED;		/* abs addr for part in mem */
  struct i_long     ilength	PACKED;		/* len of part in boot image */
  struct i_long     mlength	PACKED;		/* memory needed for part */
  union vendor_data vendor_data	PACKED;		/* optional vendor data */
};

#define FLAG_B0		1
#define FLAG_B1		2
#define FLAG_EOF	4



/*
 * Number of each load record. Don't change the loading order of the
 * modules by changing these numbers, because the whole kernel gets
 * loaded in one big chunk, while the records really split the kernel
 * image in memory. These values have to be identical with those in
 * the include file of the boot loader assembler module.
 *
 * These numbers must be in the same order that the respective "initrec"
 * calls happen!
 */

#define KERNELNUM	0		/* kernel image */
#define CMDLNUM		1		/* command line */
#define RAMDISKNUM	2		/* ramdisk image */

#define NUM_RECORDS	3

#define VENDOR_ID	"GK-mknbi-jos"	/* Vendor ID */
#define VENDOR_OFF	16			/* Offset for vendor tags */



/*
 * We need to take a look at the kernel setup header in order to determine
 * whether the kernel image needs to be loaded into high memory.
 */

struct setup_header {
  __u8          jump[2]			PACKED;
  __u8          magic[4]		PACKED;	/* "HdrS" */
  __u16         version			PACKED;	/* >= 0x0201 for initrd */
  __u8          realmode_swtch[4]	PACKED;
  __u16         start_sys_seg		PACKED;
  __u16         kernel_version		PACKED;
  /* note: above part of header is compatible with loadlin-1.5 (header v1.5),*/
  /*       must not change it */
  __u8          type_of_loader		PACKED;
  __u8          loadflags		PACKED;
  __u16         setup_move_size		PACKED;
  struct i_long code32_start		PACKED;
  struct i_long ramdisk_image		PACKED;
  struct i_long ramdisk_size		PACKED;
  struct i_long bootsect_kludge		PACKED;
  __u16         heap_end_ptr		PACKED;
};

#define SETUP_MAGIC	"HdrS"
#define SETUP_VERSION	0x0201
#define SETUP_LILO	0x00
#define SETUP_LOADLIN	0x10
#define SETUP_BOOTSECT	0x20
#define SETUP_SYSLX	0x30
#define SETUP_ETHERBOOT	0x40
#define SETUP_HIGH	0x01
#define SETUP_USEHEAP	0x80



/*
 * Ramdisk images can be loaded in three different ways:
 *  RD_AUTO  --  The ramdisk image is loaded at either 0x100000 or right
 *               behind the kernel image; at run-time it will be moved
 *               to the end of physical memory or right before 0x1000000.
 *               This technique guarantees maximum compatibility with
 *               broken BIOS versions without making assumptions on the
 *               way, the BOOT-Prom determined the end of memory. The
 *               drawback is, that moving the image costs some extra time.
 *  RD_EOM   --  The ramdisk image is loaded right before the end of
 *               physical memory. It is the BOOT-Proms responsibility to do
 *               this reliably. This might cause problems with some ancient
 *               BIOS versions, if the computer is equipped with more then
 *               16M of main memory.
 *  RD_FIXED --  The user provided a fixed load address for the ramdisk
 *               image. This entails possible compatibility restrictions
 *               when moving the image from one client to another. OTOH,
 *               it could be the final solution if everything else fails...
 */

#define RD_AUTO		0
#define RD_EOM		1
#define RD_FIXED	2



/*
 * Turnoff structure packing and return to normal data alignment.
 */
#ifdef USE_PRAGMA
#pragma pack()
#endif
#ifdef USE_PRAGMA_ALIGN
#pragma options align=reset
#endif



/*
 * Data area which contains the boot loader module
 */
extern unsigned char first_data[];
extern unsigned int first_data_size;


#endif
