#include <lpc17xx.h>

unsigned int A[8];
unsigned int B[8];

unsigned int i, djostic;
unsigned int i0;

void delay(int count){
	for (i0 = 0; i0 < count; i0++) 
	{
		
	}
}

int main()
{

	SystemInit();
	A[0] = (1<<6);
	A[1] = (1<<5);
	A[2] = (1<<4);
	A[3] = (1<<3);
	A[4] = (1<<2);
	A[5] = (1<<31);
	A[6] = (1<<29);
	A[7] = (1<<28);

	LPC_GPIO2->FIODIR  |= (1<<2) | (1<<3) | (1<<4)  | (1<<5)  | (1<<6) ;  // P2.2 - P2.6 - OUT 
	LPC_GPIO1->FIODIR  |= (1<<28)  | (1<<29)  | (1<<31)                ;  // P1.28, P1.29, P1.31 - OUT 

	while (1)
	 {
	  for (i = 0; i < 8; i++) 
	   {
				djostic = LPC_GPIO1->FIOPIN & (1<<20);
				if(djostic != 0)
				{
					if (i <= 4){ 
						LPC_GPIO2->FIOPIN  = A[i];
						LPC_GPIO1->FIOPIN = 0;
					}	else {
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
				delay(500000);
			}
    }

}