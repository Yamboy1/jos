/* standardVGA.java
 *
 * (from Thomas Bocek's code,
 * (with help from Jan Kayatz for his shifting functions)
 *
 * interface to a standard VGA console. 
 */

package jos.platform.driver;

public class standardVGA extends VGA
{
    /* The SEQuencer, CRT, GRA, and ATTribute register sets
     * for 640x480x16 color mode. */
    protected final static int[] seq ={0x03,0x01,0x0F,0x00,0x06};
    protected final static int[] crt ={0x5F,0x4F,0x50,0x82,0x54,0x80,0x0B,0x3E,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,
                      0xEA,0x8C,0xDF,0x28,0x00,0xE7,0x04,0xE3,0xFF};
    protected final static int[] gra ={0x00,0x0F,0x00,0x00,0x00,0x00,0x05,0x0F,0xFF};
    protected final static int[] att ={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	              0x01,0x00,0x0F,0x00,0x00};

    /* The old and the new palettes. */
    protected int[] oldpal=new int[768];
        
    protected static final int Pal[] = {
    0,  0,  0,  0,  0, 42,  0, 42,  0,  0, 42, 42, 42,  0,  0, 42,  0, 42,
    42, 21,  0, 42, 42, 42, 21, 21, 21, 21, 21, 63, 21, 63, 21, 21, 63, 63,
    63, 21, 21, 63, 21, 63, 63, 63, 21, 63, 63, 63,  0,  0,  0,  5,  5,  5,
    8,  8,  8, 11, 11, 11, 14, 14, 14, 17, 17, 17, 20, 20, 20, 24, 24, 24,
    28, 28, 28, 32, 32, 32, 36, 36, 36, 40, 40, 40, 45, 45, 45, 50, 50, 50,
    56, 56, 56, 63, 63, 63,  0,  0, 63, 16,  0, 63, 31,  0, 63, 47,  0, 63,
    63,  0, 63, 63,  0, 47, 63,  0, 31, 63,  0, 16, 63,  0,  0, 63, 16,  0,
    63, 31,  0, 63, 47,  0, 63, 63,  0, 47, 63,  0, 31, 63,  0, 16, 63,  0,
    0, 63,  0,  0, 63, 16,  0, 63, 31,  0, 63, 47,  0, 63, 63,  0, 47, 63,
    0, 31, 63,  0, 16, 63, 31, 31, 63, 39, 31, 63, 47, 31, 63, 55, 31, 63,
    63, 31, 63, 63, 31, 55, 63, 31, 47, 63, 31, 39, 63, 31, 31, 63, 39, 31,
    63, 47, 31, 63, 55, 31, 63, 63, 31, 55, 63, 31, 47, 63, 31, 39, 63, 31,
    31, 63, 31, 31, 63, 39, 31, 63, 47, 31, 63, 55, 31, 63, 63, 31, 55, 63,
    31, 47, 63, 31, 39, 63, 45, 45, 63, 49, 45, 63, 54, 45, 63, 58, 45, 63,
    63, 45, 63, 63, 45, 58, 63, 45, 54, 63, 45, 49, 63, 45, 45, 63, 49, 45,
    63, 54, 45, 63, 58, 45, 63, 63, 45, 58, 63, 45, 54, 63, 45, 49, 63, 45,
    45, 63, 45, 45, 63, 49, 45, 63, 54, 45, 63, 58, 45, 63, 63, 45, 58, 63,
    45, 54, 63, 45, 49, 63,  0,  0, 28,  7,  0, 28, 14,  0, 28, 21,  0, 28,
    28,  0, 28, 28,  0, 21, 28,  0, 14, 28,  0,  7, 28,  0,  0, 28,  7,  0,
    28, 14,  0, 28, 21,  0, 28, 28,  0, 21, 28,  0, 14, 28,  0,  7, 28,  0,
    0, 28,  0,  0, 28,  7,  0, 28, 14,  0, 28, 21,  0, 28, 28,  0, 21, 28,
    0, 14, 28,  0,  7, 28, 14, 14, 28, 17, 14, 28, 21, 14, 28, 24, 14, 28,
    28, 14, 28, 28, 14, 24, 28, 14, 21, 28, 14, 17, 28, 14, 14, 28, 17, 14,
    28, 21, 14, 28, 24, 14, 28, 28, 14, 24, 28, 14, 21, 28, 14, 17, 28, 14,
    14, 28, 14, 14, 28, 17, 14, 28, 21, 14, 28, 24, 14, 28, 28, 14, 24, 28,
    14, 21, 28, 14, 17, 28, 20, 20, 28, 22, 20, 28, 24, 20, 28, 26, 20, 28,
    28, 20, 28, 28, 20, 26, 28, 20, 24, 28, 20, 22, 28, 20, 20, 28, 22, 20,
    28, 24, 20, 28, 26, 20, 28, 28, 20, 26, 28, 20, 24, 28, 20, 22, 28, 20,
    20, 28, 20, 20, 28, 22, 20, 28, 24, 20, 28, 26, 20, 28, 28, 20, 26, 28,
    20, 24, 28, 20, 22, 28,  0,  0, 16,  4,  0, 16,  8,  0, 16, 12,  0, 16,
    16,  0, 16, 16,  0, 12, 16,  0,  8, 16,  0,  4, 16,  0,  0, 16,  4,  0,
    16,  8,  0, 16, 12,  0, 16, 16,  0, 12, 16,  0,  8, 16,  0,  4, 16,  0,
    0, 16,  0,  0, 16,  4,  0, 16,  8,  0, 16, 12,  0, 16, 16,  0, 12, 16,
    0,  8, 16,  0,  4, 16,  8,  8, 16, 10,  8, 16, 12,  8, 16, 14,  8, 16,
    16,  8, 16, 16,  8, 14, 16,  8, 12, 16,  8, 10, 16,  8,  8, 16, 10,  8,
    16, 12,  8, 16, 14,  8, 16, 16,  8, 14, 16,  8, 12, 16,  8, 10, 16,  8,
    8, 16,  8,  8, 16, 10,  8, 16, 12,  8, 16, 14,  8, 16, 16,  8, 14, 16,
    8, 12, 16,  8, 10, 16, 11, 11, 16, 12, 11, 16, 13, 11, 16, 15, 11, 16,
    16, 11, 16, 16, 11, 15, 16, 11, 13, 16, 11, 12, 16, 11, 11, 16, 12, 11,
    16, 13, 11, 16, 15, 11, 16, 16, 11, 15, 16, 11, 13, 16, 11, 12, 16, 11,
    11, 16, 11, 11, 16, 12, 11, 16, 13, 11, 16, 15, 11, 16, 16, 11, 15, 16,
    11, 13, 16, 11, 12, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0, 63, 63, 63
    };

