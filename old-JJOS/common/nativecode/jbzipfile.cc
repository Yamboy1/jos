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
#include "jbzipfile.h"

jjBoolean jbZipFileLocalFileHeader::isValidZipFile(void)
{
  if (myLocalFileHeaderSignature == 0x04034b50)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}
      

jju32 jbZipFileLocalFileHeader::headerSize(void)
{
  jju32 header_size = 0;

  header_size += 30;			// Can't use sizeof(*this) due to padding.
  header_size += myFilenameLength;
  header_size += myExtraFieldLength;
  return(header_size);
}
  

jju32 jbZipFileLocalFileHeader::totalSizeIncludingHeader(void)
{
  jju32 sum = 0;

  sum += headerSize();
  sum += compressedSize();	// Even though we don't support compression.
  return(sum);
}

jju8 *jbZipFileLocalFileHeader::firstByteOfDataPointer(void)
{
  jju8 *p = (jju8 *)this;
  p += headerSize();
  return(p);
}

jju8 *jbZipFileLocalFileHeader::lastByteOfDataPointer(void)
{
  jju8 *p = firstByteOfDataPointer();
  p += compressedSize();
  return(p);
}



jju32 jbZipFileLocalFileHeader::CRC32(void)
{
  return((myCRC32Hi16 << 16) | myCRC32Lo16);
}

jju32 jbZipFileLocalFileHeader::compressedSize(void)
{
  return((myCompressedSizeHi16 << 16) | myCompressedSizeLo16);
}

jju32 jbZipFileLocalFileHeader::uncompressedSize(void)
{
  return((myUncompressedSizeHi16 << 16) | myUncompressedSizeLo16);
}

void jbZipFileLocalFileHeader::getMyFileName(char *buf)
{
  char *p = myFilename;

  int i;

  for (i=0; i<myFilenameLength; i++)
    {
      buf[i] = *p++;
    }

  buf[i] = '\0';
}
      
void jbZipFileLocalFileHeader::getMyExtraField(char *buf)
{
  char *p = myFilename;
  p += myFilenameLength;

  int i;

  for (i=0; i<myExtraFieldLength; i++)
    {
      buf[i] = *p++;
    }

  buf[i] = '\0';
}
    

void jbZipFileLocalFileHeader::dump(void)
{
  if (myLocalFileHeaderSignature != 0x04034b50)
    {
      kprintf("Bad myLocalFileHeaderSignature %X\n", myLocalFileHeaderSignature);
    }
  else
    {
      kprintf("myLocalFileHeaderSignature = %x\n", myLocalFileHeaderSignature);
      kprintf("myVersionNeededToExtract = %x\n", myVersionNeededToExtract);
      kprintf("myGeneralPurposeBitFlag = %x\n", myGeneralPurposeBitFlag);
      kprintf("myCompressionMethod = %x\n", myCompressionMethod);
      kprintf("myLastModFileTime = %x\n", myLastModFileTime);
      kprintf("myLastModFileDate = %x\n", myLastModFileDate);
      kprintf("myCRC32 = %X\n", CRC32());
      kprintf("myCompressedSize = %X = %D\n", compressedSize(), compressedSize());
      kprintf("myUncompressedSize = %X = %D\n", uncompressedSize(), uncompressedSize());
      kprintf("myFilenameLength = %x\n", myFilenameLength);
      kprintf("myExtraFieldLength = %x\n", myExtraFieldLength);

      char filename[1024];
      getMyFileName(filename);
      kprintf("myFilename = \"%s\"\n", filename);

      char extra_field[1024];
      getMyExtraField(extra_field);
      kprintf("myExtraField = \"%s\"\n", extra_field);

      kprintf("totalSizeIncludingHeader = %D\n", totalSizeIncludingHeader());
    }
};

