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

#include "stdlib.h"
#include "jbdebug.h"
#include "jbzipfile.h"
#include "jbramdisk.h"

static int debug = 0;

/*
 * Open files.
 */

jbRamDiskStream::jbRamDiskStream(jju8 *first_byte, jju8 *last_byte)
{
  myFirstByte = first_byte;
  myCurrentByte = myFirstByte;
  myLastByte = last_byte;
}

int jbRamDiskStream::get(void)
{
  if (myCurrentByte >= myLastByte)
    {
      return(JJEOF);
    }
  else
    {
      // was a jju32 pointer.
      jji32 retval = (jju32)*myCurrentByte++;

      return(retval);
    }
}

jjBoolean jbRamDiskStream::readjju8(jju8 &u8)
{
  jji32 c = get();

  if (c == JJEOF)
    {
      return(JJFALSE);
    }
  else
    {
      u8 = c;
      return(JJTRUE);
    }
}

jjBoolean jbRamDiskStream::readjju16(jju16 &u16)
{
  jju8 lo_byte;
  jju8 hi_byte;

  /* Read in big-endian (Java) order. */

  if (readjju8(hi_byte) && readjju8(lo_byte))
    {
      u16 = hi_byte & 0xff;
      u16 = u16 << 8;
      u16 |= lo_byte;
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}
      
jjBoolean jbRamDiskStream::readjju32(jju32 &u32)
{
  jju16 hi_word;
  jju16 lo_word;

  /* Read in big-endian (Java) order. */

  if (readjju16(hi_word) && readjju16(lo_word))
    {
      u32 = hi_word << 16;
      u32 |= lo_word;
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}


/*
 * The ramdisk can contain only one file.
 */

jbRamDisk::jbRamDisk()
{
  myFirstByte = NULL;
  myLastByte = NULL;
}

void jbRamDisk::dump(void)
{
  jju32 num_bytes_in_ramdisk = (myLastByte-myFirstByte);

  kprintf("jbRamDisk::dump -- size = %D\n", num_bytes_in_ramdisk);

  jju8 *p = myFirstByte;

  do
    {
      jbZipFileLocalFileHeader *zipfilehdr = (jbZipFileLocalFileHeader *)p;

      if (zipfilehdr->isValidZipFile())
	{
	  char filename[1024];
	  zipfilehdr->getMyFileName(filename);
	  kprintf("   %s\n", filename);
	}
      else
	{
	  return;
	}
      
      p += zipfilehdr->totalSizeIncludingHeader();
    }
    while (p < myLastByte);
}

jbRamDiskStream *jbRamDisk::openStream(char *to_be_opened)
{
  jju8 *p = myFirstByte;

  do
    {
      jbZipFileLocalFileHeader *zipfilehdr = (jbZipFileLocalFileHeader *)p;

      if (!zipfilehdr->isValidZipFile())
	{
	  // kprintf("jbRamDisk::openStream -- failed to find \"%s\"\n", to_be_opened);
	  DEBUG(("jbRamDisk::openStream -- failed to find \"%s\"\n", to_be_opened));
	  return(NULL);
	}
      
      char filename[1024];
      zipfilehdr->getMyFileName(filename);

      DEBUG(("jbRamDisk::openStream -- looking at \"%s\" -- ", filename));

      if (strcmp(to_be_opened, filename) == 0)
	{
	  DEBUG(("jbRamDisk::openStream(%s) -- found it!\n", filename));

	  jju8 *file_first_byte = zipfilehdr->firstByteOfDataPointer();
	  jju8 *file_last_byte = zipfilehdr->lastByteOfDataPointer();

	  jbRamDiskStream *retval = new jbRamDiskStream(file_first_byte, file_last_byte);
	  return(retval);
	}
      else
	{
	  DEBUG((" NOPE\n"));
	}

      p += zipfilehdr->totalSizeIncludingHeader();
    }
  while (p < myLastByte);

  return(NULL);
}

/*
 * Test scaffolding.
 */

void jbRamDisk::test(void)
{
  dump();

  char *test_filename = "a.class";

  jbRamDiskStream *s = openStream(test_filename);

  if (s == NULL)
    {
      kprintf("jbRamDisk::test -- failed to open file %s\n", test_filename);
      return;
    }
  else
    {
      for (int i=0; ; i++)
	{
	  jji32 c = s->get();
	  
	  if (c == JJEOF)
	    {
	      if (i == 20000)	// Hard-coded a.class/b.class char counts. 
		{
		  kprintf("jbRamDisk::test() -- PASSED!\n");
		}
	      else
		{
		  kprintf("jbRamDisk::test() -- FAILED: %d chars\n", i);
		}
	      return;
	    }
	  else if (c != 'a')
	    {
	      kprintf("NOT 'a' 0x%x=%c\n", c, c);
	      return;
	    }
	  else
	    {
#ifdef NOTDEF
	      kprintf("%d = %d\n", i, c);
#endif
	    }
	}

    }
}

