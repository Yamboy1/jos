/**
 * driver.KeyboardInterpreter.java
 *
 * KeyboardInterpreter is the h/w-specific
 * implementation of platform.KeyboardInterpreter.
 */

package jos.platform.driver;

import jos.platform.*;
import jos.platform.event.*;
import java.util.*;

public class KeyboardInterpreter extends jos.platform.KeyboardInterpreter {
  /* flag definitions */
  protected final static int FLAG_SHIFT = 0x01;
  protected final static int FLAG_ALT = 0x02;
  protected final static int FLAG_CONTROL = 0x04;
  protected final static int FLAG_META = 0x08;

/*
  protected final static int LEFT_SHIFT = 0x12;
  protected final static int RIGHT_SHIFT = 0x59;
  protected final static int LEFT_CONTROL = 0x58;
  protected final static int RIGHT_CONTROL = 0x64;
  protected final static int LEFT_ALT = 0x60;
  protected final static int RIGHT_ALT = 0x62;
*/

  /* a few state-key definitions */
  protected final static int LEFT_SHIFT = 0x2A;
  protected final static int RIGHT_SHIFT = 0x36;
  protected final static int LEFT_CONTROL = 0x1D;
  protected final static int RIGHT_CONTROL = 0x1D; // preceded by 0xE0
  protected final static int LEFT_ALT = 0x38;
  protected final static int RIGHT_ALT = 0x38; // preceded 0xE0

  protected final static int NUM_LOCK = 0x45;
  protected final static int SCROLL_LOCK = 0x46;
  protected final static int CAPS_LOCK = 0x3A;

  protected final static int ESCAPE = 0x01;

  protected final static int XT_RELEASE = 0x80;
  protected final static int XT_EXTENDED = 0xE0;

  /* auxiliary state variable and methods for handleScancode() */

  /* shift(ing) */
  protected boolean leftShift = false;
  protected boolean rightShift = false;
  protected boolean capsLock = false;
  protected boolean isShifted() {
      return leftShift || rightShift;
      } /* end isShifted() */
  protected boolean isAlphaShifted() {
      return (leftShift || rightShift) ^ capsLock;
      } /* end isShifted() */
      
  /* alt(ernate) */
  protected boolean rightAlt = false;
  protected boolean leftAlt = false;
  protected boolean isAlternate() {
      return rightAlt || leftAlt;
      }

  /* control */
  protected boolean rightControl = false;
  protected boolean leftControl = false;
  protected boolean isControl() {
      return rightControl || leftControl;
      }

  /* meta(key) -- the 101-key keyboard doesn't
   * have a dedicated META key, so this is here
   * for derived interpreters that do. */
  protected boolean rightMeta = false;
  protected boolean leftMeta = false;
  protected boolean isMetakey() {
      return rightMeta || leftMeta;
      }

  /* the other two locks */
  protected boolean numLock = false;
  protected boolean scrollLock = false;

  /* convenience */
  protected static final char NO_CHAR = (char)-1;

  /* last scancode -- may have state */
  protected int lastCode = 0;

  /* declared in the platform.KI abstract class */
  public boolean isShiftDown( KeyEvent ke ) {
      return (ke.flags & FLAG_SHIFT) != 0;
      }
  public boolean isControlDown( KeyEvent ke ) {
      return (ke.flags & FLAG_CONTROL) != 0;
      }
  public boolean isAltDown( KeyEvent ke ) {
      return (ke.flags & FLAG_ALT) != 0;
      }
  public boolean isMetaDown( KeyEvent ke ) {
      return (ke.flags & FLAG_META) != 0;
      }

