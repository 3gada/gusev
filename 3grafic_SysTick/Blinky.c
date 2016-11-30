#include <LPC17xx.H>
#include "GLCD.h"
#include "LibraryDisplay.h"

unsigned int data = 0;
unsigned int data_ready = 0;
unsigned int i0, tick;
unsigned int A[8], B[3], C[3];
unsigned int i=0, j, ii=0, djostic, dj, K, N, M, CCLK, x=150, y=110;

void SysTick_Handler( void )
{
    tick = tick + 1 ;
    if (tick >= 500)
    {
        tick = 0 ;
        ii++;
        if (ii >= 8)
        {
            ii = 0 ;
        }
    }
}

void delay(int count){
	for (i0 = 0; i0 < count; i0++);
}

void port_init(){
    LPC_GPIO2->FIODIR  |= (1<<2) | (1<<3) | (1<<4)  | (1<<5)  | (1<<6);   // P2.2 - P2.6 - OUT 
	LPC_GPIO1->FIODIR  |= (1<<28)  | (1<<29)  | (1<<31);                  // P1.28, P1.29, P1.31 - OUT
}

void systick_init(){
    SysTick->LOAD = 94000 - 1;      /* set reload register */
    SysTick->VAL = (0x00);          /* Load the SysTick Counter Value */
    SysTick->CTRL = (1 << 2)|(1<<0)|(1<<1);
}

void display_line(){
	GLCD_Bargraph(5, 5, 5, 230, Purple);
	GLCD_Bargraph(5, 5, 310, 5, Purple);	
	GLCD_Bargraph(310, 5, 5, 230, Purple);
	GLCD_Bargraph(5, 230, 310, 5, Purple);
}

void display_color(){
	GLCD_SetTextColor(Navy);
	//GLCD_SetTextColor(Purple);
}

void display_const(){
	GLCD_DisplayString (1, 1, "Display data:");
	GLCD_DisplayString (2, 1, "LED:");
	GLCD_DisplayString (3, 3, "K=");
	GLCD_DisplayString (4, 3, "N=");
	GLCD_DisplayString (5, 3, "M=");
	GLCD_DisplayString (6, 3, "CCLK=   MHz");
}

int main (void)
{
	SystemInit ();	
	init_display();
	
	A[0] = (1<<6);
	A[1] = (1<<5);
	A[2] = (1<<4);
	A[3] = (1<<3);
	A[4] = (1<<2);
	A[5] = (1<<31);
	A[6] = (1<<29);
	A[7] = (1<<28);

	port_init();
	
    systick_init();
	
	display_line();
	display_color();
	display_const();
    
	K = (LPC_SC->CCLKCFG & 0xFF)%10 + 1;
	N = (((LPC_SC->PLL0CFG >> 16) & 0xFF)%10) + 1;
	M = ((LPC_SC->PLL0CFG & 0x7FFF) + 1) ;
	
	B[2] = M%10;
	B[1] = M/10%10;
	B[0] = M/100%10;
	
	CCLK = 2*M*12/N/K;
	
	C[2] = CCLK%10;
	C[1] = CCLK/10%10;
	C[0] = CCLK/100%10;
		
	GLCD_DisplayChar (3, 5, 0x30+K);
	GLCD_DisplayChar (4, 5, 0x30+N);
    
	for (j=0; j<3; j++)
    {
        GLCD_DisplayChar (5, 5+j, 0x30+B[j]);
	}
    for (j=0; j<3; j++)
    {
        GLCD_DisplayChar (6, 8+j, 0x30+C[j]);
	}
    
    while (1) 
    {                 
        if (i!=ii){        
            i=ii;
            djostic = LPC_GPIO1->FIOPIN & (1<<20);
            if(djostic != 0)
            {
                if (i <= 4){ 
                    LPC_GPIO2->FIOPIN  = A[i];
                    LPC_GPIO1->FIOPIN = 0;
                } else {
                    LPC_GPIO1->FIOPIN  = A[i];
                    LPC_GPIO2->FIOPIN = 0;
                }
            }
            else{
                if (i <= 4){ 
                    LPC_GPIO2->FIOPIN  = ~A[i];
                    LPC_GPIO1->FIOPIN = ~0;
                }	else {
                    LPC_GPIO1->FIOPIN  = ~A[i];
                    LPC_GPIO2->FIOPIN = ~0;
                    
                }
            }
            GLCD_DisplayChar (2, 5, 0x31+i);
        }
    }
}
 