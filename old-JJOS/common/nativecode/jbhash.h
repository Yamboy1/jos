#ifndef JBHASH_H
#define JBHASH_H

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

template <class KeyType, class DatumType>
class jbHashtable
{
 public:
  jbHashtable(jju32 size);
  ~jbHashtable();
  
  DatumType lookup(const KeyType);
  void      store(const KeyType, const DatumType);

protected:

  jju32 mySize;			// Better be a power of two.
  jju32 myMask;			// 

private:
  class jbHashTableItem
  {
  public:
    jbHashTableItem(KeyType key, DatumType datum)
      {
	myKey = key;
	myDatum = datum;
	myNext = NULL;
      };
    KeyType myKey;
    DatumType myDatum;
    jbHashTableItem *myNext;
  };

  jbHashTableItem **myItems;
};


template <class KeyType, class DatumType>
jbHashtable<KeyType, DatumType>::jbHashtable(jju32 size)
{
  mySize = size;
  myMask = size-1;
  myItems = new jbHashTableItem *[mySize];

  /* Typically paranoid clearing of array items. */

  for (jju32 i=0; i<mySize; i++)
    {
      myItems[i] = NULL;
    }
}

template <class KeyType, class DatumType>
jbHashtable<KeyType, DatumType>::~jbHashtable()
{
  for (jju32 i=0; i<mySize; i++)
    {
      while (myItems[i])
	{
	  jbHashTableItem *tmp = myItems[i]->myNext;
	  delete(myItems[i]);
	  myItems[i] = tmp;
	}
    }

  delete[] myItems;
}

template <class KeyType, class DatumType>
DatumType jbHashtable<KeyType, DatumType>::lookup(const KeyType key)
{
  jju32 hash_value = hash(key);
  jju32 hash_index = hash_value & myMask;

  jbHashTableItem *tmp = myItems[hash_index];

  while (tmp)
    {
      if (equal(tmp->myKey, key))
	{
	  return(tmp->myDatum);
	}
      else
	{
	  tmp = tmp->myNext;
	}
    }

  return((DatumType)NULL);	// This won't work for non-pointer types.
}

template <class KeyType, class DatumType>
void jbHashtable<KeyType, DatumType>::store(const KeyType key, const DatumType datum)
{
  DatumType d = lookup(key);

  if (d)
    {
      kprintf("already stored (%s, %d)\n", datum, key);
    }
  else
    {
      jju32 hash_value = hash(key);
      jju32 hash_index = hash_value & myMask;
      jbHashTableItem *new_hti = new jbHashTableItem(key, datum);

      if (!myItems[hash_index])
	{
	  myItems[hash_index] = new_hti;
	}
      else
	{
	  new_hti->myNext = myItems[hash_index];
	  myItems[hash_index] = new_hti;
	}
    }
}

#endif
