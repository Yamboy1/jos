/*
 * KeyEvent.java
 *
 * the JOS KeyEvent
 */

package jos.platform.event;

public class KeyEvent extends Object {
	/* event types */
	public static final int KEY_PRESSED = 0;
	public static final int KEY_RELEASED = 1;
	public static final int KEY_TYPED = 2;
	public static final int KEY_SCANCODE = -1;

	/* VK_* definitions */
	public static final int VK_UNDEFINED = -1;
	public static final char CHAR_UNDEFINED = (char)-1;

	/* These probably don't match Sun's definitions,
	   not that it should matter. */
        public static final int  VK_0 = 48;
        public static final int  VK_1 = 49;
        public static final int  VK_2 = 50;
        public static final int  VK_3 = 51;
        public static final int  VK_4 = 52;
        public static final int  VK_5 = 53;
        public static final int  VK_6 = 54;
        public static final int  VK_7 = 55;
        public static final int  VK_8 = 56;
        public static final int  VK_9 = 57;

        public static final int  VK_A = 97;
        public static final int  VK_B = 98;
        public static final int  VK_C = 99;
        public static final int  VK_D = 100;
        public static final int  VK_E = 101;
        public static final int  VK_F = 102;
        public static final int  VK_G = 103;
        public static final int  VK_H = 104;
        public static final int  VK_I = 105;
        public static final int  VK_J = 106;
        public static final int  VK_K = 107;
        public static final int  VK_L = 108;
        public static final int  VK_M = 109;
        public static final int  VK_N = 110;
        public static final int  VK_O = 111;
        public static final int  VK_P = 112;
        public static final int  VK_Q = 113;
        public static final int  VK_R = 114;
        public static final int  VK_S = 115;
        public static final int  VK_T = 116;
        public static final int  VK_U = 117;
        public static final int  VK_V = 118;
        public static final int  VK_W = 119;
        public static final int  VK_X = 120;
        public static final int  VK_Y = 121;
        public static final int  VK_Z = 122;

        public static final int  VK_ESCAPE = 0x01;
        public static final int  VK_EQUALS = 0x0d;
        public static final int  VK_BACK_SPACE = 0x0e;
        public static final int  VK_TAB = 0x0f;
        public static final int  VK_OPEN_BRACKET = 0x1a;
        public static final int  VK_CLOSE_BRACKET = 0x1b;
        public static final int  VK_ENTER = 0x1c;
        public static final int  VK_SEMICOLON = 0x27;
        public static final int  VK_QUOTE = 0x28;
        public static final int  VK_BACK_QUOTE = 0x29;
        public static final int  VK_BACK_SLASH = 0x2b;
        public static final int  VK_COMMA = 0x33;
        public static final int  VK_PERIOD = 0x34;
        public static final int  VK_SLASH = 0x35;
        public static final int  VK_MULTIPLY = 0x37;
        public static final int  VK_SPACE = 0x39;
        public static final int  VK_CAPS_LOCK = 0x3a;
        public static final int  VK_F1 = 0x3b;
        public static final int  VK_F2 = 0x3c;
        public static final int  VK_F3 = 0x3d;
        public static final int  VK_F4 = 0x3e;
        public static final int  VK_F5 = 0x3f;
        public static final int  VK_F6 = 0x40;
        public static final int  VK_F7 = 0x41;
        public static final int  VK_F8 = 0x42;
        public static final int  VK_F9 = 0x43;
        public static final int  VK_F10 = 0x44;
	/* These weren't orginally here, but it makes my
	   VC code much simpler to have them in counting order. */
        public static final int  VK_F11 = 0x45;
        public static final int  VK_F12 = 0x46;

        public static final int  VK_NUM_LOCK = 0x47;
        public static final int  VK_SCROLL_LOCK = 0x48;
        public static final int  VK_NUMPAD7 = 0x49;
        public static final int  VK_NUMPAD8 = 0x4a;
        public static final int  VK_NUMPAD9 = 0x4b;
        public static final int  VK_SUBTRACT = 0x4c;
        public static final int  VK_NUMPAD4 = 0x4d;
        public static final int  VK_NUMPAD5 = 0x4e;
        public static final int  VK_NUMPAD6 = 0x4f;
        public static final int  VK_ADD = 0x50;
        public static final int  VK_NUMPAD1 = 0x51;
        public static final int  VK_NUMPAD2 = 0x52;
        public static final int  VK_NUMPAD3 = 0x53;
        public static final int  VK_NUMPAD0 = 0x54;
        public static final int  VK_DECIMAL = 0x55;

        public static final int  VK_INSERT = 0x60;
        public static final int  VK_HOME = 0x61;
        public static final int  VK_PAGE_UP = 0x62;
        public static final int  VK_DIVIDE = 0x63;
        public static final int  VK_PRINTSCREEN = 0x64;
        public static final int  VK_DELETE = 0x65;
        public static final int  VK_END = 0x66;
        public static final int  VK_PAGE_DOWN = 0x67;
        public static final int  VK_UP = 0x68;
        public static final int  VK_SEPARATER = 0x69;
        public static final int  VK_FINAL = 0x6d;
        public static final int  VK_LEFT = 0x6f;
        public static final int  VK_DOWN = 0x70;
        public static final int  VK_RIGHT = 0x71;
        public static final int  VK_PAUSE = 0x72;
        public static final int  VK_CONTROL = 0x73;
	public static final int  VK_ALT = 0x72;
        public static final int  VK_SHIFT = 0x74;	

	/* stores the event type */
	protected int type;
	/* stores the scancode */
	protected int scancode;
	/* stores the vk code */
	protected int vkcode;
	/* stores the character */
	protected char character;
	/* stores the flags -- SHIFT/CTRL/ALT/META, for now. */
	public int flags;

	public KeyEvent( int type, int scancode, int vkcode, char character, int flags ) {
		this.type = type;
		this.scancode = scancode;
		this.vkcode = vkcode;
		this.character = character;
		this.flags = flags;
		} /* end scancode constructor */

	public int getScancode() {
		return this.scancode;
		} /* end scancode accessor */

	public int getVKcode() {
		return this.vkcode;
		} /* end VKcode accessor */

	public int getType() {
		return this.type;
		} /* end type accessor */

	public char getChar() {
		return this.character;
		} /* end char accessor */

	public int getFlags() {
		return this.flags;
		} /* end flags accessor */

	} /* end class KeyEvent */
