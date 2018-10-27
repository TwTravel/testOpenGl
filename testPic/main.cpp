#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "c24bitmap.h"
#include "c256bitmap.h"


int main(int argc,char*argv[])
{
	C24BitMap CPic;
	CPic.FormatF(800 , 600);
	int i, j;
	
	for(i=0;i<50;i++)
		for(j=0;j<60;j++)
		{
		  C24PixVal Pix = get_pix_color(CPic,i+20,j+30);
	     *Pix.r = 255;
		 *Pix.g = 0;
		 *Pix.b = 0;
		}
	
	//CPic.Save("dest.bmp");
	
	return 1;
}