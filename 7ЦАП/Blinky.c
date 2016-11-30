#include <LPC17xx.H>
#include "GLCD.h"
#include "LibraryDisplay.h"
#include <math.h>

const double pi = 3.141592653589793;
unsigned int data = 0, data1=0;
unsigned int tick=0, switching=0, step=10;
unsigned int A[8], B[3], C[3];
unsigned int i=0, j=0, ii=0, ij=0, CCLK, value=0;
double x=0;

void TIMER2_IRQHandler();
void SysTick_Handler();
void TIMER01_IRQHandler();
void delay(int);
void port_init();
void TIMER01_init();
void TIMER2_init();
void TIMER0_init();
void systick_init();
void display_line();
void display_color();
void ShIM_init();
void InitADC ();
void InitDAC();

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
	
    //TIMER01_init();
    TIMER0_init();	
    TIMER2_init();
    systick_init();
    ShIM_init();
    //InitADC();
    InitDAC();
     
	display_line();
	display_color();
        
    while (1) 
    {   
        if (i!=ii){        
            i=ii;
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
 
void TIMER2_IRQHandler( void )           //18: Timer2
{
    LPC_TIM2->IR = 0x01;  //clear the MR0 interrupt flag
    switching++ ;
    if (switching >= 1000)
    {
        switching = 0;
        ii++ ;
        if (ii >= 8)
        {
            ii = 0 ;
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
/*
void TIMER01_IRQHandler( void )           //18: Timer0
{
    LPC_TIM0->IR = 0x01;  //clear the MR0 interrupt flag
    switching++ ;
    if (switching >= 1000)
    {
        switching = 0;
        ii++ ;
        if (ii >= 8)
        {
            ii = 0 ;
        }
    }
}
*/
void delay(int count){
    unsigned int i0;
	for (i0 = 0; i0 < count; i0++);
}

void port_init(){
    LPC_GPIO2->FIODIR  |= (1<<2) | (1<<3) | (1<<4)  | (1<<5)  | (1<<6);   // P2.2 - P2.6 - OUT 
	LPC_GPIO1->FIODIR  |= (1<<28)  | (1<<29)  | (1<<31);                  // P1.28, P1.29, P1.31 - OUT
}

void TIMER01_init(){
	// Program Timer 0 - Match Register 0 
	LPC_TIM0->CTCR = 0x0;							// mode - timer
	LPC_TIM0->PR = 0x0;                           // prescaler = 0, every clock
	LPC_TIM0->TCR = 0x2;							// Disable
	LPC_TIM0->MR0 = 9400-1; 		            //10000 ; //202-1;
	LPC_TIM0->MCR = 0x03;		 			    // interrup and Reset on Match MR0
	//LPC_TIM0->EMR = 0x31; 		            // Toogle pin MAT0.0 P1.28,

	//NVIC_SetPriorityGrouping( 4 );  		    //sets PRIGROUP to 3:2 (XXX:YY)
	NVIC_SetPriority( TIMER0_IRQn, 2 );  	    //000:00 (bit 7:3) assign eint0 to group 0, sub-priority 0 within group 0
	NVIC_EnableIRQ( TIMER0_IRQn );

	LPC_TIM0->TCR = 0x1;							// Enable timer 0

	//NVIC_DisableIRQ( TIMER0_IRQn );
}


void TIMER2_init(){
	// Program Timer 2 - Match Register 0 
	//LPC_TIM2->CTCR = 0;							// mode - timer
	LPC_TIM2->TCR = 0x2;							// Disable
    LPC_TIM2->PR = 0x0;                           // prescaler = 0, every clock
	LPC_TIM2->MCR = 0x03;		 			    // interrup and Reset on Match MR1
    LPC_TIM2->MR0 = 12600-1; 		            //10000 ; //202-1;
	//LPC_TIM2->EMR = 0x31; 		            // Toogle pin MAT0.0 P1.28,

	//NVIC_SetPriority( TIMER2_IRQn, 2 );  	    //000:00 (bit 7:3) assign eint0 to group 0, sub-priority 0 within group 0
	NVIC_EnableIRQ( TIMER2_IRQn );

	LPC_TIM2->TCR = 0x1;							// Enable timer 0

	//NVIC_DisableIRQ( TIMER2_IRQn );
}


void TIMER0_init(){
	// Program Timer 0 - Match Register 0 
    LPC_PINCON->PINSEL3 |= 0xC000000;
	LPC_TIM0->CTCR = 0x0;							// mode - timer
    LPC_TIM0->TCR = 0x2;							// Disable
	LPC_TIM0->PR = 0x0;                           // prescaler = 0, every clock
	LPC_TIM0->MR1 = 9400000-1; 		            //10000 ; //202-1;
	LPC_TIM0->MCR = 0x10;		 			    // interrup and Reset on Match MR0
	LPC_TIM0->EMR = 0xC2; 		            // Toogle pin MAT0.0 P1.28,

	//NVIC_SetPriority( TIMER0_IRQn, 1 );  	    //000:00 (bit 7:3) assign eint0 to group 0, sub-priority 0 within group 0
	//NVIC_EnableIRQ( TIMER0_IRQn );
	LPC_TIM0->TCR = 0x1;							// Enable timer 0

	//NVIC_DisableIRQ( TIMER0_IRQn );
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

void ShIM_init(){
    LPC_PINCON->PINSEL4 |= (0x01<<8) ; // P2.4 - out PWM1.5
    LPC_PWM1->PCR |=  0x01<<13 ;	//Enable PWM1 3 output
    LPC_PWM1->MCR = 0x00000002 ;	//On match with timer reset the counter
    LPC_PWM1->PR  = 0x00000000 ;	//Load prescaler
    LPC_PWM1->MR0 = 16000      ;    //set cycle rate to one second
    LPC_PWM1->MR5 = 4*value;	        //set duty cycle to 0%
    LPC_PWM1->LER = (1<<3)|(1<<0) ;	//enable shadow latch for match 0 
    LPC_PWM1->TCR = 0x00000002 ;	//Reset counter and prescaler 
    LPC_PWM1->TCR = 0x00000009 ;	//enable counter and PWM, release counter from reset

}

void InitADC (void)
{
    LPC_PINCON->PINSEL1 = (1<<18);
    LPC_ADC->ADCR = (1 << 2) |
    (0xf << 12) |
    (0xf << 8) |
    (1 << 21) |
    (1 << 24) ;
}

void InitDAC()
{
    LPC_PINCON->PINSEL1 = (0x10<<20); //Set DAC output
    LPC_DAC->DACR |= (0 << 16);
    LPC_DAC->DACR |= ((data & 0x3FF) << 6);
}
