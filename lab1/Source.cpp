/*Лабараторная 1*/

/*29.	F(x) = ctg(x - 1) * cos(x)*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#include <iostream>

int main()
{
	

	int i = 0;

	do
	{
		system("cls");

		double ax, bx, x, d, res = 0, result_of_asm = 0, result_of_c = 0;
		clock_t time_start_c = 0, time_end_c = 0, time_start_asm = 0, time_end_asm = 0;
		float time_of_c = 0, time_of_asm = 0;
		float one = 1;
		printf("Enter range of interval: ");
		scanf_s("%lf\n", &ax);
		printf("Enter the increment step: ");
		scanf_s("%lf\n", &bx);
		scanf_s("%lf", &d);

		if (ax > bx)
		{
			double temp = ax;
			ax = bx;
			bx = temp;
		}

		time_start_c = clock();
		for (x = ax; x < bx; x += d)
		{
			double ct = (cos(x - 1) / sin(x - 1));
			result_of_c += ct * cos(x);
		}
		time_end_c = clock();
		time_of_c = float(time_end_c - time_start_c) / CLOCKS_PER_SEC;

		printf("\nTime of C: %.20f sec\n", time_of_c);
		//printf("Result of c: %lf\n\n", result_of_c);
		std::cout << result_of_c;

		time_start_asm = clock();
		_asm finit
		for (x = ax; x < bx; x += d)
		{
			_asm
			{
				fld x
				fsub one
				fcos
				fld x
				fsub one
				fsin
				fdiv
				fld x
				fcos
				fmul
				fstp res
			}
			_asm fwait
			result_of_asm += res;
		}
		time_end_asm = clock();
		time_of_asm = float(time_end_asm - time_start_asm) / CLOCKS_PER_SEC;

		printf("Time of ASM: %.20f sec\n", time_of_asm);
		printf("Result of asm: %lf\n\n", result_of_asm);

		system("pause >> NUL");

		printf("\nIf you want to run the program again, then press 1: ");
		scanf_s("%d", &i);
	} while (i == 1);

	return 0;
}