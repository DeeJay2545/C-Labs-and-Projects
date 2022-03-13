#include <msp430.h> 

void main(void) {

// Watchdog setup
    WDTCTL = WDTPW + WDTHOLD; // stop using the watchdog

// LED initial setup
    P1DIR |= BIT0 + BIT6;   // ENABLE GREEN AND RED LED

// Button setup
P1DIR &= ~BIT3; // button is an input
P1OUT |= BIT3; // pull-up resistor
P1REN |= BIT3; // resistor enabled

volatile unsigned int i;
int count=0;

while (1) {

    if (P1IN & BIT3) {
           count = 0;
           P1OUT &= ~BIT0;    // Green off
           P1OUT ^= BIT6;   // Red on
           for(i=20000; i>0; i--); // delay
    }
    else {
        if (count < 9) {
            P1OUT &= ~BIT6;  // Red off
            P1OUT ^= BIT0; // Green on
            for(i=20000; i>0; i--); // delay
        }

        else {
            P1OUT &= ~BIT6;
            P1OUT &= ~BIT0; // Green on
            for(i=20000; i>0; i--); // delay// Red off
        }
        count = count+1;
    }

   }
}
