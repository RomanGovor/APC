#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define NOTES 36								// Number of notes
#define DELAY 10
#define TIME_MAX 65536
#define BASEFREQUENCY 1193180

#define D33 156									// Note frequencies     
#define E3 165
#define F33 185
#define G3 196
#define C4 262
#define B3 131
#define A33 233
#define A3 220
#define D3 147
#define C33 139
#define E4 330
#define D44 311

	
#define s 110									// 110 ms

void Sound();
void DivisionCoefficient();
void StatusWord();
void RandomNumber();

int main() 
{
	clrscr();
	char choice;
	do
	{
		printf("\n\n1. Generate sound.\n2. Division coefficient.\n3. Status word.\n4. Random number.\n5. Exit.\n");
		rewind(stdin);
		printf("\n\nChoise: ");
		scanf("%c", &choice);

		switch (choice) 
		{
		case '1': Sound(); break;
		case '2': DivisionCoefficient(); break;
		case '3': StatusWord(); break;
		case '4': RandomNumber(); break;
		}
	} while (choice != '5');
	return 0;
}

void Sound() {						// Pink Panther
	int frequency[NOTES] = { D33, E3, F33, G3, D33, E3, F33, G3, C4, B3, E3, G3, B3, A33, A3, G3, E3, D3, E3, D3,
	                         C33, D3, D33, E3, F33, G3, D33, E3, F33, G3, C4, B3, E3, G3, E4, D44};

	int durability[NOTES] = { (2*s)-50, (2 * s) - 20,(2 * s) - 60, (2 * s) - 20, (2 * s) - 60, (2 * s) - 30, (2*s) - 70, (2*s)-20, (2 * s) - 60, (2 * s) - 30, (2 * s) - 60, (2 * s), (2 * s) - 60, 9*s, (2 * s) - 60, (2 * s) - 70,(2 * s) - 50, (2 * s) - 70, (5 * s) - 70, (1*s)-10, 
							 (2 * s) - 60, (2 * s) - 30, (2 * s) - 50, (2 * s) - 20,(2 * s) - 60, (2 * s) - 20, (2 * s) - 60, (2 * s) - 30, (2 * s) - 70, (2 * s) - 50, (2 * s) - 60,  (2 * s) - 20, (2 * s) - 70, (2 * s) - 70, (6 * s) - 40, 2*s};

	int delayCounter[NOTES] = { 0, (5 * s) - 20, 0, (5 * s) - 20, 0, s, 0,s-20, 0, s - 10, 0, s-40, 0, 0, 0, 0, 0, 0, 0,(4 * s) - 60, 
	                           0, s-10,  0, (5 * s) - 20, 0, (5 * s) - 20, 0, s, 0, (2*s) - 90, 0, s-10, 0, (2 * s) - 90, 0, s-50 };
			
	for (int Counter = 0; Counter < NOTES; Counter++)
	{
		outp(0x43, 0xB6);                                           // Set channel 2 timer mode
		int diviCoeff = BASEFREQUENCY / frequency[Counter];			// Calculate the delay for loading the timer counter into the register
		outp(0x42, diviCoeff % 256);								// Low byte is written
		
									   //We load the register of the timer counter - first low, then high bytes

		diviCoeff /= 256;
		outp(0x42, diviCoeff);										// High byte is written

		outp(0x61, inp(0x61) | 3);									// Turn on the loudspeaker. The signal from channel 2 of the timer will now pass to the speaker input
	
																	// 11 - turn on the speaker and the 2nd channel
		delay(durability[Counter]);									// Turn off the loudspeaker
		outp(0x61, inp(0x61) & 0xFC);								// Turn off speaker and 2nd channel
		delay(delayCounter[Counter]);
	}
}

void DivisionCoefficient() 
{
	int ports[] = { 0x40, 0x41, 0x42 };								// Ports
	int controlWords[] = { 0, 64, 128 };

	printf("\n\nCoefficient of division: \n");

	for (int Channel = 0; Channel < 3; Channel++) 
	{
		unsigned Coeff = 0;
		unsigned max = 0;

		if (Channel == 2)
		{
			outp(0x61, inp(0x61) | 3);								// turn ON
		}

		for (unsigned long j = 0; j < TIME_MAX; j++)
		{
			outp(0x43, controlWords[Channel]);						// The channel status word is read
			unsigned CoeffLow = inp(ports[Channel]);						// The low and high bytes of the counter are entered and displayed
			unsigned CoeffHigh = inp(ports[Channel]);
			Coeff = CoeffHigh * 256 + CoeffLow;

			if (Coeff > max)
				max = Coeff;
		}
		if (Channel == 2) 
		{
			outp(0x61, inp(0x61) & 0xFC);							// turn OFF
		}
		printf("\nChannel %d: %4X\n", Channel, max);
	}
}

void StatusWord()
{
	printf("\n\nStatus word: \n");
	char stateWord[8];
	int ports[] = { 0x40, 0x41, 0x42 };
	int controlWords[] = { 226, 228, 232 };							// ÊCommand words
																
	for (int Channel = 0; Channel < 3; Channel++)
	{
		outp(0x43, controlWords[Channel]);                          
		int state = inp(ports[Channel]);                            // Low byte is entered                  

		for (int i = 7; i >= 0; i--) 
		{
			stateWord[i] = state % 2 + '0';
			state /= 2;

		}
		printf("\nChannel %d: ", Channel);
		for (i = 0; i < 8; i++) 
		{
			printf("%c", stateWord[i]);
		}
		printf("\n");
	}
}

void RandomNumber()
{
	char choice;
	unsigned int limit = TIME_MAX - 1, Low, High, number;

	do
	{
		printf("\n\n1. Set a limit.\n2. Get a number.\n3. Exit\n");
		fflush(stdin);
		printf("\nChoise: ");
		scanf("%s", &choice);

		switch (choice)
		{
		case '1':											// Limit setting
		{
			do												// Enter limit
			{
				printf("\nEnter a limit in range [1...65635].\n");
				fflush(stdin);
			} while (!scanf("%d", &limit) || limit < 1);

			outp(0x43, 0xB4);								// Set mode 3 for the second timer channel
			outp(0x42, limit % 256);                        // We load the register of the timer counter - first low, then high bytes
			limit /= 256;
			outp(0x42, limit);
			outp(0x61, inp(0x61) | 1);						// Not including the speaker, but including channel 2
			break;
		}
		case '2':											// Getting the number
		{
			outp(0x43, 128);
			Low = inp(0x42);								// Enter the low and high bytes of the counter and display it
			High = inp(0x42);
			number = High * 256 + Low;

			printf("\nNumber: %u\n", number);
			break;
		}
		}
	} while (choice != '3');								
	outp(0x61, inp(0x61) & 0xFC);							// turn OFF
}
