/*
 * interfacelist.cc
 * 
 * defines the class which
 * represents the list of
 * interfaces that a Java class
 * implements.
 */

#include "interfacelist.h"

/* InterfaceList */

InterfaceList::InterfaceList() { ; }

InterfaceList * InterfaceList::generateInterfaceList( istream & is, JavaClass * jc ) {
   return RawInterfaceList::generateRawInterfaceList( is, jc );
   } /* end InterfaceList::generateInterfaceList() */


/* RawInterfaceList */

RawInterfaceList::RawInterfaceList() {
    myInterfaceCount = 0;
    myInterfaceIndexList = NULL;
    }

jju16 RawInterfaceList::operator [] (int index) {
    if ( index >= 0 && index < myInterfaceCount ) {
        if ( myInterfaceIndexList != NULL ) {
            return myInterfaceIndexList[index];
            } else {
            kprintf( "RawInterfaceList::operator []() -- myInterfaceIndexList is NULL, aborting.\n" );
            abort();
            }
        } else {
        kprintf( "RawInterfaceList::operator []() -- index out of bounds, aborting.\n" );
        abort();
        }
    /* this should never happen */
    return (jju16)0;
    } /* end operator []() */

RawInterfaceList * RawInterfaceList::generateRawInterfaceList( istream & is, JavaClass * jc ) {
    /* generate clean InterfaceList */
    RawInterfaceList * ril = new RawInterfaceList();
    if ( ril == NULL ) {
        kprintf( "RawInterfaceList::generateRawInterfaceList() -- unable to generate RIL, aborting.\n" );
        abort();
        }

    /* allocate myInterfaceIndexList */
    is >> ril->myInterfaceCount;
    if ( ril->myInterfaceCount == 0 ) { return ril; }
    ril->myInterfaceIndexList = new jju16[ ril->myInterfaceCount ];
    if ( ril->myInterfaceIndexList == NULL ) {
        kprintf( "RawInterfaceList::generateRawInterfaceList() -- unable to allocate RIL, aborting.\n" );
        abort();
        }

    /* fill in myInterfaceIndexList */
    for ( int x = 0; x < ril->myInterfaceCount; x++ ) {
        is >> ril->myInterfaceIndexList[x];
        }

    return ril;
    } /* end RawInterfaceList::generateRawInterfaceList() */
