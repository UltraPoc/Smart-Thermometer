#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define AMOUNT 8

void main( void )
{
	int i;

	srand(time(NULL));
	printf("Press 0 to exit the program!\n");
	while (_getch() != '0')
	{
		printf("Your code: ");
		for (i = 0; i < AMOUNT
			; i++)
		{
			if (rand() % 2 == 0) //use number
				printf("%i", rand() % 10);
			else //use word
			{
				if (rand() % 2 == 0) //capital
					printf("%c", rand() % 26 + 65);
				else
					printf("%c", rand() % 26 + 97);
			}
		}
		printf("\n");
	}
}