    #include <msp430.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include "HAL_board.h"
    #include "HAL_LCD.h"
    #define SPI_FREQ


int main(void)
{
    uint8_t ui8Row = 0;
    uint8_t ui8Column = 0;
    volatile int i;
    int counter = 0;
    int d=1;
    int array_counter=0;

    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    char greeting1[6];

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	LCD_init();
	LCD_clear();

	while(1) {

	    if (counter < 16) {
	        LCD_clear();
	        LCD_setPosition(ui8Row,ui8Column);
	        LCD_writeString(greeting);
	        ui8Column++;
	        counter++;
	    }


	    else if (counter < 21) {
	        ui8Column = 0;

	        while (array_counter < d)
	        {
	            greeting1[array_counter] = greeting[array_counter];
	            array_counter++;
	        }
	            LCD_clear();
	            LCD_setPosition(ui8Row,ui8Column);
	            LCD_writeString(greeting1);

	        if (counter == 20){
	            counter = 0;
	        }
	        else {
	            counter++;
	        }

	        }

	    else {}

	    for (i=30000; i>0; i--);
	}

	return 0;
}




