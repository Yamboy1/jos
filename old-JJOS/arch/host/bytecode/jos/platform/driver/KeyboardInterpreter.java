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
      /* This can be faked pretty well if we're using curses,
 	 but save that for later. */
      dispatchKeyEvent( new KeyEvent( KeyEvent.KEY_TYPED, 0, KeyEvent.VK_UNDEFINED, (char)scancode, 0 ));
      } /* end handleScancode() */

  } /* end class driver.KI */