    /* the remainder of the old state information */
    protected int old_mode;
    protected int[] oldfonts1 = new int[0x9600];
    protected int[] oldfonts2 = new int[0x9600];
    protected int[] oldfonts3 = new int[0x9600];
    protected int[] oldfonts4 = new int[0x9600];

/* BEGIN: public interface */

    public int setVideoMode(int width,int height,int depth) {
		if (width == 640 && height == 480 && depth == 4) { return 0; }
		else { /* unsupported */ 
		  return 1;
		  }
		} /* end setVideoMode() */

	public int[] getModes() {
		int[] modes = {640,480,4};
		return modes;
        } /* end setVideoMode() */
    
    public void startService() {
		deprotectCRTC();
		old_mode=getcolormode();
		current_mode=old_mode;
		if(current_mode==0) STAT=STATM;
		else STAT=STATC;
		screenoff();
		saveoldmode();
		savepal();
		setcolormode();
		setpal16();
		setmode();
		savefont();
		screenon();
		cls();
        } /* end startService() */

    public void stopService() {
		if (current_mode == 0) { STAT=STATM; }
		else { STAT = STATC; }
		
		screenoff();
		setfont();
		
		if (old_mode == 0) { setmonomode(); }
	        else { setcolormode(); }

		restorepal( oldpal );
		setoldmode();
		screenon();
	    } /* end stopService() */

    public void drawPixel(int x1, int y1, int col) {
		int adr = 0xa0000;
		
		outGRAF(0, col);
		outGRAF(8, (0x80 >> (x1 & 7)));
		
		int offset = y1*80 + (x1>>3);
		int k = jos.system.machine.read8(adr + offset);
		jos.system.machine.write8(adr + offset, (byte)0xFF);	
    } /* end drawPixel() */

