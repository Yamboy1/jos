/* vga.java
 *
 * (from Thomas Bocek's code)
 * vga function library
 */ 

package jos.platform.driver;

public class VGA
{
    /* current state information */
    public int STAT;
    public int current_mode;

/* BEGIN: protected implementation */
/* BEGIN: register-level manipulation */

    /* the value at *_I(ndex) registers
       determines the value at the R(read)
       or W(rite) registers. */

    /* ATTibute registers */
    protected final static int ATT_I=0x3C0;
    protected final static int ATT_R=0x3C1;

	    void outATT(int index, int val)
	    {
	        jos.system.machine.in8((short)STAT);
	        jos.system.machine.out8((short)ATT_I, (byte)index);
	        jos.system.machine.out8((short)ATT_I, (byte)val);
	    }
	    
	    int inATT(int index)
	    {
		    jos.system.machine.in8((short)STAT);
	        jos.system.machine.out8((short)ATT_I, (byte)(index));
	        return ((int)(jos.system.machine.in8((short)ATT_R)));
	    }

    /* MISCellaneous registers */  
    protected final static int MISC_W=0x3C2;
    protected final static int MISC_R=0x3CC;

	    void changeMISC(int val)
	    {
	        outSEQ(0,1);
	        jos.system.machine.out8((short)MISC_W, (byte)val);
	        outSEQ(0,3);
	    }
	    
	    int getMISC()
	    {
	        return ((int)(jos.system.machine.in8((short)MISC_R)));
	    }

    /* SEQuencer registers */
    protected final static int SEQ_I=0x3C4;
    protected final static int SEQ_D=0x3C5;

	    void outSEQ(int index,int val)
	    {
	        jos.system.machine.out8((short)SEQ_I, (byte)index);
	        jos.system.machine.out8((short)SEQ_D, (byte)val);
	    }
	    
	    int inSEQ(int index)
	    {
	        jos.system.machine.out8((short)SEQ_I, (byte)index);
	        return((int) (jos.system.machine.in8((short)SEQ_D)));
	    }

    /* graphics-mode registers? */
    protected final static int GRAF_I=0x3CE;
    protected final static int GRAF_D=0x3CF;

	    void outGRAF(int index,int val)
	    {
	        jos.system.machine.out8((short)GRAF_I,(byte)index);
	        jos.system.machine.out8((short)GRAF_D,(byte)val);
	    }
	    
	    int inGRAF(int index)
	    {
	        jos.system.machine.out8((short)GRAF_I,(byte)index);
	        return((int)(jos.system.machine.in8((short)GRAF_D)));
	    }

    /* CRT Control registers? */
    protected final static int CRTC_I=0x3D4;
    protected final static int CRTC_D=0x3D5;

	    void outCRTC(int index,int val)
	    {
	        jos.system.machine.out8((short)CRTC_I, (byte)index);
	        jos.system.machine.out8((short)CRTC_D, (byte)val);
	    }
	    
	    
	    int inCRTC(int index)
	    {
	        jos.system.machine.out8((short)CRTC_I, (byte)index);
	        return((int)(jos.system.machine.in8((short)CRTC_D)));
	    }

    /* CRT Control Mode registers? */
    protected final static int CRTC_IM=0x3B4;
    protected final static int CRTC_DM=0x3B5;

	    void outCRTCM(int index,int val)
	    {
	        jos.system.machine.out8((short)CRTC_IM, (byte)index);
	        jos.system.machine.out8((short)CRTC_DM, (byte)val);
	    }
	    
	    int inCRTCM(int index)
	    {
	        jos.system.machine.out8((short)CRTC_IM, (byte)index);
	        return((int)(jos.system.machine.in8((short)CRTC_DM)));
	    }

    /* STATus registers */
    protected final static int STATC=0x3DA;
    protected final static int STATM=0x3BA;

    /* VGA Error register? */
    protected final static int VGAE=0x3C3;

/* BEGIN: register-group manipulations */

    void setCRTCRegister(int[] crtreg)
    {
        deprotectCRTC();
        for(int i=0;i<crtreg.length;i++) outCRTC(i,crtreg[i]);
    }
    
