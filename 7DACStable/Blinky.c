#include <LPC17xx.H>
#define SYSTICK_DELAY 48000
#include "math.h"
 
void SysTick_Handler ( void );
 
int i;
float x =0.0;
int tick = 0, step=25;
unsigned int value;
unsigned int value1;
const float pi=3.141592653589793;
 
void SysTick_Handler( void)  
{
    tick = tick+ 1 ;
    if( tick >= 1)
    {
        tick = 0 ;
        if (tick==0)
        {
            LPC_DAC->DACR = (value1<< 6);
            /*
            value1 = (sin(x))*512+512;
            if (value1 > 1024)
            {
                value1 = 0;
            }
            x=x+0.1*3.14;  */
            x=x+1;
            if (x >= step) {
                x = 0;
            }
            value1 = sin(2*pi/step*x)*512+512;
        }
    }
}
int main (void)
{
 
    SystemInit();
    LPC_PINCON->PINSEL3 |=(3<<24);
 
    SysTick->LOAD  =  SYSTICK_DELAY -1;
    SysTick->VAL   =  (0x00);  
    SysTick->CTRL = (1 << 2)|(1<<0)|(1<<1);
    LPC_PINCON->PINSEL1  |=(1<<21);  //DAC
   
 
    while (1)
    {}
}

