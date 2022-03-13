#include <msp430.h>


/**
 * blink.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    P1DIR |= 0x01; // configure LED Output

    P1DIR &= ~BIT3; // button is an input
    P1OUT |= BIT3;
    P1REN |= BIT3; // pull-up resistor resistor enabled

    volatile unsigned int i; // volatile to prevent optimization
    volatile unsigned int count;

    while(1)
    {
        if (P1IN & BIT3) {
               P1OUT ^= 0x01; // toggle light
               for(i=50000; i>0; i--); // delay
            }
        else {
            P1OUT ^= 0x01; // turn off
            for(i=5000; i>0; i--); // delay
        }
    }
}
