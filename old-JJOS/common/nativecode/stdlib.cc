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


#ifdef TARGET_UNIX
#include <stdio.h>		// For printf()
#endif

#include "stdlib.h"
#include "jbmachin.h"
#include "jbdebug.h"

static int debug = 0;

void dumpmem(void *from, int nbytes)
{
  jju8 *p = (jju8 *)from;

#ifdef NOTDEF
  for (int i=0; i<nbytes; i++)
    {
      char buf[1024];
      
      itoa(buf, p[i]);

      char buftwo[3];
      buftwo[0] = buf[6];
      buftwo[1] = buf[7];
      buftwo[2] = '\0';

      kprintf(" %s", buftwo);
    }
#else
  for (int i=0; i<nbytes; i++)
    {
      char buf[1024];
      
      itoa(buf, p[i]);

      char buftwo[3];
      buftwo[0] = buf[6];
      buftwo[1] = buf[7];
      buftwo[2] = '\0';

      kprintf(" %s", buftwo);

      if ((i & 0xf) == 0xf)
	{
	  kprintf("\n");
	}
    }
  kprintf("\n");
#endif
}

unsigned char nibble_to_char(register unsigned char nibble)
{
  if (nibble < 10)
    {
      return(nibble + 0x30);
    }
  else
    {
      return((nibble-10) + 0x41);
    }
}

void itoa(char buf[], unsigned long l)
{
  int i;

  for (i=0; i<8; i++)
    {
      unsigned char nibble = (l >> ((7 - i) << 2)) & 0xf;
      buf[i] = nibble_to_char(nibble);
    }
}


/*
 *
 */

#include <stdarg.h>

/*
 * This could be vastly improved by:
 *  implementing floating-point
 *  implementing width specifiers, etc.
 */

int kprintf(const char *format, ...)
{
  int number_formatted_items = 0;

  va_list args;
  va_start(args, format);

  while (*format)
    {
      int c = *format++;
      
      switch(c)
	{
	case '%':
	  {
	    int type = *format++;
	    
	    switch(type)
	      {
	      case 'c':
	      case 'C':
		{
		  char ch = va_arg(args, char);
		  jbmp->myConsole->putChar(ch);
		}
		number_formatted_items++;
		break;
		
	      case 'd':
	      case 'D':
		{
		  int j = va_arg(args, int);
		  jbmp->myConsole->printULong(j);
		}
		number_formatted_items++;
		break;
		
	      case 'f':
	      case 'F':
		{
#ifdef TARGET_UNIX
		  double d = va_arg(args, double);
		  printf("%f", d); // $$$ %g? %F?
#else
		  jbmp->myConsole->printString("<double fmt unsupported>");
#endif
		}
		break;

	      case 'x':
	      case 'X':
		{
		  jju32 l = va_arg(args, jju32);
		  jbmp->myConsole->printULong(l);
		}
		number_formatted_items++;
		break;

	      case 'p':
	      case 'P':
		{
		  jju32 l = va_arg(args, jju32);
		  jbmp->myConsole->printULong(l);
		}
		number_formatted_items++;
		break;

	      case 's':
	      case 'S':
		{
		  char *s = va_arg(args, char *);
		  jbmp->myConsole->printString(s);
		}
		number_formatted_items++;
		break;

	      case 'u':
	      case 'U':
		{
		  jju32 l = va_arg(args, jju32);
		  jbmp->myConsole->printULong(l);
		}
		number_formatted_items++;
		break;

	      default:
		jbmp->myConsole->printString("bad format");
	      }
	  }
	  break;

	default:
	  jbmp->myConsole->putChar(c);
	  break;
	}
    }

  va_end(args);

  return(number_formatted_items);
}

/* string compare, courtesy of Gilbert Carl Herschberger II */
int strcmp(char *v1, char *v2 ) {
  if ( v1 == NULL && v2 == NULL ) {
    return 0;
  }
  if ( v1 == NULL ) {
    return -1;
  }
  if ( v2 == NULL ) {
    return 1;
  }

  for (;; v1++, v2++ )
    if ( *v1 == *v2 ) {
      if ( *v1 == '\0' ) return 0;
    }
    else
      return ( *v1 < *v2 ) ? -1 : 1;
}

