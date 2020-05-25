/*Лабараторная 2*/

/*1.Вычитание матриц*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define million 1000000
#define N 8


int main()
{
	srand(time(NULL));

	int i = 0;
	int two = 2, one = 1;

	do
	{
		system("cls");

		int counter;
		clock_t time_start_c = 0, time_end_c = 0, time_start_asm = 0, time_end_asm = 0, time_start_mmx = 0, time_end_mmx = 0;
		float time_of_c = 0, time_of_asm = 0, time_of_mmx;
		short matrix1[N][N], matrix2[N][N], matrix_c[N][N], matrix_asm[N][N], matrix_mmx[N][N];

		printf("\nEnter count of iteration: ");
		scanf_s("%d", &counter);
		if (counter < million)
			counter = million;

		printf("\nFirst Matrix\n\n");
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				matrix1[i][j] = -10000 + rand() % 100000;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%7.0d", matrix1[i][j]);
			}printf("\n");
		}


		printf("\nSecond Matrix\n\n");
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				matrix2[i][j] = -10000 + rand() % 100000;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%7.0d", matrix2[i][j]);
			}printf("\n");
		}


		time_start_c = clock();
		for (int k = 0; k < counter; k++)							// Вычитание матриц 
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					matrix_c[i][j] = (matrix1[i][j] - matrix2[i][j]);
		time_end_c = clock();
		time_of_c = float(time_end_c - time_start_c) / CLOCKS_PER_SEC;

		printf("\nTime of C: %.20f sec\n", time_of_c);				// Вывод времени работы в С

		printf("\nResult matrix on C\n\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%7.0d", matrix_c[i][j]);
			}printf("\n");
		}
		/**/


		time_start_asm = clock();
		_asm finit
		for (int k = 0; k < counter; k++)
		{
			short cnt = N * N;
			_asm
			{
				pusha												// сохранить в стек все регистры
				xor esi, esi
				loop1 :
				mov ax, matrix1[esi]								// чтение из памяти
					mov bx, matrix2[esi]
					sub ax, bx
					mov matrix_asm[esi], ax
					add esi, 2										// short → шаг по 2 байта
					sub cnt, 1										// по 1 числу за итерацию
					jnz loop1
					popa											// восстановить из стека сохранённые регистры
			}
			_asm fwait
		}
		time_end_asm = clock();
		time_of_asm = float(time_end_asm - time_start_asm) / CLOCKS_PER_SEC;

		printf("\nTime of ASM: %.20f sec\n", time_of_asm);			// Вывод времени по АСМ
		printf("\nResult matrix on ASM\n\n");

		for (int i = 0; i < N; i++){
			for (int j = 0; j < N; j++) {
				printf("%7.0d", matrix_asm[i][j]);
			}printf("\n");
		}                                      // Вывод результирующего массива
		/**/
		time_start_mmx = clock();
		for (int k = 0; k < counter; k++)
		{
			short cnt = N*N;
			_asm
			{
				pusha												
				xor esi, esi
				loop2:
					movq MM0, matrix1[esi]								
					movq MM1, matrix2[esi]
					psubw MM0, MM1
					movq matrix_mmx[esi],MM0
					add esi, 8										
					sub cnt, 4										
				jnz loop2
					emms											
					popa											
			}
		}
		time_end_mmx = clock();
		time_of_mmx = float(time_end_mmx - time_start_mmx) / CLOCKS_PER_SEC;

		printf("\nTime of MMX: %.20f sec\n", time_of_mmx);			// Вывод времени в mmx				
		printf("\nResult matrix on MMX\n\n");						

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("%7.0d", matrix_mmx[i][j]);
			}printf("\n");									// Вывод массива
		}

		system("pause >> NUL");
		printf("\nIf you want to run the program again, then press 1: ");
		scanf_s("%d", &i);
	} while (i == 1);

	return 0;
}