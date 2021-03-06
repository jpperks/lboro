//
// Created by Richard Marchant (B512813) and Jonathan Perkins (B529175).
// Copyright © 2015. All rights reserved.
//

//****************** LOW RESOLUTION VERSION *******************
//******************       128 x 256        *******************

//=============================================================
//                                                            
//					INCLUDES & DEFINITIONS                  
//
//=============================================================

#define _CRT_SECURE_NO_WARNINGS // Stops VS 'scanf' errors.
#define height 128 //
#define width 256  // Set the height, width and colour depth up here.
#define colour 255 //
#include <stdio.h> // Main input/output libary.
#include <math.h> // Required for waveform calculations.
#include <conio.h> // Required for quicker user input using _getch.
#include <time.h> // Required for colour generator.
#include <stdlib.h> // Required for colour generator.
#define PI 3.14159265 // To convert radians to degrees.

//=============================================================
//                                                            
//					  FUNTION PROTOTYPES                   
//
//=============================================================

void zero(int, int); // Calculates zero's position.
void ones(int, int); // Calculates one's position.
void decs(int, int); // Calculates single decimal point position.
void dec2(int, int); // Calculates double (horizontal) decimal point position.
void dec3(int, int); // Calculates triple (vertical) decimal point position.
int numbers[height][width][3]; // Our main array to store pixel values.