  public void handleScancode( int scancode ) {
//      System.err.print( "driver.KI.handleScancode()\n" );

      int vkcode = -1;
      int flags = 0;
      char character = NO_CHAR;

      switch (scancode) {
            case LEFT_SHIFT:
		  leftShift = true;
//                  System.err.println( "leftShift down" );
                  break;
            case RIGHT_SHIFT:
		  rightShift = true;
//                  System.err.println( "rightShift down" );
                  break;

            case (LEFT_SHIFT | XT_RELEASE):
		  leftShift = false;
//                  System.err.println( "leftShift up" );
                  break;
            case (RIGHT_SHIFT | XT_RELEASE):
		  rightShift = false;
//                  System.err.println( "rightShift up" );
                  break;

            case LEFT_CONTROL:
		  if ( lastCode == XT_EXTENDED ) {
			rightControl = true;
//			System.err.println( "rightControl down" );
			break;
			}
		  leftControl = true;
//                  System.err.println( "leftControl down" );
                  break;

	    case (LEFT_CONTROL | XT_RELEASE):
		  if ( lastCode == XT_EXTENDED ) {
			rightControl = false;
//			  System.err.println( "rightControl up" );
			break;
			}
		  leftControl = false;
//		    System.err.println( "leftControl up" );
		  break;

            case LEFT_ALT:
		  if ( lastCode == XT_EXTENDED ) {
			rightAlt = true;
//			  System.err.println( "rightAlt down" );
			break;
			}
		  leftAlt = true;
//                  System.err.println( "leftAlt down" );
		  break;

            case (LEFT_ALT | XT_RELEASE):
		  if ( lastCode == XT_EXTENDED ) {
			rightAlt = false;
//			  System.err.println( "rightAlt up" );
			break;
			}
		  leftAlt = false;
//                  System.err.println( "leftAlt up" );
                  break;

	    case NUM_LOCK:
		  numLock = true;
//	 	    System.err.println( "numLock down" );
	          break;
	    case (NUM_LOCK | XT_RELEASE):
		  numLock = false;
//		    System.err.println( "numLock up" );
		  break;

	    case SCROLL_LOCK:
		  scrollLock = true;
//		    System.err.println( "scrollLock down" );
	        break;
	    case (SCROLL_LOCK | XT_RELEASE):
		  scrollLock = false;
//		    System.err.println( "scrollLock up" );
		  break;

	    case CAPS_LOCK:
		  /* This shouldn't affect it's activation status. */
//  		    System.err.println( "capsLock down" );
	          break;
	    case (CAPS_LOCK | XT_RELEASE):
		  capsLock = ! capsLock;
//		    System.err.println( "capsLock up" );
		  break;

          default:
	      /* Ignore key releases for now. */
	      if ( scancode >= translation_table.length ) { break; }

              vkcode = deriveVK( scancode );
              character = deriveChar( scancode );
	
//	      if ( vkcode != -1 ) { System.err.print( vkcode ); }
//	      System.err.print( " / " );
//	      if ( character != -1 ) { System.err.print( character ); }
//	      System.err.println( "" );
              break;
          } /* end scancode switch */

      /* prep the next look */
      lastCode = scancode;

      /* set the flags */
      if ( isMetakey() ) { flags |= FLAG_META; }
      if ( isAlternate() ) { flags |= FLAG_ALT; }
      if ( isShifted() ) { flags |= FLAG_SHIFT; }
      if ( isControl() ) { flags |= FLAG_CONTROL; }

      if ( character != NO_CHAR ) {
//	System.out.println( "driver.KI.handleScancode(): KEY_TYPED event." );
      	KeyEvent ke = new KeyEvent( KeyEvent.KEY_TYPED, scancode, vkcode, character, flags ); 
      	dispatchKeyEvent( ke );
	}
      else if ( vkcode != -1 ) {
//	System.out.println( "driver.KI.handleScancode(): KEY_PRESSED event." );
      	KeyEvent ke = new KeyEvent( KeyEvent.KEY_PRESSED, scancode, vkcode, character, flags ); 
      	dispatchKeyEvent( ke );
	}
      } /* end handleScancode() */

  public char deriveChar( int scancode ) {
      try {
	    char c = NO_CHAR;
            if ( isShifted() ) {
		  c = uppercase[scancode];
		  if ( capsLock && c <= 'Z' && c >= 'A' ) {
			c = lowercase[scancode];
			}
                  } else {
		  c = lowercase[scancode];
		  if ( capsLock && c <= 'z' && c >= 'a' ) {
			c = uppercase[scancode];
			}
		  } /* end case */
	   return c;
           } /* end try */      
      catch ( IndexOutOfBoundsException iiobe ) {
            /* we've failed to match the scancode at all.  How curious. */
            System.err.print( "KI failed to match " );
            System.err.println( scancode );
            return NO_CHAR;
            }
      } /* end deriveChar */