size_t strlen(const char *s)
{
  int length = 0;

  while(*s++)
    {
      length++;
    }
  return(length);
}

char *strcpy(char *dest, const char *src)
{
  while(*src)
    {
      *dest++ = *src++;
    }
  *dest = '\0';
  return(dest);
}

char *strdup(const char *s)
{
  char *retval = new char[strlen(s)+1];

  if (!retval)
    {
      kprintf("strdup() -- out of memory\n");
    }
  else
    {
      strcpy(retval, s);
    }
  return(retval);
}

const char *index(const char *s, int c)
{
  const char *p = s;

  while (*p)
    {
      if (*p == c)
	{
	  return(p);
	}
      else
	{
	  p++;
	}
    }
  return(NULL);
}

const char *strpbrk(const char *s, const char *accept)
{
  const char *p = s;

  while (*p)
    {
      if (index(accept, *p))
	{
	  return(p);
	}
    }
  return(NULL);
}

char *strtok(char *s, const char *delim)
{
  static char *foo = NULL;
  
  DEBUG(("==========strtok -- 0 %d %X \"%s\"\n",
	 strlen(foo ? foo : s), foo, foo));

  if (s)			// Reduce first call to general case.
    {
      DEBUG(("strtok -- 1\n"));
      foo = s;
      return(strtok(NULL, delim));
    }
  else if (!(*foo))		// Have run out of characters.
    {
      DEBUG(("strtok -- 2\n"));
      return(NULL);
    }
  else if (index(delim, *foo))	// Skip leading whitespace.
    {
      DEBUG(("strtok -- 3\n"));
      foo++;
      return(strtok(NULL, delim));
    }
  else
    {
      DEBUG(("strtok -- 4\n"));
      char *retval = foo;

      while (*foo)
	{
	  if (index(delim, *foo))
	    {
	      *foo = '\0';
	      foo++;
	      DEBUG(("strtok -- 5 \"%s\"\n", retval));
	      return(retval);
	    }
	  else
	    {
	      DEBUG(("strtok -- 6 '%c'\n", *foo));
	      foo++;
	    }
	}
      DEBUG(("strtok -- 7 \"%s\"\n", retval));
      return(retval);
    }
}



void jb_test_strings(void)
{
  char *s1 = "foobar";
  char *s2 = strdup(s1);

  kprintf("s1 = %s, s2 = %s\n", s1, s2);
  kprintf("strlen(%s) = %d\n", s1, strlen(s1));

  char s3[512];

  strcpy(s3, s2);
  kprintf("s3 = %s\n", s3);

  kprintf("index(%s, q) = %s\n", s3, index(s3, 'q'));
  kprintf("index(%s, f) = %s\n", s3, index(s3, 'f'));
  kprintf("index(%s, b) = %s\n", s3, index(s3, 'b'));

  char *s4 = strdup("arg0 arg1 arg2     arg3            arg4 \n arg5 ar");
  char *delim = " \n";

  kprintf("call 1: %s\n", strtok(s4, delim));
  char *s5;
  while ((s5 = strtok(NULL, delim)))
    {
      kprintf(" s5 = %s\n", s5);
    }
}
  
#ifndef TARGET_UNIX

void abort(void) {
	jbmp->halt();
} /* end abort() */

int isnan( jjfloat64 d ) {
	return d != d;
} /* end isnan() */

int isnan( jjfloat32 f ) {
	return f != f;
} /* end isnan() */

jjfloat64 int_pow( jjfloat64 base, jji32 exp ) {
	if( exp == 1 ) { return base; }
	else if ( exp & 0x00000001 ) { /* odd, if little-endian... */
		return int_pow( base, exp/2 ) * int_pow( base, exp/2 ) * base;
		} /* end if odd */ 
	else { /* even */
		return int_pow( base, exp/2 ) * int_pow( base, exp/2 );
		} /* end if even */
	} /* end int_pow */

jjfloat64 pow( jjfloat64 d, jjfloat64 f ) {
	jji32 integral = (jji32)f;		/* $$$ should be jji64 */
	jjfloat64 fractional = f - integral;    
	return (1/fractional) * int_pow( d, integral ); 
} /* end pow() */ 

#endif
