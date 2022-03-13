#include <msp430.h>
#include <stdint.h>
#include <stdio.h>

void serial_charTX(char c);
char serial_charRX(void);
void serial_string(char string[]);
void serial_number(int value);

const float VCC = 3.31; // Measured voltage of LaunchPad 5V line
const float R_DIV = 3300.0; // Measured resistance of 47k resistor
int volt;
long IntDegF;
long IntDegC;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;         // Stop WDT

  DCOCTL  = 0;                      // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;            // Set MCLK and SMCLK to 1MHz
  DCOCTL  = CALDCO_1MHZ;            // Set MCLK and SMCLK to 1MHz

  P1DIR |= BIT0;
  P1DIR |= BIT6;
                                    ////// Serial port back channel setup

  P1SEL  = BIT1 + BIT2;             // Select UART as the pin function
  P1SEL2 = BIT1 + BIT2;
  UCA0CTL1 |= UCSWRST;              // Disable UART module for configuration
  UCA0CTL0 = 0x00;                  // No parity, LSB first, 8-bit data, 1 stop bit, UART, Asynchronous
  UCA0CTL1 = UCSSEL_2 + UCSWRST;    // SMCLK source, keep in reset state
  UCA0BR0 = 104;                    // 9600 Baud rate   - Assumes 1 MHz clock
  UCA0BR1 = 0;                      // 9600 Baud rate   - Assumes 1 MHz clock
  UCA0MCTL = 0x02;                  // 2nd Stage modulation = 1, Oversampling off
  IE2 = 0x00;                       // Interrupts disabled
  UCA0CTL1 &= ~UCSWRST;
                                    // Analog setup
  ADC10CTL1 = INCH_4 + ADC10DIV_3; // Temperature Sensor and ADC10CLK/4
                                    // ADC set up parameters
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
  __delay_cycles(10000);              // required for settling of ADC

while(1)
  {
  ADC10CTL0 |= ENC + ADC10SC;       // Sampling and conversion start with interrupt
  __bis_SR_register(CPUOFF + GIE);  // LPM0 with interrupts enabled, sleep until interrupt

  volt = ADC10MEM;
  float lightV = (VCC * volt / 1023)*1000;

  // serial_number(volt);
  // serial_string(" ");
  serial_string("Voltage = ");
  serial_number(lightV);           //print value
  serial_string(" mV\n");

  if ((lightV >= 331) && (lightV < 2979)) {
      P1OUT ^= BIT0;
      P1OUT &= ~BIT6;
  }
  else if (lightV >= 2979) {
      P1OUT ^= BIT0;
      P1OUT ^= BIT6;
  }
  else {
      P1OUT &= ~BIT0;
      P1OUT &= ~BIT6;
  }


  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
__bic_SR_register_on_exit(CPUOFF); // Clear CPUOFF bit from 0(SR)
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

void serial_string(char string[])
{
    int i;
    for(i = 0; string[i] != '\0'; i++)  // Send characters until end-of-string
    {
        if(string[i] == '\n')           // The terminal program recognizes \r as the carriage return
            serial_charTX('\r');
        else
            serial_charTX(string[i]);
    }
}

void serial_number(int value)
{
    char string[10];

    sprintf(string, "%d", value);       // Convert an integer to a character string

    serial_string(string);
}
