#ifndef JBARRAY_H
#define JBARRAY_H

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
 * This defines an array template class which checks array bounds.
 * It uses one level of indirection to get at the elements.
 *
 * Because the array objects in decaf inherit from Object and
 * from jbArray, jbArray must not have any virtual functions
 * to ensure that the native build's static casts will work.
 */

template <class elem_t> class jbArray
{
 public:
    jbArray(jju32 size, jju32 dims = 1);

    /*virtual*/ elem_t  store(jju32 index, elem_t value); /* Returns value param. */
    /*virtual*/ elem_t  load(jju32 index);
    jju32   size();
    jju32   dims();

    /* array type-checking support */
    enum primitiveArrayType {
	T_BOOLEAN = 4, T_CHAR = 5, T_FLOAT = 6, T_DOUBLE = 7,
	T_BYTE = 8, T_SHORT = 9, T_INT = 10, T_LONG = 11,
	T_CLASS = 3, T_MAPPED=99 }; /* T_CLASS, T_MAPPED are NOT in the spec */

#ifdef OLD_DECAF
    virtual primitiveArrayType array_type() const = 0;
#endif

 protected:

    jju32   myNumberOfElements;
    jju32   myNumberOfDims;
    elem_t *myElements;
};

template <class elem_t> jju32
jbArray<elem_t>::size() {
	return this->myNumberOfElements;
	} /* end size()*/

template <class elem_t> jju32
jbArray<elem_t>::dims() {
	return this->myNumberOfDims;
	} /* end size()*/

template <class elem_t>
jbArray<elem_t>::jbArray(jju32 size, jju32 dims)
{
  myNumberOfElements = size;
  myNumberOfDims = dims;
  myElements = new elem_t[myNumberOfElements];
}

template <class elem_t>
elem_t jbArray<elem_t>::store(jju32 index, elem_t value)
{
#ifdef PARANOID
    if (index >= myNumberOfElements)
	{
	    kprintf("jbArray<elem_t>::store(%d, ...), index too big (>=%d)\n",
		    index, myNumberOfElements);
	    return(myElements[0]);
	}
    else
#endif
	{
	    myElements[index] = value;
	    return(myElements[index]);
	}
}
	    
template <class elem_t>
elem_t jbArray<elem_t>::load(jju32 index)
{
#ifdef PARANOID
    if (index >= myNumberOfElements)
	{
	    kprintf("jbArray<elem_t>::load(%d), index too big (>=%d)\n",
		    index, myNumberOfElements);
	    abort();
	    return(myElements[0]);
	}
    else
#endif
	{
	    return(myElements[index]);
	}
}

#endif
