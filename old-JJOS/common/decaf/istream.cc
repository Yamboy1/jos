/*
 * istream.cc
 *
 * the input-stream workalike
 * for the decaf JVM.
 */

#include "istream.h"

istream::istream( jbRamDiskStream * jbrds ) : jbRamDiskStream( jbrds->myFirstByte, jbrds->myLastByte ) {
        this->myCurrentByte = jbrds->myCurrentByte;
        } /* end istream constructor */

istream& operator >> ( istream& is, jju8& v1 ) {
        if ( ! is.readjju8( v1 ) ) {
                kprintf( "istream: fault reading jju8\n" );
                }
        return is;
        }
         

istream& operator >> ( istream& is, jju16& v2 ) {
        if ( ! is.readjju16( v2 ) ) {
                kprintf( "istream: fault reading jju16\n" );
                }
        return is;
        }
  
istream& operator >> ( istream& is, jju32& v4 ) {
        if ( ! is.readjju32( v4 ) ) {
                kprintf( "istream: fault reading jju32\n" );
                }
        return is;
        }
