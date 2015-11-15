// Functions

#include <stdio.h>
#include <functions.h>


void zero(int y, int x)      // Calculates zero's position.
{
	int c = 0;
	int y1 = 0;
	int x1 = 0;

	for (x1 = (x); x1 < (x + 3); x1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y][x1][c] = 171;
		}
	} x1 = 0; c = 0;

	for (y1 = (y); y1 < (y + 4); y1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y1][x][c] = 171;
		}
	} y1 = 0; c = 0;

	for (y1 = (y + 1); y1 < (y + 5); y1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y1][x + 2][c] = 171;
		}
	} y1 = 0; c = 0;

	for (x1 = (x); x1 < (x + 3); x1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y + 4][x1][c] = 171;
		}
	} x1 = 0; c = 0;
}


void ones(int y, int x)           // Calculates one's position.
{
	int c;
	int y1 = 0;
	for (y1 = (y); y1 < (y + 5); y1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y1][x][c] = 171;
		}
	} x = 0; y = 0; y1 = 0;
}

void decs(int y, int x)         // Calculates single decimal point position.
{
	int c = 0;
	for (c = 0; c < 3; c++)
	{
		numbers[y][x][c] = 171;
	}
}

void dec2(int y, int x)        // Calculates double (horizontal) decimal point position.
{
	int c = 0;
	for (c = 0; c < 3; c++)
	{
		numbers[y][x][c] = 171;
		numbers[y][x + 1][c] = 171;
	}
}

void dec3(int y, int x)        // Calculates triple (vertical) decimal point position.
{
	int c;
	int y1 = 0;
	for (y1 = (y); y1 < (y + 3); y1++)
	{
		for (c = 0; c < 3; c++)
		{
			numbers[y1][x][c] = 171;
		}
	} x = 0; y = 0; y1 = 0;
}