int main()
{
	char d = 'r'; // Intialise restart variable to 'r' so the program runs on launch.
	do
	{
		if (d == 'r' || d == 'R')
		{
			char d = 'x'; // Set the restart variable back to 'x' to stop further loops.

			//=============================================================
			//                                                            
			//                      SET ARRAY TO 0                     
			//
			//=============================================================

			int i, j, z;     // Setting all the values of the array i = Y axis j = X axis z = Colour type (RGB).

			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					for (z = 0; z < 3; z++)
					{
						numbers[i][j][z] = 0; // Sets the whole array to 0.
					}
				}
			}

			//=============================================================
			//                                                            
			//                      DRAW AXIS LINES                     
			//
			//=============================================================

			/*Set Y Axis Line*/
			{
				int y, w;           // y = y axis level, w = axis line width.
				for (y = 8; y < 109; y++)
				{
					for (w = 37; w < 39; w++)
					{
						numbers[y][w][0] = 255;
						numbers[y][w][1] = 255;
						numbers[y][w][2] = 255;
					}
				}

			}

			/*Set X Axis Line*/
			{
				int x, w;           // x = x axis level, w = axis line width.
				for (x = 37; x < 219; x++)
				{
					for (w = 109; w < 111; w++)
					{
						numbers[w][x][0] = 255;
						numbers[w][x][1] = 255;
						numbers[w][x][2] = 255;
					}
				}

			}

			/*Set X Axis Centre Line*/
			{
				int x;           // x = x axis level.
				for (x = 39; x < 219; x++)
				{

					numbers[58][x][0] = 168;
					numbers[58][x][1] = 177;
					numbers[58][x][2] = 181;
				}

			}

			//=============================================================
			//                                                            
			//                     DRAW AXIS TICKS                     
			//
			//=============================================================

			/*Y Axis Ticks*/
			{
				int x, c;
				for (x = 35; x < 37; x++)
				{
					for (c = 0; c < 3; c++)
					{
						numbers[19][x][c] = 255; // '1.0' Value Tick
						numbers[58][x][c] = 255; // '0.0' value Tick
						numbers[97][x][c] = 255; // '-1.0' value Tick
					}
				}
			}

			/*X Axis Ticks */
			{
				int y, c;
				for (y = 111; y < 113; y++)
				{
					for (c = 0; c < 3; c++)
					{
						numbers[y][39][c] = 255; // '0 deg' value Tick
						numbers[y][83][c] = 255; // '180 deg' value Tick
						numbers[y][128][c] = 255; // '360 deg' value Tick
						numbers[y][173][c] = 255; // '540 deg' value Tick
						numbers[y][218][c] = 255; // '720 deg' value Tick
					}
				}
			}

			//=============================================================
			//                                                            
			//                       Y AXIS NUMBERS             
			//
			//=============================================================

			ones(17, 26); //
			decs(21, 28); // "1.0"
			zero(17, 30); //

			zero(56, 24); //
			decs(59, 28); // "0.0"
			zero(56, 30); //

			dec2(97, 23); //
			ones(95, 26); // "-1.0"
			decs(99, 28); //
			zero(95, 30); //

						  //=============================================================
						  //                                                            
						  //                       X AXIS NUMBERS             
						  //
						  //=============================================================

			zero(114, 38); // "0"

			zero(114, 82); //
			zero(114, 86); // "180"
			ones(114, 80); //
			decs(116, 83); //

			dec2(114, 123); //
			dec2(116, 123); //
			dec2(118, 123); //
			zero(114, 131); //
			dec2(116, 128); // "360"
			dec2(118, 128); //
			decs(117, 129); //
			ones(114, 125); //
			ones(114, 127); //

			zero(114, 176); //
			dec2(114, 168); //
			dec3(114, 167); //
			decs(116, 168); //
			dec3(116, 169); // "540"
			dec2(118, 167); //
			dec3(114, 171); //
			dec2(117, 171); //
			dec3(116, 173); //
			decs(117, 174); //

			ones(114, 215); //
			zero(114, 221); //
			dec2(114, 213); //
			dec2(114, 217); //
			decs(114, 219); // "720"
			decs(115, 219); //
			decs(116, 218); //
			decs(117, 217); //
			dec2(118, 217); //
			decs(118, 219); //

							//=============================================================
							//                                                            
							//                       WAVEFORMS         
							//
							//=============================================================

			int inp, r1[999], g1[999], b1[999]; // Previous waveform colour save arrays.
			{
				int t = 0;
				int offt, offs, ap, j, z, c, r, g, b;
				double val;
				ap = 40; // Amplitude.
				val = PI / 180; // Radians to degrees conversion.
				offt = 39; // Offset in the x direction.
				offs = 58; // Offset in the y direction.
				int count = 1;
				printf("How many sine waves of accuracy do you want? ");
				scanf("%d/n", &inp);

				time_t e;                   // Define colour generator.
				srand((unsigned)time(&e));  //

				while (inp <= 0 || inp > 999) // Accuracy must be between 1 and 999
				{
					printf("\nNumber must be between 1 and 999! Try again. ");
					scanf("%d/n", &inp);
				}

				if (inp > 500)
				{
					printf("\nYikes! This might take a while...");
				}
				else { printf("Please Wait..."); }

				// BUILD PREVIOUS SINE WAVEFORMS
				{
					for (c = inp; c > 0; c--)
					{
						int s = 0;
						j = ((2 * c) - 1);

						for (t = 0; t < 720; t++)
						{
							s = 0;

							for (count = 1; count <= j; count += 2)
							{
								s = (int)((ap / count) * (sin(count*t*val))); // Generate Equation for next waveform.
							}

							z = offs - s;

							numbers[z][offt + (t / 4)][0] = 150; //
							numbers[z][offt + (t / 4)][1] = 0;  // Colour is always dark red for each waveform.
							numbers[z][offt + (t / 4)][2] = 0; //
						}
					}
				}

				// BUILD PREVIOUS SQUARE WAVEFORMS
				{
					for (c = inp; c > 0; c--)
					{
						r = (rand() % 150) + 50; //
						g = (rand() % 150) + 50; // Create random colour between 50 & 200 for each waveform.
						b = (rand() % 150) + 50; //

						r1[c] = r; //
						g1[c] = g; // Stores colour for each wave to fetch later...
						b1[c] = b; //

						int s = 0;
						j = ((2 * c) - 1);

						for (t = 0; t < 720; t++)
						{
							s = 0;

							for (count = 1; count <= j; count += 2)
							{
								s = (int)(((ap / count) * (sin(count*t*val))) + s); // Generate Equation for next waveform.
							}

							z = offs - s;

							numbers[z][offt + (t / 4)][0] = r; //
							numbers[z][offt + (t / 4)][1] = g; // Random colour for each waveform.
							numbers[z][offt + (t / 4)][2] = b; //
						}
					}
				}

				// BUILD FINAL SQAURE WAVEFORM
				int s = 0;
				j = ((2 * inp) - 1);

				for (t = 0; t < 720; t++)
				{
					s = 0;

					for (count = 1; count <= j; count += 2)
					{
						s = (int)(((ap / count) * (sin(count*t*val))) + s); // Generate Equation for next waveform.
					}

					z = offs - s;

					numbers[z][offt + (t / 4)][0] = 245; //
					numbers[z][offt + (t / 4)][1] = 245; // Final waveform is always yellow.
					numbers[z][offt + (t / 4)][2] = 60;  //
				}
			}

			//=============================================================
			//                                                            
			//						 WAVEFORM KEY       
			//
			//=============================================================

			{
				int c; // Line pixel counter.

					   /*  "FIRST" */
				dec2(13, 218); //
				ones(13, 217); // "F"
				dec2(15, 218); //

				ones(13, 221); // "I"

				ones(13, 223); //
				dec2(13, 224); //
				decs(14, 225); // "R"
				dec2(15, 224); //
				decs(16, 224); //
				decs(17, 225); //

				dec2(13, 227); //
				decs(13, 229); //
				decs(14, 227); //
				decs(15, 228); // "S"
				decs(16, 229); //
				dec2(17, 227); // 
				decs(17, 229); //

				dec2(13, 231); //
				decs(13, 233); // "T"
				dec3(14, 232); //
				decs(17, 232); //

							   /* First "=" */
				dec2(14, 235);
				decs(14, 237);
				dec2(16, 235);
				decs(16, 237);


				/* "FINAL" */
				if (inp > 1)
				{
					ones(23, 216); //
					dec2(23, 217); // "F"
					dec2(25, 217); //

					ones(23, 220); // "I"

					ones(23, 222); //
					decs(24, 223); //
					decs(25, 223); // "N"
					decs(25, 224); //
					decs(26, 224); //
					ones(23, 225); //

					dec3(24, 227); //
					decs(27, 227); //
					decs(23, 228); // "A"
					decs(25, 228); //
					dec3(24, 229); //
					decs(27, 229); //

					ones(23, 231); // "L"
					dec2(27, 232); //

								   /*Final "="*/
					dec2(24, 235);
					decs(24, 237);
					dec2(26, 235);
					decs(26, 237);

					/*Final Sample Line*/
					for (c = 0; c < 15; c++)
					{
						numbers[25][239 + c][0] = 245; //
						numbers[25][239 + c][1] = 245; // Line sample of the final waveform. This is always yellow.
						numbers[25][239 + c][2] = 60;  //
					}

					/*First Sample Line*/
					for (c = 0; c < 15; c++)
					{
						numbers[15][239 + c][0] = r1[1]; //
						numbers[15][239 + c][1] = g1[1]; // Line sample of the first waveform if > 1 has been displayed.
						numbers[15][239 + c][2] = b1[1]; //
					} c = 0;
				}
				else
				{
					/*First Sample Line*/
					for (c = 0; c < 15; c++)
					{
						numbers[15][239 + c][0] = 245; //
						numbers[15][239 + c][1] = 245; // Line sample of the first waveform if only 1 has been displayed.
						numbers[15][239 + c][2] = 60;  //
					} c = 0;
				}

				/* "KEY" */
				ones(3, 226); //
				decs(5, 227); //
				decs(4, 228); // "K"
				decs(3, 229); //
				decs(6, 228); //
				decs(7, 229); //

				ones(3, 231); //
				dec2(3, 232); // "E"
				dec2(5, 232); //
				dec2(7, 232); //

				decs(3, 235); //
				decs(4, 235); //
				dec3(5, 236); // "Y"
				decs(3, 237); //
				decs(4, 237); //

							  /* "KEY" Underline */
				for (c = 224; c < 239; c++)
				{
					numbers[8][c][0] = 171;
					numbers[8][c][1] = 171;
					numbers[8][c][2] = 171;
				} c = 0;

				/* Outer Lines*/
				for (c = 0; c < 31; c++)
				{
					numbers[c][210][0] = 230; //
					numbers[c][210][1] = 230; // Vertical "off-white" colour, key boundary line.
					numbers[c][210][2] = 230; //
				} c = 0;

				for (c = 210; c < 256; c++)
				{
					numbers[30][c][0] = 230; //
					numbers[30][c][1] = 230; // Horizontal "off-white" colour, key boundary line.
					numbers[30][c][2] = 230; //
				} c = 0;
			}

			//=============================================================
			//                                                            
			//               OPEN AND SET PPM FILE HEADER                     
			//
			//=============================================================

			FILE *pfile = NULL; // Create file pointer and set to NULL.

			if ((pfile = fopen("myimage.ppm", "w")) == NULL) /* Checks if the .pmm file can be opened successfully.
															 Returns -1 if there is a problem.*/
			{
				printf("\nThere was a problem opening '%s', please contact support.", "myimage.ppm");
				return -1;
			}

			fprintf(pfile, "P3\n"); // Identify PPM file type.
			fprintf(pfile, "# myimage.ppm\n"); // Writes confirmation comment of the file name.
			fprintf(pfile, "%d %d\n", width, height); // Writes the width and height of the array.
			fprintf(pfile, "%d\n", colour); // Writes the colour depth level (always 255).

			//=============================================================
			//                                                            
			//               WRITE DATA FROM ARRAY TO FILE        
			//
			//=============================================================

			int a, b, c;
			for (a = 0; a < height; a++)
			{
				for (b = 0; b < width; b++)
				{
					for (c = 0; c < 3; c++)
					{
						fprintf(pfile, "%d", numbers[a][b][c]);
						fprintf(pfile, " ");
					}fprintf(pfile, "   ");
				}fprintf(pfile, "\n\n");
			}	                              // The file can be found here...\executable_folder\myimage.ppm
			printf("\nMade Changes! Look for 'myimage.pmm' in the project folder.\n\n");
			fclose(pfile);
		}

		printf("Press [R] to restart or any key to exit...\n\n\n\n");
		d = _getch();

	} while (d == 'r' || d == 'R');
	printf("\n\n");
	printf("Goodbye! ");
}

//=============================================================
//                                                            
//                      FUNCTIONS      
//
//=============================================================

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
