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
 * Avery Regier (ability to view native output with F1 console)
 */

#include "jjconsole.h"
#include "jbmachin.h"
#include "jbdebug.h"

/* not sure if this should be static in jjmachin,
   or if something else needs to be done (to manage
   resources) */

void out8 (jju16 port, jju8 value) {
  asm("movw %0, %%dx" : : "m" (port));
  asm("movb %0, %%al" : : "m" (value));
  asm("outb %al, %dx");
  }

void jjConsole::updateCursor() {
	  /* Put the cursor in the right place */
	  if( myColumn < (JJSCREENWIDTH-1) ) {
		/* we can put the cursor on the current row */
		setCursor( myRow, myColumn );
		} else {
		/* we need to put the cursor a line further down */
		if ( myRow + 1 < (JJSCREENHEIGHT-1) ) {
			/* we can put the cursor on the next row */
			setCursor( myRow + 1, 0 );
			} else {
			/* we need to scroll */
			putChar('\n');
			setCursor( myRow, 0 );
			} /* end if not on current line */
		} /* end if on current line */
	} /* end updateCursor() */
  
jjConsole::jjConsole(jju32 VGAScreenBufferPointer)
  : jbConsole()
{
  myVGAScreenBufferPointer = (unsigned char *) VGAScreenBufferPointer;

  /* There MUST be a better way to do this. */

  myRow = 0;
  myColumn = 0;

  buffer = NULL;
  visible = JJTRUE;
}

void jjConsole::setCursor(int row, int col)
{
  int debug = 0;

  /* the cursor is set by a 16-bit offset 
     into the framebuffer */
  short cursor_addr = ( 80 * 2 * row ) + (col * 2);
  /* some random VGA breakage */
  cursor_addr /= 2;
  jju8 high = (cursor_addr >> 8) & 0xff;
  jju8 low = cursor_addr & 0xff;

  /* output the high byte */
  out8( 0x3d4, 0x00e );
  out8( 0x3d5, high );
  /* and the low byte */
  out8( 0x3d4, 0x00f );
  out8( 0x3d5, low );

  /* debugging code */
  DEBUG(( "cursor_addr: %d, (%d|%d) %d\n", cursor_addr, high, low, (high << 8) | low ));
  if ( debug ) {
	setChar( row, col, 'X', VGA_MODE_7_INTENSE );
	}
} 

jju8 jjConsole::getAttribute(int row, int col)
{
  int subscript = (row*JJSCREENWIDTH*2) + (col*2) + 1;

  int attribute;
  if ( buffer != NULL ) { attribute = buffer[subscript]; }
  else { attribute = myVGAScreenBufferPointer[subscript]; }

  return(attribute);
}

jju8 jjConsole::getChar(int row, int col)
{
  int character;
  int subscript = (row*JJSCREENWIDTH*2) + (col*2);
  
  if( buffer != NULL ) { character = buffer[subscript]; }
  else { character = myVGAScreenBufferPointer[subscript]; }

  return(character);
}

void jjConsole::setChar(int row, int col, jju8 c, jju8 a)
{
  int charsubscript = (row*JJSCREENWIDTH*2) + (col*2);
  if( visible == JJTRUE ) { myVGAScreenBufferPointer[charsubscript] = c; }
  if( buffer != NULL ) { buffer[charsubscript] = c; }
  
  int attribsubscript = charsubscript + 1;
  if( visible == JJTRUE ) { myVGAScreenBufferPointer[attribsubscript] = a; }
  if( buffer != NULL ) { buffer[attribsubscript] = a; }
}
  

void jjConsole::scrollUpOneLine(void)
{
  int row;
  for (row=1; row<JJSCREENHEIGHT; row++) // Skip top row.
    {
      for (int col=0; col<JJSCREENWIDTH; col++)
	{
	  jju8 char_to_move = getChar(row, col);
	  jju8 attrib_to_move = getAttribute(row, col);

	  //	  jbmp->delay();
	  
	  setChar(row-1, col, char_to_move, attrib_to_move);
	}
    }

  /* Fill last line with blanks. */

  for (int col=0; col<JJSCREENWIDTH; col++)
    {
      setChar(row-1, col, ' ', VGA_MODE_7_NORMAL);
    }
}

int jjConsole::putChar(int c)
{
  if (c == '\n')
    {
      if (myRow < (JJSCREENHEIGHT-1)) // Can go to next row without scrolling.
	{
	  myColumn = 0;
	  myRow++;
	}
      else			// Must scroll everything else up to make room.
	{
	  scrollUpOneLine();
	  myColumn = 0;
	}
    }
  else
    {
      if (myColumn < (JJSCREENWIDTH-1))	// Can fit this char in current row.
	{
	  setChar(myRow, myColumn, c, VGA_MODE_7_NORMAL );
	  myColumn++;
	  updateCursor();
	}
      else			// Go to next row (maybe scrolling) and try again.
	{
	  putChar('\n');
	  putChar(c);
	}
    }

  return(c);
}

void jjConsole::clear_screen() {
	if ( visible == JJTRUE ) {
		for ( int x = 0; x < JJSCREENWIDTH * JJSCREENHEIGHT * 2; x += 2 ) {
			myVGAScreenBufferPointer[x] = ' ';
			myVGAScreenBufferPointer[x+1] = VGA_MODE_7_NORMAL;
			}
		}
	if ( buffer != NULL ) {
		for ( int x = 0; x < JJSCREENWIDTH * JJSCREENHEIGHT * 2; x += 2 ) {
			buffer[x] = ' ';
			buffer[x+1] = VGA_MODE_7_NORMAL;
			}
		}
	} /* end clear_screen() */
		
void jjConsole::setVisible( jjBoolean v ) {
	if ( buffer == NULL ) {
	       int allocationSize = sizeof( jju8 ) * JJSCREENWIDTH * JJSCREENHEIGHT * 2;

	       // Allocates the buffer off of the Java heap.  Is there a better place
	       // to allocate this from?  Could GC interfere with this?
	       buffer = (jju8 *) jbmp->myHeap->allocate(allocationSize);
		
	       for ( int x = 0; x < JJSCREENWIDTH * JJSCREENHEIGHT * 2; x += 2 ) {
			buffer[x] =  myVGAScreenBufferPointer[x]; 
			buffer[x+1] = myVGAScreenBufferPointer[x+1];
       			}
	       } /* end if buffer is NULL */
	
	if ( (visible == JJFALSE) && (v == JJTRUE )) {
		/* switching from invisible to visible, so draw the
		   buffer to the screen */
		for ( int x = 0; x < JJSCREENWIDTH * JJSCREENHEIGHT * 2; x += 2 ) {
			myVGAScreenBufferPointer[x] = buffer[x];  
          		myVGAScreenBufferPointer[x+1] = buffer[x+1];
		        }
		updateCursor();
    		}

	visible = v;
	} /* end setVisible */

