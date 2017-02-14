#include <LPC17xx.H>
#include "GLCD.h"
#include "LibraryDisplay.h"

unsigned int i0, tick = 0;
unsigned int A[8];
unsigned int i = 0, djostic, button, value;
unsigned int timing = 1100000, led = 0, step, hody = 0, score;

struct Object
{
    unsigned int x;
    unsigned int y;
};
struct Object zmeyaDel;
struct Object eda;
struct Object rand;

struct MasObject
{
    unsigned int x[1024];
    unsigned int y[1024];
    unsigned int dlina;
};
struct MasObject zmeya;

enum Vector
{
    Up,
    Right,
    Down,
    Left
};
enum Vector vector = Up;

void SysTick_Handler();
void InitADC();
void delay(int);
void port_init();
void systick_init();
void display_color();
void display_line();
void coord_init();
void vector_set(unsigned int);
void game_over();
void init_ShIM();

int main(void)
{
    SystemInit();
    init_display();
    port_init();
    systick_init();
    InitADC();
    display_line();
    display_color();
    coord_init();
    init_ShIM();

    while (1)
    {
        timing = (value * 1000) + 300000;
        //GLCD_DisplayU32(1, 1, 1, value);
        if (led <= 4)
        {
            LPC_GPIO2->FIOPIN = A[led];
            LPC_GPIO1->FIOPIN = 0;
        }
        else
        {
            LPC_GPIO1->FIOPIN = A[led];
            LPC_GPIO2->FIOPIN = 0;
        }
        switch (vector)
        {
        case Up:
            zmeya.y[hody] = zmeya.y[hody - 1] - step;
            zmeya.x[hody] = zmeya.x[hody - 1];
            break;
        case Right:
            zmeya.x[hody] = zmeya.x[hody - 1] - step;
            zmeya.y[hody] = zmeya.y[hody - 1];
            break;
        case Down:
            zmeya.y[hody] = zmeya.y[hody - 1] + step;
            zmeya.x[hody] = zmeya.x[hody - 1];
            break;
        case Left:
            zmeya.x[hody] = zmeya.x[hody - 1] + step;
            zmeya.y[hody] = zmeya.y[hody - 1];
            break;
        }

        if (zmeya.y[hody] > 225)
            zmeya.y[hody] = 10;
        else if (zmeya.y[hody] < 10)
            zmeya.y[hody] = 225;
        if (zmeya.x[hody] > 305)
            zmeya.x[hody] = 10;
        else if (zmeya.x[hody] < 10)
            zmeya.x[hody] = 305;

        if ((zmeya.x[hody] == eda.x) && (zmeya.y[hody] == eda.y))
        {
            zmeya.dlina++;
            eda.x = rand.x;
            eda.y = rand.y;
            GLCD_Bargraph(eda.x, eda.y, step, step, Purple);
        }

        GLCD_Bargraph(zmeya.x[hody], zmeya.y[hody], step, step, Purple);
        for (i = 1; i < zmeya.dlina; i++)
        {
            if ((zmeya.x[hody] == zmeya.x[hody - i]) && (zmeya.y[hody - i] == zmeya.y[hody]))
            {
                game_over();
            }
        }
        delay(timing);
        GLCD_Bargraph(zmeya.x[hody - zmeya.dlina + 1], zmeya.y[hody - zmeya.dlina + 1], step, step, Blue);
        hody++;
        if (hody > 1023)
        {
            hody = zmeya.dlina;
            for (i = zmeya.dlina; i > 0; i--)
            {
                zmeya.x[zmeya.dlina - i] = zmeya.x[1024 - i];
                zmeya.y[zmeya.dlina - i] = zmeya.y[1024 - i];
            }
        }
    }
}

void SysTick_Handler()
{
    tick++;
    if (tick >= 100)
    {
        tick = 0;
        rand.x += step;
        rand.y += step;
        if (rand.y > 225)
            rand.y = 10;
        if (rand.x > 305)
            rand.x = 10;
        djostic = LPC_GPIO1->FIOPIN;
        vector_set(djostic);
        if ((LPC_ADC->ADGDR & (1 << 31)) != 0)
        {
            value = (LPC_ADC->ADGDR >> 4) & 0x0FFF;
            LPC_ADC->ADCR |= (1 << 24) | (1 << 2);
        }
        /*button = (LPC_GPIO2->FIOPIN)&(1<<10);
        if (button == 0)
        {
            timing -= 100000;
            led++;
            if (timing <= 300000)
            {    
                timing = 1100000;
                led = 0;
            }
        }*/
    }
}

