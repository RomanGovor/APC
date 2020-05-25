#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <io.h>
#include <iostream.h>

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

VIDEO  far* screen = (VIDEO far*)MK_FP(0xB800, 0);

void interrupt newInt9(...); 		 // Interrupt function
void interrupt(*oldInt9)(...); 		 // Pointer to an interrupt handler
void indicator(unsigned char mask);	 // Indicator management function
void blinking(void);				 // LED flashing function
void calc(int value);

int quitFlag = 0; 					// Program exit flag
int blinkingON = 0;					// Flag flashing indicators


void main() 
{
	oldInt9 = getvect(0x09);		// Save the pointer to the old handler
	setvect(0x09, newInt9); 		// Change the handler to a new one

	while (!quitFlag) {			    // No exit flag set yet
		if (blinkingON) 			// If the indicator flashing flag is set
			blinking(); 		    // Blinking indicators
	}
	setvect(0x09, oldInt9);			// Restore the old interrupt handler
	return;
}

void interrupt newInt9(...) 		// Interrupt function
{
	unsigned char value = 0;
	oldInt9();
	value = inp(0x60); 				// Get the value from port 60h

	if (value == 0x01)				// Устанавливаем флаг выхода(нажата Esc)
		quitFlag = 1; 		

	if (value == 0x1C && blinkingON == 0) // Press Enter
		blinkingON = 1; 				  // Set Blinking Flag

	//puts("   ");
	//puts((const char*)value);
	// printf("\t%x", value);			// Print value
	calc(value);
	screen->symb = ' ';
	screen->attr = 0x5E;
	screen++;


	outp(0x20, 0x20); 				// Reset Interrupt Controller
}

void calc(int value)
{
	int base = 16;
	if (value == 0)
		return;
	int rem = value % base;
	if (rem <= 9)
	{
		calc(value / base);
		screen->symb = rem + '0';
		screen->attr = 0x5E;
		screen++;
	}
	else
	{
		calc(value / base);
		screen->symb = rem - 10 + 'A';
		screen->attr = 0x5E;
		screen++;
	}
}


void indicator(unsigned char mask) // Indicator management function
{
	int flg = 0;			
	while ((inp(0x64) & 0x02) != 0x00); // While there is no confirmation of the successful completion of the command
	outp(0x60, 0xED);				// Control byte

	for (int i = 9; i > 0; i--)
	{
		if ((inp(0x60) == 0xFA))	// (FA)Byte processed successfully
		{
			flg = 1;
			break;
		}
		delay(600);
	}
	if (flg) 
	{
		while ((inp(0x64) & 0x02) != 0x00);
		outp(0x60, mask);
	}
	delay(100);
}

void blinking()						// LED flashing function
{
	indicator(0x02);                // Turn on Num Lock
	delay(300);

	indicator(0x04);				// Turn on Caps Lock
	delay(300);

	indicator(0x6);					// Turn on Num Lock и Caps Lock
	delay(300);

	indicator(0x00);				// Turn off all indicators
	delay(100);

	indicator(0x06);				// Turn on all indicators
	delay(200);

	indicator(0x00);				// Turn off all indicators
	blinkingON = 0;
}
