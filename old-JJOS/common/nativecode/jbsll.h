#ifndef JBSLL_H
#define JBSLL_H
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

template <class Type> class jbSLLItem;

/*
 * The singly-linked list proper.
 */

template <class Type>
class jbSLL
{
 public:
    jbSLL();
    ~jbSLL();

    void	init(void);
    Type	remove(const Type&);
    void	append(const Type&);
    jjBoolean	isEmpty(void);
    jju32	length(void);
    jbSLLItem<Type> *lookup(const Type&);


    // private:
    jbSLLItem<Type>	*_remove(jbSLLItem<Type>*, const Type&);
    jbSLLItem<Type>	*_lookup(jbSLLItem<Type>*, const Type&);
    jbSLLItem<Type>	*_append(jbSLLItem<Type>*, jbSLLItem<Type> *);
    jju32		_length(jbSLLItem<Type> *);

    jbSLLItem<Type>	*myFront;
};

template <class Type>
jbSLL<Type>::jbSLL()
{
  init();
}

template <class Type>
void jbSLL<Type>::init(void)
{
  myFront = NULL;
}

template <class Type>
jbSLL<Type>::~jbSLL()
{
  while (!isEmpty())
    {
      (void)remove(myFront->myItem);
    }
}

template <class Type>
jjBoolean jbSLL<Type>::isEmpty(void)
{
  if (myFront == NULL)
    {
      return(JJTRUE);
    }
  else
    {
      return(JJFALSE);
    }
}    


template <class Type>
jju32 jbSLL<Type>::_length(jbSLLItem<Type> *l)
{
  if (l == NULL)
    {
      return(0);
    }
  else
    {
      return(1 + _length(l->myNext));
    }
}

template <class Type>
jju32 jbSLL<Type>::length(void)
{
  return(_length(myFront));
}


template <class Type>
jbSLLItem<Type> *jbSLL<Type>::_lookup(jbSLLItem<Type> *l, const Type &val)
{
  if (l == NULL)
    {
      return(NULL);
    }
  else if (l->myItem == val)
    {
      return(l);
    }
  else
    {
      return(_lookup(l->myNext, val));
    }
}

template <class Type>
jbSLLItem<Type> *jbSLL<Type>::lookup(const Type &val)
{
  if (isEmpty())
    {
      return(NULL);
    }
  else
    {
      return(_lookup(myFront, val));
    }
}


template <class Type>
jbSLLItem<Type> *jbSLL<Type>::_append(jbSLLItem<Type> *l, jbSLLItem<Type> *pt)
{
  if (l == NULL)
    {
      return(pt);
    }
  else if (l->myNext == NULL)
    {
      l->myNext = pt;
      return(l);
    }
  else
    {
      l->myNext = _append(l->myNext, pt);
      return(l);
    }
}

template <class Type>
void jbSLL<Type>::append(const Type &val)
{
  jbSLLItem<Type> *pt = new jbSLLItem<Type>(val);

  myFront = _append(myFront, pt);
}

template <class Type>
jbSLLItem<Type> *jbSLL<Type>::_remove(jbSLLItem<Type> *l, const Type &val)
{
  if (l == NULL)		/* No more to do. */
    {
      return(NULL);
    }
  else if (l->myItem == val)	/* Found it. */
    {
      jbSLLItem<Type> *retval = l->myNext;
      delete(l);		/* Does NOT delete myItem! */
      return(retval);
    }
  else				/* Check the rest of the list. */
    {
      l->myNext = _remove(l->myNext, val);
      return(l);
    }
}

template <class Type>
Type jbSLL<Type>::remove(const Type &val)
{
  if (isEmpty())
    {
      kprintf("jbSLL<>::remove -- remove on empty list\n");

#ifdef PARANOID
      jbmp->printBacktrace();
      jbmp->halt();
#endif
      /* There is no sensible return value at this point. */
      Type whatever_constructor_leaves;
      return(whatever_constructor_leaves);
    }
  else
    {
      myFront = _remove(myFront, val);
      /* $$$ How do we know we haven't found the thing? */
      return(val);
    }
}
			    
    
/*
 * The singly-linked list item proper.
 */

template <class Type>
class jbSLLItem
{
  friend class jbSLL<Type>;

 public:

  jbSLLItem(const Type &val);
  ~jbSLLItem();

  Type myItem;
  jbSLLItem *myNext;
};

template <class Type>
jbSLLItem<Type>::jbSLLItem(const Type &val)
{
  myItem = val;
  myNext = NULL;
}

template <class Type>
jbSLLItem<Type>::~jbSLLItem()
{
  ;				/* Does NOT delete the myItem! */
  ;				/* NEITHER does it delete the rest of the list! */
}
#endif