    public void bitblt(int x1, int y1, int[] array, int xlength) {
		outGRAF(1,0);
		outGRAF(8,0xFF);
		int exp=array.length/xlength;
		int exp2=divroundup(xlength,8);
		int arlength=exp*exp2;
		int[] plane0=new int[arlength],plane1=new int[arlength],plane2=new int[arlength],plane3=new int[arlength];
	        int plan0val=0,plan1val=0,plan2val=0,plan3val=0,count=0,loop=0;
		for(int i=0;i<exp;i++)
	        {
		    for(int k=0;k<exp2;k++)
	            {
	                if((xlength/((k+1)*8))==0) loop=xlength%8;
	                else loop=8;
			    for(int j=0;j<loop;j++)
	                {
	                    int arindex=0;
	                    if(i!=0) arindex=((i*xlength)+(k*8)+j);
	                    else arindex=((k*8)+j);
	                    int div=1<<j;
			    if((array[arindex]&0x8)==8) plan0val+=0x80/div;
			    if((array[arindex]&0x4)==4) plan1val+=0x80/div;
	                    if((array[arindex]&0x2)==2) plan2val+=0x80/div;
	                    if((array[arindex]&0x1)==1) plan3val+=0x80/div;
	                }
	                plane0[count]=plan0val;plane1[count]=plan1val;plane2[count]=plan2val;plane3[count]=plan3val;
        			count++;
	                plan0val=0;plan1val=0;plan2val=0;plan3val=0;
	            }
	        }
		outSEQ(2,1); //plane 0
		outGRAF(4,0);
	    bytetomem(plane0,x1,y1,xlength,exp2);
		outSEQ(2,2); //plane 1
		outGRAF(4,1);
		bytetomem(plane1,x1,y1,xlength,exp2);
		outSEQ(2,4); //plane 2
		outGRAF(4,2);
	    bytetomem(plane2,x1,y1,xlength,exp2);
		outSEQ(2,8);//plane 3
		outGRAF(4,3);
	    bytetomem(plane3,x1,y1,xlength,exp2);
		outSEQ(2,0x0F); //restore
		outGRAF(1,0x0F);
        } /* end bitblt() */    
	    
/* BEGIN: private implementation */

    /* save & set the fonts */
    private void savefont()
    {
    /* _Quinn: savefont() w/ memory-mapped arrays ought to be much faster. */
    byte [] vgamem = jos.system.machine.mapByteArray( 0xA0000, 0x9600 );
    
	outGRAF(1,0);
	outGRAF(8,0xFF);
	outSEQ(2,1);
	outGRAF(4,0);
//	for(int i=0;i<0x9600;i++) oldfonts1[i]=jos.system.machine.read8(0xa0000+i);
    for ( int i = 0; i < 0x9600; i ++ ) {
        oldfonts1[i] = vgamem[i];
        }
	outSEQ(2,2);
	outGRAF(4,1);
//	for(int i=0;i<0x9600;i++) oldfonts2[i]=jos.system.machine.read8(0xa0000+i);
    for ( int i = 0; i < 0x9600; i ++ ) {
        oldfonts2[i] = vgamem[i];
        }
	outSEQ(2,4);
	outGRAF(4,2);
//	for(int i=0;i<0x9600;i++) oldfonts3[i]=jos.system.machine.read8(0xa0000+i);
    for ( int i = 0; i < 0x9600; i ++ ) {
        oldfonts3[i] = vgamem[i];
        }
	outSEQ(2,8);
	outGRAF(4,3);
//	for(int i=0;i<0x9600;i++) oldfonts4[i]=jos.system.machine.read8(0xa0000+i);
    for ( int i = 0; i < 0x9600; i ++ ) {
        oldfonts4[i] = vgamem[i];
        }
	outSEQ(2,0x0F); //restore
	outGRAF(1,0x0F);
    }
    
    private void setfont()
    {
    /* _Quinn: setfont() w/ memory-mapped arrays ought to be much faster. */
    byte [] vgamem = jos.system.machine.mapByteArray( 0xA0000, 0x9600 );

	outGRAF(1,0);
	outGRAF(8,0xFF);
	outSEQ(2,1);
	outGRAF(4,0);
//	for(int i=0;i<0x9600;i++) jos.system.machine.write8(0xa0000+i,(short)oldfonts1[i]);
    for ( int i = 0; i < 0x9600; i++ ) {
        vgamem[i] = (byte)oldfonts1[i];
        }
	outSEQ(2,2);
	outGRAF(4,1);
//	for(int i=0;i<0x9600;i++) jos.system.machine.write8(0xa0000+i,(short)oldfonts2[i]);
    for ( int i = 0; i < 0x9600; i++ ) {
        vgamem[i] = (byte)oldfonts2[i];
        }
	outSEQ(2,4);
	outGRAF(4,2);
//	for(int i=0;i<0x9600;i++) jos.system.machine.write8(0xa0000+i,(short)oldfonts3[i]);
    for ( int i = 0; i < 0x9600; i++ ) {
        vgamem[i] = (byte)oldfonts3[i];
        }
	outSEQ(2,8);
	outGRAF(4,3);
//	for(int i=0;i<0x9600;i++) jos.system.machine.write8(0xa0000+i,(short)oldfonts4[i]);
    for ( int i = 0; i < 0x9600; i++ ) {
        vgamem[i] = (byte)oldfonts4[i];
        }
	outSEQ(2,0x0F); //restore
	outGRAF(1,0x0F);
    }
    
