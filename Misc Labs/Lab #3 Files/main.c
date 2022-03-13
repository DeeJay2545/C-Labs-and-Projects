#include <msp430.h> 

#define TRUE 1==1

#define BUTTON BIT3 // Port 1.3
#define RED_LED BIT0 // Port 1.0
#define GREEN_LED BIT6 // Port 1.6

void main(void) {

// Watchdog setup
    WDTCTL = WDTPW + WDTHOLD; // stop using the watchdog

// LED initial setup
    P1DIR |= RED_LED + GREEN_LED;   // ENABLE GREEN AND RED LED

// Button setup
P1DIR &= ~BUTTON; // button is an input
P1OUT |= BUTTON; // pull-up resistor
P1REN |= BUTTON; // resistor enabled

while (TRUE) {

if (P1IN & BUTTON) {
P1OUT &= ~GREEN_LED;    // Green off
P1OUT |= RED_LED;   // Red on
} else {
P1OUT &= ~RED_LED;  // Red off
P1OUT |= GREEN_LED; // Green on
}

}
}
