#include <dos.h> 

#define BUFF_WIDTH 80
#define CENTER_OFFSET 12
#define LEFT_OFFSET 20
#define REG_SCREEN_SIZE 9

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

int attribute = 0x5e;	//color

void print(int offset, int value);
void getRegisterValue();

void interrupt(*oldHandleB0) (...);
void interrupt(*oldHandleB1) (...);
void interrupt(*oldHandleB2) (...);
void interrupt(*oldHandleB3) (...);
void interrupt(*oldHandleB4) (...);
void interrupt(*oldHandleB5) (...);
void interrupt(*oldHandleB6) (...);
void interrupt(*oldHandleB7) (...);

void interrupt(*oldHandle08) (...);
void interrupt(*oldHandle09) (...);
void interrupt(*oldHandle0A) (...);
void interrupt(*oldHandle0B) (...);
void interrupt(*oldHandle0C) (...);
void interrupt(*oldHandle0D) (...);
void interrupt(*oldHandle0E) (...);
void interrupt(*oldHandle0F) (...);

void interrupt newHandleB0(...) { getRegisterValue(); oldHandleB0(); }
void interrupt newHandleB1(...) { attribute++; getRegisterValue(); oldHandleB1(); }
void interrupt newHandleB2(...) { getRegisterValue(); oldHandleB2(); }
void interrupt newHandleB3(...) { getRegisterValue(); oldHandleB3(); }
void interrupt newHandleB4(...) { getRegisterValue(); oldHandleB4(); }
void interrupt newHandleB5(...) { getRegisterValue(); oldHandleB5(); }
void interrupt newHandleB6(...) { getRegisterValue(); oldHandleB6(); }
void interrupt newHandleB7(...) { getRegisterValue(); oldHandleB7(); }

void interrupt newHandle08(...) { getRegisterValue(); oldHandle08(); }
void interrupt newHandle09(...) { getRegisterValue(); oldHandle09(); }
void interrupt newHandle0A(...) { getRegisterValue(); oldHandle0A(); }
void interrupt newHandle0B(...) { getRegisterValue(); oldHandle0B(); }
void interrupt newHandle0C(...) { getRegisterValue(); oldHandle0C(); }
void interrupt newHandle0D(...) { getRegisterValue(); oldHandle0D(); }
void interrupt newHandle0E(...) { getRegisterValue(); oldHandle0E(); }
void interrupt newHandle0F(...) { getRegisterValue(); oldHandle0F(); }

void print(int offset, int value)
{
	char temp;

	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);
	screen += CENTER_OFFSET * BUFF_WIDTH + offset;

	for (int i = 7; i >= 0; i--)
	{    
		temp = value % 2;
		value /= 2;
		screen->symb = temp + '0';
		screen->attr = attribute;
		screen++;
	}
}

void getRegisterValue() 
{
	print(0 + LEFT_OFFSET, inp(0x21));

	outp(0x20, 0x0B);             
	print(REG_SCREEN_SIZE + LEFT_OFFSET, inp(0x20));

	outp(0x20, 0x0A);             
	print(REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0x20));
             
	print(BUFF_WIDTH + LEFT_OFFSET, inp(0xA1));

	outp(0xA0, 0x0B);             
	print(BUFF_WIDTH + REG_SCREEN_SIZE + LEFT_OFFSET, inp(0xA0));

	outp(0xA0, 0x0A);             
	print(BUFF_WIDTH + REG_SCREEN_SIZE * 2 + LEFT_OFFSET, inp(0xA0));
}

void init()
{
	// IRQ8-15
	oldHandleB0 = getvect(0x70);		// Real time clock
	oldHandleB1 = getvect(0x71);	    // Random deviece
	oldHandleB2 = getvect(0x72);	    // Random deviece
	oldHandleB3 = getvect(0x73);		// Random deviece or timer
	oldHandleB4 = getvect(0x74);		// PS/2 mouse
	oldHandleB5 = getvect(0x75);		// FPU error
	oldHandleB6 = getvect(0x76);		// Random deviece or first ATA controller
	oldHandleB7 = getvect(0x77);		// Random deviece or second ATA controller

	// IRQ0-7
	oldHandle08 = getvect(0x08);        // Timer
	oldHandle09 = getvect(0x09);        // Keyboard
	oldHandle0A = getvect(0x0A);		// Slave IRQ
	oldHandle0B = getvect(0x0B);	    // Random deviece
	oldHandle0C = getvect(0x0C);	    // Random deviece
	oldHandle0D = getvect(0x0D); 	    // Random deviece
	oldHandle0E = getvect(0x0E); 	    // Random deviece
	oldHandle0F = getvect(0x0F);	    // Random deviece

	setvect(0xB0, newHandleB0);
	setvect(0xB1, newHandleB1);
	setvect(0xB2, newHandleB2);
	setvect(0xB3, newHandleB3);
	setvect(0xB4, newHandleB4);
	setvect(0xB5, newHandleB5);
	setvect(0xB6, newHandleB6);
	setvect(0xB7, newHandleB7);

	setvect(0x08, newHandle08);
	setvect(0x09, newHandle09);
	setvect(0x0A, newHandle0A);
	setvect(0x0B, newHandle0B);
	setvect(0x0C, newHandle0C);
	setvect(0x0D, newHandle0D);
	setvect(0x0E, newHandle0E);
	setvect(0x0F, newHandle0F);

	// Disable interrupts handling (cli)
	_disable();

	// Master
	outp(0x20, 0x11); // ICW1
	outp(0x21, 0x08); // ICW2
	outp(0x21, 0x04); // ICW3
	outp(0x21, 0x01); // ICW4

	// Slave
	outp(0xA0, 0x11); // ICW1
	outp(0xA1, 0xB0); // ICW2
	outp(0xA1, 0x02); // ICW3
	outp(0xA1, 0x01); // ICW4

	_enable();
}

int main()
{
	unsigned far *fp;

	init();

	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
	return 0;
}