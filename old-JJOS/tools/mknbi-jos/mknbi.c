/*
 * mknbi.c  -  MaKe NetBoot Image for Linux
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

#include "common.h"
#ifdef HAVE_INET
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include "mknbi.h"
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#ifndef _MKNBI_H_JOS_
#error Included wrong header file
#endif


int cmdl_msize = CMDLMSIZE;	/* $$$ KLUDGE -jm */

char *progname;
char kname[MAXPATHLEN];		/* Name of kernel image file */
char outname[MAXPATHLEN];	/* Name of output file */
char rdname[MAXPATHLEN];	/* Name of ramdisk image file */
int kimage;			/* File handle for kernel image */
int outfile;			/* File handle for output file */
int rdimage = -1;		/* File handle for ramdisk image */
int rdlocation = -1;		/* Memory location of ramdisk image */
enum {
	rd_auto = RD_AUTO,	/* Ramdisk load mode */
	rd_eom = RD_EOM,
	rd_fixed = RD_FIXED
} rdmode = rd_eom;		/* $$$ -jm */
int verbose = 1;		/* Verbosity level; currently just 0, 1 or 2 */

int cur_rec_num = -1;		/* Number of current load record */
struct load_header header;	/* Load header */
struct load_record *cur_rec;	/* Pointer to current load record */

unsigned char copyrec_buf[SECTSIZE];



/*
 * Write a buffer into the output file and update the load record
 */
static void putrec(recnum, src, size)
int recnum;
char *src;
int size;
{
  unsigned long l;
  size_t isize;
  char *buf;

  if (cur_rec_num != recnum) {
    fprintf(stderr, "%s: Internal error; image chunks mis-ordered!\n",
	    progname);
    exit(EXIT_LINUX_MISORDER);
  }
  isize = ((size / (SECTSIZE + 1)) + 1) * SECTSIZE;
  if ((buf = malloc(isize)) == NULL) {
	perror(progname);
	exit(EXIT_MEMORY);
  }
  memset(buf, 0, isize);
  memcpy(buf, src, size);
  if (write(outfile, buf, isize) != isize) {
	perror(outname);
	exit(EXIT_WRITE);
  }
  free(buf);
  l = get_long(cur_rec->ilength) + isize;
  assign(cur_rec->ilength.low, htot(low_word(l)));
  assign(cur_rec->ilength.high, htot(high_word(l)));
  l = get_long(cur_rec->mlength) + isize;
  assign(cur_rec->mlength.low, htot(low_word(l)));
  assign(cur_rec->mlength.high, htot(high_word(l)));
}


/*
 * Copy a certain number of bytes from the kernel image file into the
 * boot file
 */
static int copyrec(recnum, remaining, image, name)
int recnum;
int remaining;
int image;
char *name;
{
  unsigned int size, bytes_read;
  int i = 1;

  if (cur_rec_num != recnum) {
    fprintf(stderr, "%s: Internal error; image chunks mis-ordered!\n",
	    progname);
    exit(EXIT_LINUX_MISORDER);
  }
  for (bytes_read = 0; remaining > 0 && i > 0;) {
	size = (remaining > SECTSIZE) ? SECTSIZE : remaining;
	if ((i = read(image, copyrec_buf, size)) < 0) {
		perror(name);
		exit(EXIT_READ);
	}
	if (i) {
		putrec(recnum, (char *)copyrec_buf, i);
		bytes_read += i;
		remaining -= i;
	}
  }
  return (bytes_read);
}


/*
 * Initialize a load record
 */
static void initrec(recnum, segment, flags, vendor_size)
int recnum;
int segment;
int flags;
int vendor_size;
{
printf("cur_rec_num = %d\n", cur_rec_num);
printf("initrec(%d, 0x%x, 0x%x, %d)\n", recnum, segment, flags, vendor_size);

  if (++cur_rec_num != recnum) {
	fprintf(stderr, "%s: Internal error; image chunks mis-ordered!\n",
		progname);
	exit(EXIT_LINUX_MISORDER);
  }

  if (cur_rec_num > 0)
	cur_rec = (struct load_record *)((unsigned char *)cur_rec +
					((cur_rec->rlength << 2) & 0x3c) +
					((cur_rec->rlength >> 2) & 0x3c));
  cur_rec->rlength      = (sizeof(struct load_record) -
		           sizeof(union vendor_data)) >> 2;
  cur_rec->rlength     |= ((vendor_size + 3) & 0x3c) << 2;
  cur_rec->rtag1        = recnum + VENDOR_OFF;
  cur_rec->rflags       = flags;
  assign(cur_rec->address.low, htot(low_word((unsigned long) segment << 4)));
  assign(cur_rec->address.high, htot(high_word((unsigned long) segment << 4)));
}


