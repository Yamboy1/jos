#ifndef JBZIPILE_H
#define JBZIPILE_H

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

#include "jjtypes.h"

/*
 * Zip file header.
 * This will only work on little-endian machines.
 */

class jbZipFileLocalFileHeader
{
public:

  void dump(void);
  jju32 CRC32(void);
  jju32 compressedSize(void);
  jju32 uncompressedSize(void);
  jju32 totalSizeIncludingHeader(void);
  void getMyFileName(char *buf);
  void getMyExtraField(char *buf);

  jju8 *firstByteOfDataPointer(void);
  jju8 *lastByteOfDataPointer(void);
  jju32 headerSize(void);
  jjBoolean isValidZipFile(void);

  /* Member variables. */

  jju32 myLocalFileHeaderSignature;
  jju16 myVersionNeededToExtract;
  jju16 myGeneralPurposeBitFlag;
  jju16 myCompressionMethod;
  jju16 myLastModFileTime;
  jju16 myLastModFileDate;
  /* Alignment problems -- can't use jju32 -- somebody should'a seen this comin'. */
  jju16 myCRC32Lo16;
  jju16 myCRC32Hi16;
  jju16 myCompressedSizeLo16;
  jju16 myCompressedSizeHi16;
  jju16 myUncompressedSizeLo16;
  jju16 myUncompressedSizeHi16;
  jju16 myFilenameLength;
  jju16 myExtraFieldLength;

  /* Zero-length is a kludge, but... */
  char myFilename[0];
  char myExtraField[0];
};

#endif