    void setCRTCMRegister(int[] crtreg)
    {
        deprotectCRTC();
        for(int i=0;i<crtreg.length;i++) outCRTCM(i,crtreg[i]);
    }
    
    void setGRAFRegister(int[] grareg)
    {
        for(int i=0;i<grareg.length;i++) outGRAF(i,grareg[i]);
    }
    
    void setATTRegister(int[] attreg)
    {
        for(int i=0;i<attreg.length;i++) outATT(i,attreg[i]);
    }
    
    void setSEQRegister(int[] seqreg)
    {
        outSEQ(0,1);
        for(int i=1;i<seqreg.length;i++) outSEQ(i,seqreg[i]);
        outSEQ(0,3);
    }

/* BEGIN: state-handling operations */

    /* old state information */
    protected int[] oldseq=new int[5];
    protected int[] oldcrt=new int[25];
    protected int[] oldgra=new int[9];
    protected int[] oldatt=new int[21];
    protected int oldMISC;

    void saveSEQ()
    {
        for(int i=0;i<oldseq.length;i++) oldseq[i]=(byte) inSEQ(i);
    }
    
    void saveCRTC()
    {
        for(int i=0;i<oldcrt.length;i++)
        {
            if(current_mode==1) oldcrt[i]=(byte) inCRTC(i);
            else oldcrt[i]=(byte) inCRTCM(i);
        }
    }

    void saveATT()
    {
        for(int i=0;i<oldatt.length;i++) oldatt[i]=(byte) inATT(i);
    }
    
    void saveGRAF()
    {
        for(int i=0;i<oldgra.length;i++) oldgra[i]=(byte) inGRAF(i);
    }
    
    void saveMISC()
    {
        oldMISC=getMISC();
    }

/* BEGIN: color operations */
    
    void setpal(int color,int r,int g,int b)
    {
        jos.system.machine.out8((short)0x03C8,(byte)color);
        jos.system.machine.out8((short)0x03C9,(byte)r);
        jos.system.machine.out8((short)0x03C9,(byte)g);
        jos.system.machine.out8((short)0x03C9,(byte)b);
    }
    
    int[] getpal(int color)
    {
        jos.system.machine.out8((short)0x03C7,(byte)color);
        int[] ret=new int[3];
        ret[0]=jos.system.machine.in8((short)0x03C9);
        ret[1]=jos.system.machine.in8((short)0x03C9);
        ret[2]=jos.system.machine.in8((short)0x03C9);
        return ret;
    }

/* BEGIN: high-level operations */

    int getcolormode()
    {
        return (jos.system.machine.in8((short)MISC_R) & 1);
    }

    void setcolormode()
    {
        outSEQ(0,1);
        jos.system.machine.out8((short)MISC_W,(byte)(jos.system.machine.in8((short)MISC_R) | 0x01));
        outSEQ(0,3);
        STAT=STATC;
        current_mode=1;
    }
    
    void setmonomode()
    {
        outSEQ(0,1);
        jos.system.machine.out8((short)MISC_W,(byte)(jos.system.machine.in8((short)MISC_R) & 0xFE));
        outSEQ(0,3);
        STAT=STATM;
        current_mode=0;
    }
        
    void screenoff()
    {
        outSEQ(0,1);
        outSEQ(1,inSEQ(1)|0x20);
        outSEQ(0,3);
        jos.system.machine.in8((short)STAT);
        jos.system.machine.out8((short)ATT_I,(byte)0);
    }
    
    void screenon()
    {
        outSEQ(0,1);
        outSEQ(1,inSEQ(1)&0xDF);
        outSEQ(0,3);
        jos.system.machine.in8((short)STAT);
        jos.system.machine.out8((short)ATT_I,(byte)0x20);
    }
    
    void deprotectCRTC()
    {
        if(current_mode==1) outCRTC(0x11,(inCRTC(0x11)&0x7F));
        else outCRTCM(0x11,(inCRTCM(0x11)&0x7F));
    }
} /* end class vga */