  public int deriveVK( int scancode ) {
      try { return translation_table[scancode]; }
      catch ( IndexOutOfBoundsException iiobe ) {
            /* we've failed to match the scancode at all.  How curious. */
            System.err.print( "KI failed to match " );
            System.err.println( scancode );
            return -1;
            }
      } /* end deriveVK */

public static final int[] translation_table = {
        -1, // 0x00 = nothing
        KeyEvent.VK_ESCAPE, // 0x01 = escape
        KeyEvent.VK_1, // 0x02 = 1/!
        KeyEvent.VK_2, // 0x03 = 2/@
        KeyEvent.VK_3, // 0x04 = 3/#
        KeyEvent.VK_4, // 0x05 = 4/$
        KeyEvent.VK_5, // 0x06 = 5/%
        KeyEvent.VK_6, // 0x07 = 6/^
        KeyEvent.VK_7, // 0x08 = 7/&
        KeyEvent.VK_8, // 0x09 = 8/*
        KeyEvent.VK_9, // 0x0a = 9/(
        KeyEvent.VK_0, // 0x0b = 0/)
        KeyEvent.VK_SUBTRACT, // 0x0c = -/_
        KeyEvent.VK_EQUALS, // 0x0d = =/+
        KeyEvent.VK_BACK_SPACE, // 0x0e = backspace
        KeyEvent.VK_TAB, // 0x0f = tab

        KeyEvent.VK_Q, // 0x10 = q/Q
        KeyEvent.VK_W, // 0x11 = w/W
        KeyEvent.VK_E, // 0x12 = e/E
        KeyEvent.VK_R, // 0x13 = r/R
        KeyEvent.VK_T, // 0x14 = t/T
        KeyEvent.VK_Y, // 0x15 = y/Y
        KeyEvent.VK_U, // 0x16 = u/U
        KeyEvent.VK_I, // 0x17 = i/I
        KeyEvent.VK_O, // 0x18 = o/O
        KeyEvent.VK_P, // 0x19 = p/P
        KeyEvent.VK_OPEN_BRACKET, // 0x1a = [/{
        KeyEvent.VK_CLOSE_BRACKET, // 0x1b = ]/}
        KeyEvent.VK_ENTER, // 0x1c = keyboard enter
        KeyEvent.VK_CONTROL, // 0x1d = left ctrl
        KeyEvent.VK_A, // 0x1e = a/A
        KeyEvent.VK_S, // 0x1f = s/S
        KeyEvent.VK_D, // 0x20 = d/D
        KeyEvent.VK_F, // 0x21 = f/F
        KeyEvent.VK_G, // 0x22 = g/G
        KeyEvent.VK_H, // 0x23 = h/H
        KeyEvent.VK_J, // 0x24 = j/J
        KeyEvent.VK_K, // 0x25 = k/K
        KeyEvent.VK_L, // 0x26 = l/L
        KeyEvent.VK_SEMICOLON, // 0x27 = ;/:
        KeyEvent.VK_QUOTE, // 0x28 = '/"
        KeyEvent.VK_BACK_QUOTE, // 0x29 = tick/~
        KeyEvent.VK_SHIFT, // 0x2a = left shift
        KeyEvent.VK_BACK_SLASH, // 0x2b = backslash ****
        KeyEvent.VK_Z, // 0x2c = z/Z
        KeyEvent.VK_X, // 0x2d = x/X
        KeyEvent.VK_C, // 0x2e = c/C
        KeyEvent.VK_V, // 0x2f = v/V

        KeyEvent.VK_B, // 0x30 = b/B
        KeyEvent.VK_N, // 0x31 = n/N
        KeyEvent.VK_M, // 0x32 = m/M
        KeyEvent.VK_COMMA, // 0x33 = ,/<
        KeyEvent.VK_PERIOD, // 0x34 = ./>
        KeyEvent.VK_SLASH, // 0x35 = //?
        KeyEvent.VK_SHIFT, // 0x36 = right shift
        KeyEvent.VK_MULTIPLY, // 0x37 = pad *
        KeyEvent.VK_ALT, // 0x38 = left alt
        KeyEvent.VK_SPACE, // 0x39 = space
        KeyEvent.VK_CAPS_LOCK, // 0x3a = Caps Lock
        KeyEvent.VK_F1, // 0x3b = f1
        KeyEvent.VK_F2, // 0x3c = f2
        KeyEvent.VK_F3, // 0x3d = f3
        KeyEvent.VK_F4, // 0x3e = f4
        KeyEvent.VK_F5, // 0x3f = f5

        KeyEvent.VK_F6, // 0x40 = f6
        KeyEvent.VK_F7, // 0x41 = f7
        KeyEvent.VK_F8, // 0x42 = f8
        KeyEvent.VK_F9, // 0x43 = f9
        KeyEvent.VK_F10, // 0x44 = f10
        KeyEvent.VK_NUM_LOCK, // 0x45 = Num Lock
        KeyEvent.VK_SCROLL_LOCK, // 0x46 = Scroll Lock
        KeyEvent.VK_NUMPAD7, // 0x47 = pad 7
        KeyEvent.VK_NUMPAD8, // 0x48 = pad 8
        KeyEvent.VK_NUMPAD9, // 0x49 = pad 9
        KeyEvent.VK_SUBTRACT, // 0x4a = pad -
        KeyEvent.VK_NUMPAD4, // 0x4b = pad 4
        KeyEvent.VK_NUMPAD5, // 0x4c = pad 5
        KeyEvent.VK_NUMPAD6, // 0x4d = pad 6
        KeyEvent.VK_ADD, // 0x4e = pad plus
        KeyEvent.VK_NUMPAD1, // 0x4f = pad 1

        KeyEvent.VK_NUMPAD2, // 0x50 = pad 2
        KeyEvent.VK_NUMPAD3, // 0x51 = pad 3
        KeyEvent.VK_NUMPAD0, // 0x52 = pad 0
        KeyEvent.VK_DECIMAL, // 0x53 = pad .
        -1, // 0x54 = nothing
        -1, // 0x55 = nothing
        -1, // 0x56 = nothing
        KeyEvent.VK_F11, // 0x57 = f11
        KeyEvent.VK_F12, // 0x58 = f12
        -1, // 0x59 = nothing
        -1, // 0x5a = nothing
        -1, // 0x5b = nothing
        -1, // 0x5c = nothing
        -1, // 0x5d = nothing
        -1, // 0x5e = nothing
        -1, // 0x5f = nothing

        /* These are synthetic scancodes */
        KeyEvent.VK_INSERT, // 0x60 = insert (E0 + 52)
        KeyEvent.VK_HOME, // 0x61 = home (EO + 47)
        KeyEvent.VK_PAGE_UP, // 0x62 = page up ( E0 + 49)
        KeyEvent.VK_DIVIDE, // 0x63 = pad slash (E0 + 35)
        KeyEvent.VK_PRINTSCREEN, // 0x64 = Print Screen/SysRq (E0 + 37)
        KeyEvent.VK_DELETE, // 0x65 = delete (E0 + 53)
        KeyEvent.VK_END, // 0x66 = end (E0 + 4f)
        KeyEvent.VK_PAGE_DOWN, // 0x67 = page down (E0 + 51)
        KeyEvent.VK_UP, // 0x68 = up arrow (E0 + 48)
        KeyEvent.VK_SEPARATER, // 0x69 = pad enter (E0 + 1c)
	  -1, // 0x70 = nothing
        -1, // 0x6a = left window (E0 + 5b)
        -1, // 0x6b = right alt (E0 + 38)
        -1, // 0x6c = right window (E0 + 5c)
	  -1, // 0x6d = nothing
        KeyEvent.VK_FINAL, // 0x6d = keyboard mouse (E0 + 5d)
        KeyEvent.VK_CONTROL, // 0x6e = right ctrl (E0 + 1d)
        KeyEvent.VK_LEFT, // 0x6f = left arrow (E0 + 4b)
        KeyEvent.VK_DOWN, // 0x70 = down arrow (E0 + 50)
        KeyEvent.VK_RIGHT, // 0x71 = right arrow (E0 + 4d)
        KeyEvent.VK_PAUSE  // 0x72 = pause / break (E1 + 1d), but could be
                           // (9D) or (C5) or (5-E1)
	}; /* end translation table */


public static final char[] lowercase = {
        NO_CHAR, // 0x00 = nothing
        NO_CHAR, // 0x01 = escape
        '1', // 0x02 = 1/!
        '2', // 0x03 = 2/@
        '3', // 0x04 = 3/#
        '4', // 0x05 = 4/$
        '5', // 0x06 = 5/%
        '6', // 0x07 = 6/^
        '7', // 0x08 = 7/&
        '8', // 0x09 = 8/*
        '9', // 0x0a = 9/(
        '0', // 0x0b = 0/)
        '-', // 0x0c = -/_
        '=', // 0x0d = =/+
        NO_CHAR, // 0x0e = backspace
        NO_CHAR, // 0x0f = tab

        'q', // 0x10 = q/Q
        'w', // 0x11 = w/W
        'e', // 0x12 = e/E
        'r', // 0x13 = r/R
        't', // 0x14 = t/T
        'y', // 0x15 = y/Y
        'u', // 0x16 = u/U
        'i', // 0x17 = i/I
        'o', // 0x18 = o/O
        'p', // 0x19 = p/P
        '[', // 0x1a = [/{
        ']', // 0x1b = ]/}
        NO_CHAR, // 0x1c = keyboard enter
        NO_CHAR, // 0x1d = left ctrl
        'a', // 0x1e = a/A
        's', // 0x1f = s/S
        'd', // 0x20 = d/D
        'f', // 0x21 = f/F
        'g', // 0x22 = g/G
        'h', // 0x23 = h/H
        'j', // 0x24 = j/J
        'k', // 0x25 = k/K
        'l', // 0x26 = l/L
        ';', // 0x27 = ;/:
        '\'', // 0x28 = '/"
        '`', // 0x29 = tick/~
        NO_CHAR,  // 0x2a = left shift
        '\\', // 0x2b = backslash ****
        'z', // 0x2c = z/Z
        'x', // 0x2d = x/X
        'c', // 0x2e = c/C
        'v', // 0x2f = v/V

        'b', // 0x30 = b/B
        'n', // 0x31 = n/N
        'm', // 0x32 = m/M
        ',', // 0x33 = ,/<
        '.', // 0x34 = ./>
        '/', // 0x35 = //?
        NO_CHAR, // 0x36 = right shift
        '*', // 0x37 = pad *
        NO_CHAR, // 0x38 = left alt
        ' ', // 0x39 = space
        NO_CHAR, // 0x3a = Caps Lock
        NO_CHAR, // 0x3b = f1
        NO_CHAR, // 0x3c = f2
        NO_CHAR, // 0x3d = f3
        NO_CHAR, // 0x3e = f4
        NO_CHAR, // 0x3f = f5

        NO_CHAR, // 0x40 = f6
        NO_CHAR, // 0x41 = f7
        NO_CHAR, // 0x42 = f8
        NO_CHAR, // 0x43 = f9
        NO_CHAR, // 0x44 = f10
        NO_CHAR, // 0x45 = Num Lock
        NO_CHAR, // 0x46 = Scroll Lock
        '7', // 0x47 = pad 7
        '8', // 0x48 = pad 8
        '9', // 0x49 = pad 9
        '-', // 0x4a = pad -
        '4', // 0x4b = pad 4
        '5', // 0x4c = pad 5
        '6', // 0x4d = pad 6
        '+', // 0x4e = pad plus
        '1', // 0x4f = pad 1

        '2', // 0x50 = pad 2
        '3', // 0x51 = pad 3
        '0', // 0x52 = pad 0
        '.', // 0x53 = pad .

        NO_CHAR, // 0x54 = nothing
        NO_CHAR, // 0x55 = nothing
        NO_CHAR, // 0x56 = nothing
        NO_CHAR, // 0x57 = nothing
        NO_CHAR, // 0x58 = nothing
        NO_CHAR, // 0x59 = nothing
        NO_CHAR, // 0x5a = nothing
        NO_CHAR, // 0x5b = nothing
        NO_CHAR, // 0x5c = nothing
        NO_CHAR, // 0x5d = nothing
        NO_CHAR, // 0x5e = nothing
        NO_CHAR, // 0x5f = nothing
	}; /* end lowercase table */


public static final char[] uppercase = {
        NO_CHAR, // 0x00 = nothing
        NO_CHAR, // 0x01 = escape
        '!', // 0x02 = 1/!
        '@', // 0x03 = 2/@
        '#', // 0x04 = 3/#
        '$', // 0x05 = 4/$
        '%', // 0x06 = 5/%
        '^', // 0x07 = 6/^
        '&', // 0x08 = 7/&
        '*', // 0x09 = 8/*
        '(', // 0x0a = 9/(
        ')', // 0x0b = 0/)
        '_', // 0x0c = -/_
        '+', // 0x0d = =/+
        NO_CHAR, // 0x0e = backspace
        NO_CHAR, // 0x0f = tab

        'Q', // 0x10 = q/Q
        'W', // 0x11 = w/W
        'E', // 0x12 = e/E
        'R', // 0x13 = r/R
        'T', // 0x14 = t/T
        'Y', // 0x15 = y/Y
        'U', // 0x16 = u/U
        'I', // 0x17 = i/I
        'O', // 0x18 = o/O
        'P', // 0x19 = p/P
        '{', // 0x1a = [/{
        '}', // 0x1b = ]/}
        NO_CHAR, // 0x1c = keyboard enter
        NO_CHAR, // 0x1d = left ctrl
        'A', // 0x1e = a/A
        'S', // 0x1f = s/S
        'D', // 0x20 = d/D
        'F', // 0x21 = f/F
        'G', // 0x22 = g/G
        'H', // 0x23 = h/H
        'J', // 0x24 = j/J
        'K', // 0x25 = k/K
        'L', // 0x26 = l/L
        ':', // 0x27 = ;/:
        '"', // 0x28 = '/"
        '~', // 0x29 = tick/~
        NO_CHAR,  // 0x2a = left shift
        '|', // 0x2b = backslash ****
        'Z', // 0x2c = z/Z
        'X', // 0x2d = x/X
        'C', // 0x2e = c/C
        'V', // 0x2f = v/V

        'B', // 0x30 = b/B
        'N', // 0x31 = n/N
        'M', // 0x32 = m/M
        '<', // 0x33 = ,/<
        '>', // 0x34 = ./>
        '?', // 0x35 = //?
        NO_CHAR, // 0x36 = right shift
        '*', // 0x37 = pad *
        NO_CHAR, // 0x38 = left alt
        ' ', // 0x39 = space
        NO_CHAR, // 0x3a = Caps Lock
        NO_CHAR, // 0x3b = f1
        NO_CHAR, // 0x3c = f2
        NO_CHAR, // 0x3d = f3
        NO_CHAR, // 0x3e = f4
        NO_CHAR, // 0x3f = f5

        NO_CHAR, // 0x40 = f6
        NO_CHAR, // 0x41 = f7
        NO_CHAR, // 0x42 = f8
        NO_CHAR, // 0x43 = f9
        NO_CHAR, // 0x44 = f10
        NO_CHAR, // 0x45 = Num Lock
        NO_CHAR, // 0x46 = Scroll Lock
        '7', // 0x47 = pad 7
        '8', // 0x48 = pad 8
        '9', // 0x49 = pad 9
        '-', // 0x4a = pad -
        '4', // 0x4b = pad 4
        '5', // 0x4c = pad 5
        '6', // 0x4d = pad 6
        '+', // 0x4e = pad plus
        '1', // 0x4f = pad 1

        '2', // 0x50 = pad 2
        '3', // 0x51 = pad 3
        '0', // 0x52 = pad 0
        '.', // 0x53 = pad .

        NO_CHAR, // 0x54 = nothing
        NO_CHAR, // 0x55 = nothing
        NO_CHAR, // 0x56 = nothing
        NO_CHAR, // 0x57 = nothing
        NO_CHAR, // 0x58 = nothing
        NO_CHAR, // 0x59 = nothing
        NO_CHAR, // 0x5a = nothing
        NO_CHAR, // 0x5b = nothing
        NO_CHAR, // 0x5c = nothing
        NO_CHAR, // 0x5d = nothing
        NO_CHAR, // 0x5e = nothing
        NO_CHAR, // 0x5f = nothing
	}; /* end uppercase table */
      
  } /* end class driver.KI */
