#include <LPC17xx.H>
#include "GLCD.h"
#include "LibraryDisplay.h"
#include "math.h"

unsigned int data=0, data1=0;
unsigned int i0, tick=0, switching=0, step=10;;
unsigned int A[8], B[3], C[3];
unsigned int i=0, j, ii=0, ij=0, value=0;
float x=0;
const float pi = 3.141592653589793;


void SysTick_Handler();
void delay(int);
void port_init();
void systick_init();
void display_line();
void InitDAC();

int main (void)
{
	SystemInit ();	
	init_display();
	port_init();
    systick_init();    
	display_line();
    InitDAC();
        
    while (1) 
    {
        if (j!=ij){
            j=ij;
            x++;
            if (x >= step) {
                x = 0;
            }
            data = 512*sin(2*pi/step*x)+512;
            data1 = ((data & 0x3FF) << 6);
            LPC_DAC->DACR = data1;
        }
    }
}
 

void SysTick_Handler( void )
{
    tick++;
    if (tick >= 500)
    {
        tick = 0 ;
        ij++;
        if (ij >= 8)
        {
            ij = 0 ;
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



void display_line(){
	GLCD_Bargraph(5, 5, 5, 230, Purple);
	GLCD_Bargraph(5, 5, 310, 5, Purple);	
	GLCD_Bargraph(310, 5, 5, 230, Purple);
	GLCD_Bargraph(5, 230, 310, 5, Purple);
}


void InitDAC()
{
    LPC_PINCON->PINSEL1 = (0x10<<20); //Set DAC output
    //LPC_DAC->DACR |= (0 << 16);
    LPC_DAC->DACR |= ((data & 0x3FF) << 6);
}