/*
 * constantpool.cc
 * 
 * defines the classes representing
 * the constant pool for a Java class.
 */

#include "constantpool.h"

ConstantPool::ConstantPool() {
    ;
    }

ConstantPool * ConstantPool::generateConstantPool( istream & is ) {
    /* the ConstantPool we'll be returning. */
    RawConstantPool * rcp = new RawConstantPool();

    if ( rcp == NULL ) {
        kprintf( "ConstantPool::generateConstantPool() -- unable to allocate raw CP, aborting.\n" );
        abort();
        }

    /* Number of items in the constant pool. */
    is >> rcp->myCount;

    /* Allocate an empty constant pool. */
    rcp->myConstantPool = new CPEntry*[rcp->myCount];

    if ( rcp->myConstantPool == NULL ) {
        kprintf( "ConstantPool::generateConstantPool() -- unable to allocate constant pool, aborting.\n" );
        abort();
        }

    /* Construct the constant pool.
     * The initial constant pool entry is
     * reserved for the JVM's internal use. */
    for ( int x = 1; x < rcp->myCount; x++ ) {
        /* Construct this entry */
        rcp->myConstantPool[x] = CPEntry::generateCPEntry( is );

        /* Verify its validity. */
        if( rcp->myConstantPool[x] == NULL ) {
            kprintf( "ConstantPool::generateConstantPool() -- failed to generate CP entry, aborting.\n" );
            abort();
            }

        /* Adjust the index, if necessary. */
        if ( rcp->myConstantPool[x]->usesTwoEntries() ) { x++; }       
        } /* end constant pool construction loop */

    return rcp;
    } /* end generateConstantPool() */  

RawConstantPool::RawConstantPool() {
	myCount = 0;
	myConstantPool = NULL;
	} /* end of RawConstantPool() */

CPEntry * RawConstantPool::operator [] ( jju16 index ) {
    if( index < myCount ) {
        return myConstantPool[index];
        } else { 
        kprintf( "RawConstantPool::operator[] (%d) -- index out of bounds, aborting.\n", index );
        abort();
        }
    /* should never happen */
    return NULL;
    } /* end operator [] () */