void InitADC(void)
{
    LPC_PINCON->PINSEL1 = (1 << 18);
    LPC_ADC->ADCR = (1 << 2) |
                    (0xf << 12) |
                    (0xf << 8) |
                    (1 << 21) |
                    (1 << 24);
}

void delay(int count)
{
    for (i0 = 0; i0 < count; i0++)
        ;
}

void port_init()
{
    A[0] = (1 << 6);
    A[1] = (1 << 5);
    A[2] = (1 << 4);
    A[3] = (1 << 3);
    A[4] = (1 << 2);
    A[5] = (1 << 31);
    A[6] = (1 << 29);
    A[7] = (1 << 28);
    LPC_GPIO2->FIODIR |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6); // P2.2 - P2.6 - OUT
    LPC_GPIO1->FIODIR |= (1 << 28) | (1 << 29) | (1 << 31);                    // P1.28, P1.29, P1.31 - OUT
}

void systick_init()
{
    SysTick->LOAD = 94000 - 1; /* set reload register */
    SysTick->VAL = (0x00);     /* Load the SysTick Counter Value */
    SysTick->CTRL = (1 << 2) | (1 << 0) | (1 << 1);
}

void display_line()
{
    GLCD_Clear(White);
    GLCD_Bargraph(5, 5, 5, 230, Purple);
    GLCD_Bargraph(5, 5, 310, 5, Purple);
    GLCD_Bargraph(310, 5, 5, 230, Purple);
    GLCD_Bargraph(5, 230, 310, 5, Purple);
}

void display_color()
{
    GLCD_SetTextColor(Navy);
    //GLCD_SetTextColor(Purple);
}

void coord_init()
{
    //    for (i=1; i<1024; i++)
    //    {
    //        zmeya.x[i] = 0;
    //        zmeya.y[i] = 0;
    //    }
    hody = 1;
    zmeya.x[0] = 150;
    zmeya.y[0] = 110;
    zmeya.dlina = 1;

    eda.x = 110;
    eda.y = 120;

    rand.x = 10;
    rand.y = 30;

    score = 0;
    step = 5;
    GLCD_Bargraph(eda.x, eda.y, step, step, Purple);
}

void vector_set(unsigned int dj)
{
    if ((dj & (1 << 23)) == 0)
    {
        if (vector != Down)
            vector = Up;
    }
    if ((dj & (1 << 24)) == 0)
    {
        if (vector != Left)
            vector = Right;
    }
    if ((dj & (1 << 25)) == 0)
    {
        if (vector != Up)
            vector = Down;
    }
    if ((dj & (1 << 26)) == 0)
    {
        if (vector != Right)
            vector = Left;
    }
}

void game_over()
{
    GLCD_DisplayString(4, 5, "GAME OVER");
    while (1)
    {
        djostic = LPC_GPIO1->FIOPIN & (1 << 20);
        if (djostic == 0)
        {
            break;
        }
    }
    display_line();
    coord_init();
}

void init_ShIM()
{
    LPC_PINCON->PINSEL4 |= (0x01 << 4); // P2.2 - out PWM1.3
    //LPC_GPIO2->FIOSET   |= (1<<3)    ;

    LPC_PWM1->PCR |= 0x01 << 11;         //Enable PWM1 3 output
    LPC_PWM1->MCR = 0x00000002;          //On match with timer reset the counter
    LPC_PWM1->PR = 0x00000000;           //Load prescaler
    LPC_PWM1->MR0 = 16000;               //set cycle rate to one second
    LPC_PWM1->MR3 = 2000;                //set duty cycle to 0%
    LPC_PWM1->LER = (1 << 3) | (1 << 0); //enable shadow latch for match 0
    LPC_PWM1->TCR = 0x00000002;          //Reset counter and prescaler
    LPC_PWM1->TCR = 0x00000009;          //enable counter and PWM, release counter from reset
}
