//***************************************************************************************
//  MSP430 Blink the LED Demo - Software Toggle P1.0 and P1.6
//  Description; Toggle P1.0 by xor'ing P1.0 inside of a software loop.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//                MSP430
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED1
//            |             P1.6|-->LED2
//***************************************************************************************

#include <msp430g2553.h>
int main(void)

{
  WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
  P1DIR |= 0x41;                                   // Set P1.0 and P1.6 to outputs
  P1REN = 0x08;         // Turn on pull up resistor to push button
  P1OUT = 0x49;         // Drive high voltages to both LEDs
while (1)                  // endless while loop - condition is always true (one)
  {
volatile unsigned int i;       // volatile to prevent optimization

P1OUT ^= 0x41;                  // Toggle P1.0 and P1.6 using exclusive-OR
for(i=0;i<20000;i++) {}    // empty for loop for delay - change number
  }
}
