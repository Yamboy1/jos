/*
 * The contents of this file are subject to the JJOS Public License
 * Version 0.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License
 * at http://www.jos.org/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is JJOS code, released October, 1998.
 *
 * The Initial Developer of the Original Code is John Morrison.
 * Portions created by John Morrison are Copyright (C) 1998.
 * All rights reserved.
 *
 * Contributor(s):
 *
 */

/* #include <stdlib.h> doesn't seem to get rid of errors about malloc() */
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#ifdef _sun
	#include <sys/fcntlcom.h>
#else
	/* #include <fcntlbits.h> */
	#include <fcntl.h>
#endif

#include "jjramdisk.h"

jjRamDisk::jjRamDisk(char *path, int flags, int mode)
  : jbRamDisk()
{
  myFD = -1;			// This ought to attract attention!

  if ((myFD = ::open(path, O_RDONLY)) < 0)
    {
      perror("failed to open ramdisk file");
      fprintf(stderr, " filename = \"%s\"\n", path);
      abort();
    }
    
  struct stat buf;
  if (fstat(myFD, &buf) < 0)
    {
      perror("failed to stat ramdisk file");
      fprintf(stderr, " filename = \"%s\"\n", path);
      abort();
    }
    
  char *ram_disk_first_byte = (char *)malloc(buf.st_size);
  // char *ram_disk_last_byte = ram_disk_first_byte + (buf.st_size);

  caddr_t rdb0 = mmap(ram_disk_first_byte,
		      buf.st_size,
		      PROT_READ,
		      MAP_PRIVATE,
		      myFD,
		      0);

  if ((int)rdb0 == -1)
    {
      perror("failed to mmap ramdisk file; aborting.\n");
#ifdef NOTDEF
      exit(0);
#else
      abort();
#endif
    }
  else
    {
      myFirstByte = (jju8*)rdb0;
#ifdef NOTDEF
      myCurrentPointer = myFirstByte;
#endif
      myLastByte = myFirstByte + buf.st_size;
    }
}
    

