/*
 * istream.h
 *
 * the input-stream workalike
 * for the decaf JVM.
 */

#ifndef decaf_istream
#define decaf_istream

#include "jbramdisk.h"

class istream : public jbRamDiskStream {
        public:
                istream( jbRamDiskStream* rds );

                friend istream& operator >> ( istream& is, jju8& v1 );
                friend istream& operator >> ( istream& is, jju16& v2 );
                friend istream& operator >> ( istream& is, jju32& v4 );
        }; // end istream class

#endif
