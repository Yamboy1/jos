/*
 * KeyListener.java
 * 
 * the interface to JOS key listeners
 */

package jos.platform.event;

public interface KeyListener {
      public void keyTyped( KeyEvent ke );
      public void keyPressed( KeyEvent ke );
      public void keyReleased( KeyEvent ke );
      } /* end interface KeyListener */