    private void saveoldmode()
    {
	saveCRTC();
	saveATT();
	saveGRAF();
	saveSEQ();
	saveMISC();
    }

    private void setoldmode()
    {
	changeMISC(oldMISC);
	setSEQRegister(oldseq);
	if(current_mode==1) setCRTCRegister(oldcrt);
	else setCRTCMRegister(oldcrt);
	setGRAFRegister(oldgra);
	setATTRegister(oldatt);
	jos.system.machine.in8((short)STAT);
    }

    private void setmode()
    {
	changeMISC(0xE3);
	setSEQRegister(seq);
	setCRTCRegister(crt);
	setGRAFRegister(gra);
	setATTRegister(att);
	jos.system.machine.in8((short)STAT);
    }
    
    private int[] shiftarray(int[] array,int xlength,int times)
    {
	int lmask=(1<<times)-1;
	int backshift=8-times;
	int umask=lmask^0xFFFFFFFF;
	int xs=divroundup(xlength+times,8);
	int xold=divroundup(xlength,8);
	int ys=(array.length/xold);
	int[] array2=new int[xs*ys];
	int xwidth=divroundup(xlength,8);
	boolean resized=(xs > xwidth);
	for(int y=0;y<ys;y++)
	{
	    array2[y*xs]|=(array[y*xwidth]&umask)>>times;
	    for(int x=0;x<(xwidth-1);x++)
	    {
		array2[y*xs+x]|=(array[y*xwidth+x]&umask)>>times;
	        array2[(y*xs)+x+1]|=(array[y*xwidth+x]&lmask)<<backshift;
	    }
	    if(resized)
	    {
		array2[y*xs+(xwidth-1)]|=(array[xwidth*(y+1)-1]&umask)>>times;
		array2[(y+1)*xs-1]|=(array[xwidth*(y+1)-1]&lmask)<<backshift;
	    }
	}
	return array2;
    }
    
    private void bytetomem(int[] arr, int x1, int y1, int xlength, int lines)
    {
	int shr=x1%8; //1
	int lmask=(1<<(8-(shr+(xlength%8))))-1;
	int umask=((1<<(8-shr))-1)^0xFF;
	int addr=0xa0000+(x1/8)+(0x50*y1);
	int oldaddr=addr;
	int arl=arr.length;
	if(shr!=0) arr=shiftarray(arr,xlength,shr);
	if(arr.length>arl) lines++;
	
	for(int i=0;i<arr.length;i++)
	{
	    if((i%lines)==0)
	    {
		if(i!=0) addr+=0x50-lines;
		if(shr!=0) arr[i]+=(((int)jos.system.machine.read8(addr+i))&umask);
	    }
	    if((i%lines)==(lines-1)) if(lmask!=255) arr[i]+=(((int)jos.system.machine.read8(addr+i))&lmask);
	}
	addr=oldaddr-0x50+lines;
	int arl2=arr.length;
	//while((jos.system.machine.in8((short)0x3DA)&8)==0);
	for(int i=0;i<arl2;i++)
	{
	    if((i%lines)==0) addr+=0x50-lines;
	    jos.system.machine.write8(addr+i,(byte)arr[i]);
	}
	
    } /* end bytetomem() */
    
    private void setpal16()
    {
	int j=0;
	for(int i=0;i<48;i+=3)
	   {
	    setpal(j,Pal[i],Pal[i+1],Pal[i+2]);
	    j++;
	   }
    }
    
    private void savepal()
    {
	for(int i=0;i<256;i++)
    	{
	    int tmp[]=getpal(i);
	    oldpal[i*3]=tmp[0];
	    oldpal[(i*3)+1]=tmp[1];
	    oldpal[(i*3)+2]=tmp[2];
	   }
    }
    
    private void restorepal(int[] pal)
    {
	for(int i=0;i<256;i++)
     setpal(i,pal[i*3],pal[(i*3)+1],pal[(i*3)+2]);
    }
    
    private void cls() {
        /* _Quinn: changed from write16 to mapped byte array */
		jos.system.machine.out8((short)GRAF_I,(byte)0x08);
		jos.system.machine.out8((short)GRAF_D,(byte)0xFF);

		byte[] vidmem = jos.system.machine.mapByteArray( 0xA0000, 0x9600 );
		for ( int x = 0; x < 0x9600; x++ ) {
	       vidmem[x] = 0;
		   } 
    } /* end cls() */
    
    private int divroundup(int num, int div)
    {
        if(num%div==0)
        {
            return num/div;
        }
        else
        {
            return (num/div)+1;
        }
    }

    public void delay(int len)
    {
        int z=0;
        while(z<len)
        {z++;}
    }    

} /* end class standardVGA */