/*
 * Process kernel image file
 */

char *cmdline = DFLT_CMDL;

static void do_kernel()
{
  int flags;
  struct setup_header *setup_header;
  unsigned long l;

  /* Process the kernel */
  flags = rdimage >= 0 ? 0 : FLAG_EOF;

  initrec(KERNELNUM, DEF_SYSSEG, flags, 0);
  while (copyrec(KERNELNUM, SECTSIZE, kimage, kname) == SECTSIZE) ;
  if (get_long(cur_rec->ilength) > SYSLSIZE) {
    fprintf(stderr, "%s: Kernel image too large\n", progname);
    exit(EXIT_LINUX_KERNSIZE);
  }
  assign(cur_rec->mlength.low, htot(low_word(SYSMSIZE)));
  assign(cur_rec->mlength.high, htot(high_word(SYSMSIZE)));

  /* Process the command line */
  initrec(CMDLNUM, DEF_CMDLSEG, 0, 0);
  putrec(CMDLNUM, cmdline, strlen(cmdline) + 1);
printf("cmdline = %s\n", cmdline);
  assign(cur_rec->mlength.low, htot(low_word(cmdl_msize)));
  assign(cur_rec->mlength.high, htot(high_word(cmdl_msize)));

  /* Process the ramdisk image */
  if (rdimage >= 0) {
	unsigned long imagestart = get_long(cur_rec->address) +
						get_long(cur_rec->ilength);

	if (imagestart < 0x100000L)
		imagestart = 0x100000L;
	initrec(RAMDISKNUM, 0, (rdmode == rd_eom ? FLAG_B1 : 0) | FLAG_EOF,
					sizeof(cur_rec->vendor_data.rdflags));
	if (rdmode == rd_auto)
		rdlocation = imagestart;
	else if (rdmode == rd_fixed)
		/* always align to 4kB page boundary */
		rdlocation &= ~0xfff;
	cur_rec->vendor_data.rdflags = (__u8)rdmode;
	while (copyrec(RAMDISKNUM, SECTSIZE, rdimage, rdname) == SECTSIZE) ;
	/* memory length has to be a multiple of 4kB */
	l = (get_long(cur_rec->ilength) + 0xfff) & (unsigned long)~0xfff;
	assign(cur_rec->mlength.low, htot(low_word(l)));
	assign(cur_rec->mlength.high, htot(high_word(l)));
	l = rdmode == rd_eom ? get_long(cur_rec->mlength) : rdlocation;
	assign(cur_rec->address.low, htot(low_word(l)));
	assign(cur_rec->address.high, htot(high_word(l)));
  }
}


/*
 * Remove duplicate and contradictory entries from the commandline
 */
static void cleanup_cmdline(cmdline)
char *cmdline;
{
  static char *entries[] = { " root=", " nfsroot=", " nfsaddrs=", NULL };
  char   **cur_entry;
  char   *ptr1,*ptr2;

  /*
   * This code assumes, that the first entry in the command line is a word
   * followed by a space character. This word must not contain the letter 'r'.
   * This assumptions holds, because the standard command line adds the word
   * "auto " to the beginning of the command line!
   */
  if (strncmp(cmdline,"auto ",5)) {
	fprintf(stderr, "%s: Internal error; command line mangled\n",progname);
	exit(EXIT_INTERNAL);
  }

  for (cur_entry = entries; *cur_entry; cur_entry++) {
	while ((ptr1 = strstr(cmdline, *cur_entry)) != NULL &&
	       strstr(ptr1+1, *cur_entry) != NULL) {
		ptr2 = strchr(ptr1+1, ' ');
		memmove(ptr1+1, ptr2+1, strlen(ptr2));
	}
  }

again: /* Remove multiple occurrences of "ro" and "rw" */
  for (ptr1 = strchr(cmdline,'r'); ptr1 != NULL; ptr1 = strchr(ptr1+1, 'r'))
	if (ptr1[-1] == ' ' && (ptr1[1] == 'o' || ptr1[1] == 'w') && ptr1[2] == ' ')
		for (ptr2 = strchr(ptr1+3,'r'); ptr2 != NULL; ptr2 = strchr(ptr2+1, 'r'))
			if (ptr2[-1] == ' ' &&
			    (ptr2[1] == 'o' || ptr2[1] == 'w') &&
			    (ptr2[2] == ' ' || ptr2[2] == '\000')) {
				memmove(ptr1, ptr1+3, strlen(ptr1+2));
				goto again;
			}
  return;
}


