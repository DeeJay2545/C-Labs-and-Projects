#include <msp430.h> 


void main(void) {

// Watchdog setup
    WDTCTL = WDTPW + WDTHOLD; // stop using the watchdog

// LED initial setup
    P1DIR |= BIT0;  // ENABLE GREEN AND RED LED

// Button setup
P1DIR &= ~BIT3; // button is an input
P1OUT |= BIT3; // pull-up resistor
P1REN |= BIT3; // resistor enabled

while (1) {

    if (!(P1IN & BIT3)) {
        P1OUT ^= BIT0;
        }
    else {
        P1OUT &= ~BIT0;
        }
    }
}
