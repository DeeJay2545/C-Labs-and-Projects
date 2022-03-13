#include <msp430.h>
#include <stdint.h>
#include <stdio.h>


void serial_string(char string[]);
void serial_number(int value);
void delay();
void serial_charTX(char c);
char serial_charRX(void);


volatile unsigned int sec=5;
volatile unsigned int min=0;
volatile unsigned int hour=0;
volatile unsigned int count=0;
volatile unsigned int ms_delay=0;
int i;
int weightcheck = 0;
int cycle_counter = 0;
int TOD = 1;


const float VCC = 3.31;         // Measured voltage of LaunchPad 3V line
const float R_DIV = 1000.0;     // Measured resistance of 1k resistor
float measurement;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    DCOCTL  = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;   // Set range
    DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation

    P1DIR |= BIT0;                          // Set-up Green LED
    P1DIR |= BIT6;                          // Set-up Red LED
    P1OUT &= ~BIT0;
    P1OUT &= ~BIT6;


    // Servo Stuff

        P2DIR |= BIT2;                          // P2.2/TA1.1
        P2OUT &= ~BIT2;                         // Clear all outputs P2
        P2SEL |= BIT2;                          // P2.2 select TA1.1 option
        TA1CCR0 = 20000;                        // PWM Period TA1.1 - Approximately 20 ms pulse
        TA1CCR1 = 1500;                         // CCR1 PWM duty cycle - Starts a value of ~90 degrees
        TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
        TA1CTL   = TASSEL_2 + MC_1;             // SMCLK, contmode


    // Timer Stuff
    // P1DIR |= 0x40;
    // P1OUT &= ~0x40;

    TA0CCTL0 = CCIE;                    // CCR0 interrupt enabled
    TA0CCR0 = 1000;                     // 1ms Timer
    TA0CTL = TASSEL_2 + MC_1;           // SMCLK, contmode

    P1SEL  = BIT1 + BIT2;               // Select UART as the pin function
    P1SEL2 = BIT1 + BIT2;
    UCA0CTL1 |= UCSWRST;                // Disable UART module for configuration
    UCA0CTL0 = 0x00;                    // No parity, LSB first, 8-bit data, 1 stop bit, UART, Asynchronous
    UCA0CTL1 = UCSSEL_2 + UCSWRST;      // SMCLK source, keep in reset state
    UCA0BR0 = 104;                      // 9600 Baud rate   - Assumes 1 MHz clock
    UCA0BR1 = 0;                        // 9600 Baud rate   - Assumes 1 MHz clock
    UCA0MCTL = 0x02;                    // 2nd Stage modulation = 1, Oversampling off
    UCA0CTL1 &= ~UCSWRST;               // Enable UART module

    // Sensor Stuff

    DCOCTL  = 0;                      // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;            // Set MCLK and SMCLK to 1MHz
    DCOCTL  = CALDCO_1MHZ;            // Set MCLK and SMCLK to 1MH

    ADC10CTL1 = INCH_4 + ADC10DIV_3; // Pressure Sensor and ADC10CLK/4
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

}