/*
 * Dump the load record information to stderr
 */
static void dump_header(lh)
struct load_header *lh;
{
  static char *s_tags[] = { /* KERNELNUM */   "kernel image",
			    /* CMDLNUM */    "command line",
			    /* RAMDISKNUM */ "ramdisk image"};
  static char *s_flags[]= { "absolute address", "after previous segment",
			    "at end of memory", "before previos segment"};
  static char *s_rd[]   = { /* RD_AUTO */  "RD: auto positioning",
			    /* RD_EOM */   "RD: positioned by Boot-Rom",
			    /* RD_FIXED */ "RD: fixed memory location" };
  struct load_record *lr;
  int    i, num = 0;

  fprintf(stderr,"\n"
	  "Load record information:\n"
	  "  Magic number:     0x%08lX\n"
	  "  Length of header: %d bytes (standard) + %d bytes (vendor)\n"
	  "  Flags:            0x%08lX\n"
	  "  Location address: %04X:%04X\n"
	  "  Execute address:  %04X:%04X\n"
	  "  Vendor data:      %s\n"
	  "\n",
	  get_long(lh->magic),
	  (lh->hlength << 2) & 0x3c,
	  (lh->hlength >> 2) & 0x3c,
	  (unsigned long)lh->hflags1 +
		((unsigned long)lh->hflags2 << 8) +
		((unsigned long)lh->hflags3 << 16),
	  ttoh(getval(lh->locn.segment)), ttoh(getval(lh->locn.offset)),
	  ttoh(getval(lh->execute.segment)), ttoh(getval(lh->execute.offset)),
	  lh->dummy);

  i  = ((lh->hlength >> 2) & 0x3c) + ((lh->hlength << 2) & 0x3c);
  lr = (struct load_record *)&(((__u8 *)lh)[i]);

  for (;;) {
  	fprintf(stderr,
	    "Record #%d:\n"
	    "  Length of header: %d bytes (standard) + %d bytes (vendor)\n"
	    "  Vendor tag:       0x%02X (%s)\n"
	    "  Reserved flags:   0x%02X\n"
	    "  Flags:            0x%02X (%s%s)\n"
	    "  Load address:     0x%08lX%s\n"
	    "  Image length:     0x%08lX bytes\n"
	    "  Memory length:    0x%08lX bytes\n"
	    "  Vendor data:      %s\n"
	    "\n",
	    ++num,
	    (lr->rlength << 2) & 0x3c,
	    (lr->rlength >> 2) & 0x3c,
	    (int)lr->rtag1,
	    lr->rtag1 < 16 || lr->rtag1-16 >= NUM_RECORDS ? "unknown" : s_tags[lr->rtag1-16],
	    (int)lr->rtag2,
	    (int)lr->rflags, s_flags[lr->rflags & 0x03],
	    lr->rflags & FLAG_EOF ? ", last record" : "",
	    get_long(lr->address),
	    get_long(lr->address) >= 0x100000L ? " (high memory)" : "",
	    get_long(lr->ilength),
	    get_long(lr->mlength),
	    lr->rlength & 0xf0 ? lr->rtag1-16 == RAMDISKNUM ?
	    lr->vendor_data.rdflags < 3 ? s_rd[lr->vendor_data.rdflags] :
	    "illegal" : "unknown" : "none");

	if (lr->rflags & FLAG_EOF)
		break;

	i  = ((lr->rlength >> 2) & 0x3c) + ((lr->rlength << 2) & 0x3c);
	lr = (struct load_record *)&(((__u8 *)lr)[i]);
  }
}


/*
 * Print the usage
 */
static void usage()
{
  fprintf(stderr,
#ifdef HAVE_INET
	"Usage: %s [-a <append>] [-d rom|ram|kernel|<dir>]\n"
	"             [-i rom|kernel|<client:server:gateway:netmask:hostname>]\n"
#else
	"Usage: %s [-a <append>] [-d rom|ram|kernel|<dir>] [-i rom|kernel]\n"
#endif
	"             [-l auto|eom|<addr>] [-r <ramdisk>] [-s <max cmdline size]\n"
	"             [-x] [-k] <kernel> [-o] <outfile>\n"
	"       %s [-h]\n"
	"       %s [-v]\n",
	  progname, progname, progname);
  exit(EXIT_USAGE);
}


