#include <msp430.h>
    int count, upndown, increment;
    int Counter = 1000;
    int up = 1;
    int main(void)
    {
      WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
      upndown = 1;
      count = 0;
      // increment = 10;
      increment = 1000;
      P2DIR |= BIT1;                            //Turns On Red color LED
      P2SEL |= BIT1;
      P2SEL2 &= ~BIT1;


      P1REN |= BIT3;
      P1OUT |= BIT3;
      P1IE |= BIT3;
      //P1IES |= BIT3;
      P1IFG &= ~BIT3;

      //Timer0_A Set-up
      //TA0CCTL0 = CCIE;                             // Enable the Interrupt
      // TA0CCR0 = 1000;
      // TA0CTL = TASSEL_2 + MC_2;                  // SMCLK, continuous mode


      //Timer1_A Set-Up
      TA1CCR0  = 10000;                          //Timer1 CCR0
      TA1CCTL1 |= OUTMOD_7;                     //reset the timer
      TA1CCR1  = count;                             //Timer1 CCR1
      TA1CTL   |= TASSEL_2 + MC_1;                  //SMCLK, Up mode

      __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
      while(1);
    }


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    // TA1CCR1 += 1000;

    if(upndown) {
         count = count + increment;
              if (count >= 10000) {
                    upndown = 0;
              }
     }
        else {
           count = count - increment;
                if (count <= 10) {
                   upndown = 1;
                }
      }
      _delay_cycles(200000);
      TA1CCR1 = count;

/*    if (TA1CCR1 > 10000)
    {
        TA1CCR1 = 1000;
    }
*/


P1IFG &= ~BIT3;

}



    // Timer A0 interrupt service routine
    // void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
    // {
        // TA0CCR0+= 1000;
        // if(upndown) {
        // count = count + increment;
             // if (count >= 10000) {
                 // upndown = 0;
             // }
        // }
         // else {
         // count = count - increment;
             // if (count <= 10) {
                 // upndown = 1;
             //}
         // }
         //_delay_cycles(200000);
        // TA1CCR1 = count;

            //Reset the TA1CCR1 value
    // }