/////////////////////////////////////////////
// Timer A0 interrupt service routine
/////////////////////////////////////////////
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
{

    // Interupt Counter - 1000 cycles ~= 1 sec
        count = count + 1;

      if(count == 1000)  {

                          // Output the count down to food
                          serial_string("                               \r");
                          serial_string("Countdown to food: ");
                          serial_number(hour);
                          serial_string(":");
                          serial_number(min);
                          serial_string(":");
                          serial_number(sec);
                          serial_string(" ");
                          serial_string("\r");

                          // Count down seconds, minutes, and hours
                          sec = sec-1;
                          count = 0;

                          if (sec == -1) {
                              sec = 59;
                              min = min - 1;

                              if (min == -1) {
                                  min = 59;
                                  hour = hour - 1;

                                  if (hour == -1) {
                                      weightcheck = 0;
                                      cycle_counter = 0;

                                      // Output that the food is dispensing
                                      serial_string("                               \r");
                                      serial_string("Dispensing Food!");
                                      serial_string(" ");
                                      serial_string("\r");
                                      P1OUT |= BIT0;      // Blink green LED to signify motors are on

                                      // Enter while loop that turns on the motor
                                      while (weightcheck < 2) {
                                            cycle_counter++;

                                            delay();            // Delay movement of motor so it has time reach position
                                            TA1CCR1 = 500;      // Set motor angle to ~ 0 degrees

                                            // ============================================================= //
                                                    ADC10CTL0 |= ENC + ADC10SC;     // Set-up pressure sensor for measurement
                                                    measurement = ADC10MEM;         // Get measurement from pressure sensor

                                                    // Based on the graph in the datasheet,
                                                    // a 1 k-Ohm resistor would roughly be able to sense
                                                    // the full weight of a dog bowl. Hence, at a measurement value of
                                                    // > 1023, the bowl is likely full.

                                            // ============================================================= //


                                            // First check to see if enough food
                                            // was dispensed. Must pass both checks for
                                            // the motors to turn off
                                            if (measurement > 1000) {
                                                weightcheck++;
                                            }
                                            else {
                                                weightcheck = 0;
                                            }



                                            delay();            // Delay movement of motor so it has time reach position
                                            TA1CCR1 = 2425;      // Set motor angle to ~ 180 degrees

                                            // ============================================================= //
                                            ADC10CTL0 |= ENC + ADC10SC;                         // Set-up pressure sensor for measurement
                                            measurement = ADC10MEM;                             // Get measurement from pressure sensor

                                            // ============================================================= //


                                            // Second check to see if enough food
                                            // was dispensed. Must pass both checks for
                                            // the motors to turn off
                                            if (measurement > 1000) {
                                               weightcheck++;
                                            }
                                            else {
                                                weightcheck = 0;
                                            }



                                            if (weightcheck >= 2) {
                                                // Output the food is done dispensing
                                                serial_string("                               \r");
                                                serial_string("Ready to eat!");
                                                serial_string(" ");
                                                serial_string("\r");

                                                P1OUT &= ~BIT6;
                                                P1OUT |= BIT0;      // Show that food is ready if user had to refill machine
                                                delay();
                                            }
                                            else if (cycle_counter >= 10) {
                                                // Assuming a cycle count over ten means the machine
                                                // is likely empty and needs to be refilled with food.
                                                P1OUT &= ~BIT0;
                                                P1OUT |= BIT6;

                                                // Output the machine might need to be refilled
                                                serial_string("                               \r");
                                                serial_string("Machine needs refilled!");
                                                serial_string(" ");
                                                serial_string("\r");
                                            }
                                            else {}

                                         }

                                        // Time of day reset
                                        // TD = 1 = breakfast, TD = 2 = lunch , & TD = 3 = dinner
                                        // Notice, some cycles are longer than others. Hence, time between
                                        // dinner and breakfast my be much longer than breakfast to lunch.
                                        // The following are arbitrary values and can be set to whatever the user would like.

                                        if (TOD == 1) {
                                            hour = 0;
                                            min = 1;
                                            sec = 0;
                                            TOD = 2;
                                        }
                                        else if (TOD == 2) {
                                            hour = 0;
                                            min = 2;
                                            sec = 0;
                                            TOD = 3;
                                        }
                                        else {
                                            hour = 0;
                                            min = 5;
                                            sec = 0;
                                            TOD = 1;
                                        }

                                   P1OUT &= ~BIT0;
                                  }
                                  else {}

                              }
                              else {}
                          }
                          else {}
                     }
              else {}
  }

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
__bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit from 0(SR)
}

void serial_string(char string[])
{
    int i;
    for(i = 0; string[i] != '\0'; i++)      // Send characters until end-of-string
    {
        while( !(IFG2 & UCA0TXIFG) );       // Wait until the transmit buffer is empty
        UCA0TXBUF = string[i];                      // Send the character through the Xmit buffer
    }
}

void serial_number(int value)
    {
        char string[10];
        sprintf(string, "%d", value);       // Convert an integer to a character string
        serial_string(string);
    }

void delay()
{
    volatile unsigned long motor_delay = 100000;
    while (motor_delay != 0) {
           motor_delay = motor_delay - 1;          // Motor Delay
     }

}

// The following functions allow us to write strings and numbers to our PC through the back channel uart.
void serial_charTX(char c)
{
    while( !(IFG2 & UCA0TXIFG) );       // Wait until the transmit buffer is empty
    UCA0TXBUF = c;                      // Send the character through the Xmit buffer
}

char serial_charRX(void)
{
    while( !(IFG2 & UCA0RXIFG) );       // Wait until a character has been received
    return UCA0RXBUF;                   // Return received character
}