/*
 * Main program
 */

void main(argc, argv)
int argc;
char **argv;
{
#ifdef NOTDEF
  char *cmdline = NULL;		/* Command line to pass to the kernel */
#endif
  char *append = NULL;		/* String to append to end of command line */
  char *addrs = NULL;		/* String containing various addresses */
  char *cp, *ip;
  size_t len;
  int vendor_size;
  int i;

  /* Determine my own name for error output */
  if ((cp = strrchr(argv[0], '/')) == NULL)
	progname = argv[0];
  else
	progname = ++cp;

  /* Initialize option argments */
#ifdef NOTDEF
  strcpy(nfsdir, DFLT_DIR);
  strcpy(rootdev, DFLT_DEV);
  strcpy(kname, DFLT_IMAGE);
#endif
  strcpy(outname, "");
  strcpy(rdname, "../bytecode/jjos.zip");

  /* Parse options */
  opterr = 0;
  while ((i = getopt(argc, argv, "a:d:hi:k:l:o:r:s:vx")) != EOF)
	switch (i) {
		case 'a': if ((append = strdup(optarg)) == NULL) {
		          	perror(progname);
		          	exit(EXIT_MEMORY);
		          }
		          break;
		case 'i': if ((addrs = strdup(optarg)) == NULL) {
				perror(progname);
				exit(EXIT_MEMORY);
		          }
		          break;
		case 'k': strncpy(kname, optarg, MAXPATHLEN-1);
		          kname[MAXPATHLEN-1] = '\0';
		          break;
		case 'l': if (!strcmp("auto",optarg))
				rdmode = rd_auto;
		          else if (!strcmp("eom",optarg))
				rdmode = rd_eom;
		          else {
				rdmode = rd_fixed;
				rdlocation = strtol(optarg, NULL, 0); }
		          break;
		case 'o': strncpy(outname, optarg, MAXPATHLEN-1);
		          outname[MAXPATHLEN-1] = '\0';
		          break;
		case 'r': strncpy(rdname, optarg, MAXPATHLEN-1);
		          rdname[MAXPATHLEN-1] = '\0';
		          break;
		case 's': if ((i = atoi(optarg)) < CMDLMSIZE) {
				fprintf(stderr, "%s: command line size "
				                "must be > %d bytes\n",
				        progname, CMDLMSIZE-1);
				exit(EXIT_LINUX_CMDLSIZE);
		          }
		          cmdl_msize = i;
			  break;
		case 'v': fprintf(stderr, VERSION"\n");
		          exit(EXIT_SUCCESS);
		case 'x': verbose++;
		          break;
		case 'h': /* fall thru */
		default:  usage();
	}

  /* Parse additional arguments */
  if (optind < argc) {
	strncpy(kname, argv[optind++], MAXPATHLEN-1);
	kname[MAXPATHLEN-1] = '\0';
  }
  if (optind < argc) {
	strncpy(outname, argv[optind++], MAXPATHLEN-1);
	outname[MAXPATHLEN-1] = '\0';
  }
  if (optind != argc || !*kname || !*outname)
	usage();

  /* Parse the IP address option */
#ifdef HAVE_INET
  if (addrs != NULL && *addrs && strcmp(addrs, "rom") &&
      strcmp(addrs, "kernel")) {
	struct hostent *hp;
	char *buf, *bp;

	/* Allocate memory for 5 address strings, each with max 15 chars */
	if ((buf = malloc((MAX_ADDR_SIZE + 1) * 5)) == NULL) {
		perror(progname);
		exit(EXIT_MEMORY);
	}
	i = 0;
	ip = addrs;
	bp = buf;
	while (ip != NULL && *ip) {
		if ((cp = strchr(ip, ':')) != NULL) *cp++ = '\0';
		if (strlen(ip) > 0) {
			if ((hp = gethostbyname(ip)) == NULL) {
				fprintf(stderr, "%s: invalid hostname %s\n",
					progname, ip);
				exit(EXIT_HOSTNAME);
			}
			if (hp->h_length != sizeof(struct in_addr)) {
				fprintf(stderr, "%s: invalid host address "
					"type\n", progname);
				exit(EXIT_HOSTADDR);
			}
			/* Result from ntoa is smaller than MAX_ADDR_SIZE */
			strcpy(bp, inet_ntoa(*(struct in_addr *) hp->h_addr));
			bp += strlen(bp);
		}
		ip = cp;
		if (i < 3) *bp++ = ':';
		if (i >= 3) break;
		i++;
	}
	for (; i < 4; i++)
		*bp++ = ':';
	/* Finally copy host name */
	if (ip != NULL) {
		if ((i = strlen(ip)) > MAX_ADDR_SIZE)
			i = MAX_ADDR_SIZE;
		strncpy(bp, ip, i);
		bp += i;
	}
	*bp = '\0';
	free(addrs);
	addrs = buf;
  }
#else
  if (addrs != NULL && *addrs && strcmp(addrs, "rom") &&
      strcmp(addrs, "kernel")) {
	fprintf(stderr, "%s: No INET support for -i option\n", progname);
	exit(EXIT_INET);
  }
#endif

  /* Verify validity of ramdisk load information */
  if (rdmode == rd_fixed && rdlocation < 0x100000L) {
	fprintf(stderr, "%s: Ramdisk image load location must be above 1M\n",
								progname);
	exit(EXIT_LINUX_RDLOC);
  }

  /* Open the input and output files */
  if ((kimage = open(kname, O_RDONLY)) < 0) {
	perror(kname);
	exit(EXIT_LINUX_KERNOPEN);
  }
  if ((outfile = creat(outname, 0644)) < 0) {
	perror(outname);
	exit(EXIT_LINUX_IMGCREATE);
  }
  if (*rdname && (rdimage = open(rdname, O_RDONLY)) < 0) {
	perror(rdname);
	exit(EXIT_LINUX_RDOPEN);
  }
  if (verbose > 0) {
	fprintf(stderr, "Kernel image file name  = %s\n", kname);
	fprintf(stderr, "Output file name        = %s\n", outname);
	if (*rdname)
		fprintf(stderr, "Ramdisk image file name = %s\n", rdname);
  }

  /* Construct command line to pass to the kernel */
  len = strlen(DFLT_CMDL) + 1;
  if (append != NULL)
	len += strlen(append) + 1;
  
  cmdline = malloc(len);
  sprintf(cmdline, "%s %s", DFLT_CMDL, append);

  if (verbose > 0)
	fprintf(stderr, "Kernel command line     = \"%s\"\n", cmdline);

  /* Initialize the boot header */
  vendor_size = (sizeof(VENDOR_ID) / sizeof(__u32) + 1) * sizeof(__u32);
  memset(&header, 0, sizeof(header));
  assign(header.magic.low,       htot(low_word(HEADER_MAGIC)));
  assign(header.magic.high,      htot(high_word(HEADER_MAGIC)));
  assign(header.locn.segment,    htot(DEF_HEADERSEG));
  assign(header.locn.offset,     htot(0));
#ifdef NOTDEF
  assign(header.execute.segment, htot(DEF_BOOTLSEG));
#else
  assign(header.execute.segment, htot(DEF_SYSSEG));
#endif
  assign(header.execute.offset,  htot(0));
  assign(header.bootsig,         htot(BOOT_SIGNATURE));
  header.hlength         = (__u8)(((int)header.dummy - (int)&header)
                           / sizeof(__u32)) & 0x0f;
  header.hlength        |= (__u8)((vendor_size/sizeof(__u32)) << 4) & 0xf0;
  memcpy(header.dummy, VENDOR_ID, sizeof(VENDOR_ID));
  if (write(outfile, &header, sizeof(header)) < 0) {
	perror(outname);
	exit(EXIT_WRITE);
  }

  /* Initialize pointer to first load record */
  cur_rec = (struct load_record *)&(header.dummy[vendor_size]);

  /* Process the boot loader record */
  if (first_data_size > BOOTLLSIZE) {
	fprintf(stderr, "%s: Boot loader too large\n", progname);
	exit(EXIT_LINUX_BOOTLSIZE);
  }


  /* Process the kernel image */
  do_kernel();

  /* After writing out all these stuff, finally update the boot header */
  if (lseek(outfile, 0, 0) != 0) {
	perror(outname);
	exit(EXIT_SEEK);
  }
  if (write(outfile, &header, sizeof(header)) < 0) {
	perror(outname);
	exit(EXIT_WRITE);
  }

  /* If user asked for detailed output, parse the header and output all of */
  /* the load record information */
  if (verbose > 1)
	dump_header(&header);

  exit(EXIT_SUCCESS);
}
