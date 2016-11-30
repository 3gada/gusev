#include <LPC17xx.h>
#include "GLCD.h"

void init_display() 
{	
  GLCD_Init ();
  GLCD_Clear (White);
  GLCD_SetTextColor (Blue);								  
}
int Convert(unsigned int Data, char * charData)
{
	unsigned int Temp0, Temp1 = Data, i0 = 0, i1;
	char charTemp[30];
	for (i1 = 0; i1 < 20; i1++)
		charData[i1] = 0;
		
	while(Temp1 > 0)
	{
		Temp0 	= Temp1/10;
		charTemp[i0++] 	= Temp1-Temp0*10;
		Temp1	= Temp0;		
	}
	for (i1 = 0; i1 < i0; i1++)
		charData[i1] = 48 + charTemp[i0-1-i1];
	charData[i0] = '\0';
	return i0;
}
void PreparDispl(int Leng, int N, char * charData)
{											   	
	char charTemp[30];
	int i0;
	
	for (i0 = 0; i0 < N; i0++)
		charTemp[Leng-1-i0] = charData[N-1-i0];			 
	for (i0 = 0; i0 < (Leng-N); i0++)
		charTemp[i0] = '0';
	for (i0 = 0; i0 < Leng; i0++)
		charData[i0] = charTemp[i0];
		charData[Leng] = '\0';
}
void GLCD_DisplayU32(unsigned int ln, unsigned int col, unsigned char fi, unsigned int Data)
{
	char U8Data[30], N;
	N = Convert(Data, U8Data);
	PreparDispl(5, N, U8Data);
	GLCD_DisplayString (ln, col, U8Data);
}