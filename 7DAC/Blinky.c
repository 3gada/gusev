#include <LPC17xx.H>
#include "GLCD.h"
#include "LibraryDisplay.h"
#include "math.h"

unsigned int data=0, data1=0;
unsigned int i=0, i0, tick=0, step=30;
unsigned int Fd=15; //kHz
float x=0;
const float pi = 3.141592653589793;


void SysTick_Handler();
void systick_init();
void delay(int);
void port_init();
void InitDAC();

int main (void)
{
	SystemInit ();	
	port_init();
    systick_init();    
    InitDAC();
        
    while (1) 
    {
        if (tick!=0){
            tick=0;
            x++;
            if (x >= step) {
                x = 0;
                LPC_GPIO1->FIOPIN ^= (1<<28);
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
}

void systick_init(){
    SysTick->LOAD = 94000/Fd - 1;      /* set reload register */
    SysTick->VAL = (0x00);          /* Load the SysTick Counter Value */
    SysTick->CTRL = (1 << 2)|(1<<0)|(1<<1);
}

void delay(int count){
	for (i0 = 0; i0 < count; i0++);
}

void port_init(){
    LPC_GPIO2->FIODIR  |= (1<<2) | (1<<3) | (1<<4)  | (1<<5)  | (1<<6);   // P2.2 - P2.6 - OUT 
	LPC_GPIO1->FIODIR  |= (1<<28)  | (1<<29)  | (1<<31);                  // P1.28, P1.29, P1.31 - OUT
}

void InitDAC()
{
    LPC_PINCON->PINSEL1 = (0x2<<20); //Set DAC output
    LPC_DAC->DACR |= (0 << 16);
}