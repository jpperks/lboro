//
// Created by Richard Marchant (B512813) and Jonathan Perkins (B529175).
// Copyright © 2015. All rights reserved.
//

// **************** HIGH RESOLUTION VERSION *******************

//=============================================================
//                                                            
//					INCLUDES & DEFINITIONS                  
//
//=============================================================

#define _CRT_SECURE_NO_WARNINGS // Stops VS 'scanf' errors.
#define height 512 //
#define width 1024  // Set the height, width and colour depth up here.
#define colour 255 //
#include <stdio.h>
#include <math.h>
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
void lslope(int, int, int, int); // Calculates left (downwards) slope.
void rslope(int, int, int, int); // Calculates right (downwards) slope.
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
						  //           SET PPM FILE HEADER AND ARRAY TO '0'                    
						  //
						  //=============================================================

			FILE *pfile = NULL;

			if ((pfile = fopen("myimage.ppm", "w")) == NULL)
			{
				printf("\nThere was a problem opening '%s', please contact support.", "myimage.ppm");
				return -1;
			}

			fprintf(pfile, "P3\n");
			fprintf(pfile, "# myimage.ppm\n");
			fprintf(pfile, "%d %d\n", width, height);
			fprintf(pfile, "%d\n", colour);

			int i, j, z;     // Setting all the values of the array i = Y axis j = X axis z = Colour type (RGB).

			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					for (z = 0; z < 3; z++)
					{
						numbers[i][j][z] = 0;
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
				int y, w;           // y = y axis level, w = axis line width (2px).

				for (y = 32; y < 436; y++)
				{
					for (w = 152; w < 156; w++)
					{
						numbers[y][w][0] = 255;
						numbers[y][w][1] = 255;
						numbers[y][w][2] = 255;
					}
				}

			}

			/*Set X Axis Line*/

			{
				int x, w;           // x = x axis level, w = axis line width (2px).

				for (x = 152; x < 876; x++)
				{
					for (w = 436; w < 440; w++)
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

				for (x = 156; x < 876; x++)
				{

					numbers[232][x][0] = 168;
					numbers[232][x][1] = 177;
					numbers[232][x][2] = 181;
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
				for (x = 144; x < 152; x++)
				{
					for (c = 0; c < 3; c++)
					{
						numbers[72][x][c] = 255; // '1.0' Value Tick
						numbers[232][x][c] = 255; // '0.0' value Tick
						numbers[392][x][c] = 255; // '-1.0' value Tick
					}
				}
			}

			/*X Axis Ticks */

			{
				int y, c;
				for (y = 440; y < 448; y++)
				{
					for (c = 0; c < 3; c++)
					{
						numbers[y][156][c] = 255; // '0 deg' value Tick
						numbers[y][335][c] = 255; // '180 deg' value Tick
						numbers[y][515][c] = 255; // '360 deg' value Tick
						numbers[y][695][c] = 255; // '540 deg' value Tick
						numbers[y][875][c] = 255; // '720 deg' value Tick
					}
				}
			}

			//=============================================================
			//                                                            
			//                       Y AXIS NUMBERS             
			//
			//=============================================================

			ones(63, 104);  //
			decs(82, 113);  // "1.0"
			zero(63, 120);  //

			zero(223, 94);  //
			decs(242, 113); // "0.0"
			zero(223, 120); //

			dec2(392, 92);  //
			ones(382, 104); // "-1.0"
			decs(401, 113); //
			zero(382, 120); //

			//=============================================================
			//                                                            
			//                       X AXIS NUMBERS             
			//
			//=============================================================
			
			zero(454, 150); // "0"
			
			zero(454, 328); //
			zero(454, 346); // "180"
			ones(454, 320); //
			dec2(464, 331); //
			
			dec2(454, 493); //
			dec2(463, 493); //
			dec2(472, 493); //
			ones(454, 500); // 
			decs(463, 517); // "360"
			decs(463, 518); //
			dec2(463, 511); // 
			dec2(472, 511); //
			dec3(465, 517); //
			ones(454, 508); //
			zero(454, 525); //
			
			dec2(454, 677); //
			dec3(454, 674); //
			dec2(464, 674); // 
			dec3(464, 681); // 
			dec2(472, 674); //
			dec2(472, 677); // "540"
			dec3(454, 689); //
			dec2(464, 689); //
			dec2(464, 694); //
			dec3(465, 695); //
			dec3(460, 695); //
			zero(454, 706); //

			ones(454, 860);       //
			dec2(454, 853);       //                     
			dec2(454, 868);       //                     
			dec2(454, 871);       //
			lslope(456, 876, 15, 0); // "720"  
			dec2(472, 868);       //                
			dec2(472, 871);       //                             
			zero(454, 883);       //

			//=============================================================
			//                                                            
			//                       WAVEFORMS         
			//
			//=============================================================

			int inp, r1[500], g1[500], b1[500];
			{
				int t = 0;
				int offt, offs, ap, j, z, c, r, g, b;
				double val;
				ap = 160; // Amplitude.
				val = PI / 180; // Radians to degrees conversion.
				offt = 156; // Offset in the x direction.
				offs = 232; // Offset in the y direction.
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

							numbers[z][offt + (t)][0] = 150; //
							numbers[z][offt + (t)][1] = 0;  // Colour always dark red for each waveform.
							numbers[z][offt + (t)][2] = 0; //
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

						r1[c] = r;
						g1[c] = g;
						b1[c] = b;

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

							numbers[z][offt + (t)][0] = r; //
							numbers[z][offt + (t)][1] = g; // Random colour for each waveform.
							numbers[z][offt + (t)][2] = b; //
						}
					}
				}

				// BUILD FINAL WAVEFORM

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

					numbers[z][offt + (t)][0] = 245; //
					numbers[z][offt + (t)][1] = 245; // Final waveform is always yellow.
					numbers[z][offt + (t)][2] = 60;  //
				}
			}

			//=============================================================
			//                                                            
			//						 WAVEFORM KEY       
			//
			//=============================================================
			
			{
				int c; // Line pixel counter.
				int w;
				/*  "FIRST" */
				dec2(42, 871);         //
				ones(42, 868);         // "F"
				dec2(50, 871);         //

				ones(42, 883);         // "I"

				ones(42, 891);         //
				dec2(42, 894);         //
				dec3(42, 898);         // 
				dec2(50, 894);         // 
				decs(59, 898);         // "R"
				decs(57, 896);         //
				decs(55, 894);         //
				decs(61, 900);         //
				rslope(53, 894, 8, 1); // 
				
				dec2(42, 909);         //
				dec3(42, 906);         //
				dec2(52, 906);         // "S"
				dec3(52, 913);         //
				dec2(60, 906);         //
				dec2(60, 909);         //
				
				ones(42, 924);         //
				dec2(42, 921);         // "T"
				dec2(42, 924);         //
			
				/* First "=" */
				dec2(50, 936);
				dec2(56, 936);
			
				/* "FINAL" */
			
				if (inp > 1)
				{
					ones(82, 863);          //
					dec2(82, 866);          // "F"
					dec2(90, 866);          //

					ones(82, 878);          // "I"
					
					ones(82, 886);          //
					ones(82, 897);          // "N"                     
					rslope(82, 888, 20, 0); //

					dec3(85, 905);          // 
					dec3(93, 905);          //
					dec2(82, 907);          // "A"
					dec2(92, 907);          //
					dec3(85, 913);          //
					dec3(93, 913);          //

					ones(82, 921);          // "L"
					dec2(100, 924);         //

					/*Final "="*/
					dec2(90, 936);
					dec2(96, 936);

					/*Final Line*/
					for (w = 93; w < 96; w++)
					{
						for (c = 0; c < 55; c++)
						{
							numbers[w][953 + c][0] = 245; //
							numbers[w][953 + c][1] = 245; // Line sample of the final waveform. This is always yellow.
							numbers[w][953 + c][2] = 60;  //
						}
					}

					/*First Line*/
					for (w = 53; w < 56; w++)
					{
						for (c = 0; c < 55; c++)
						{
							numbers[w][953 + c][0] = r1[1]; //
							numbers[w][953 + c][1] = g1[1]; // Line sample of the first waveform if > 1 has been displayed.
							numbers[w][953 + c][2] = b1[1]; //
						} c = 0;
					}
				}
				else
				{
					/*First Line*/
					for (w = 53; w < 56; w++)
					{
						for (c = 0; c < 55; c++)
						{
							numbers[w][953 + c][0] = 245; //
							numbers[w][953 + c][1] = 245; // Line sample of the first waveform if only 1 has been displayed.
							numbers[w][953 + c][2] = 60;  //
						} c = 0;
					}
				}

				/* "KEY"*/
				ones(9, 914);          // 
				lslope(9, 924, 9, 1);  // "K"
				rslope(20, 916, 9, 1); //
				decs(19, 916);         //
			
				ones(9, 931);          //
				dec2(9, 934);          // "E"
				dec2(18, 934);         //
				dec2(27, 934);         //

				rslope(9, 946, 10, 0); //
				lslope(9, 955, 10, 0); // "Y"
				dec3(20, 950);         //
				dec3(18, 950);         // 
			
				/* "KEY" Underline */
				for (c = 910; c < 959; c++)
				{
				numbers[31][c][0] = 171;
				numbers[31][c][1] = 171;
				numbers[31][c][2] = 171;
				} c = 0;

				/* Outer Lines*/
				for (c = 0; c < 110; c++)
				{
					for (w = 846; w < 848; w++)
					{
						numbers[c][w][0] = 230; //
						numbers[c][w][1] = 230; // Vertical "off-white" colour, key boundary line.
						numbers[c][w][2] = 230; //
					}
				} c = 0; w = 0;
				for (c = 846; c < 1023; c++)
				{
					for (w = 108; w < 110; w++)
					{
						numbers[w][c][0] = 230; //
						numbers[w][c][1] = 230; // Horizontal "off-white" colour, key boundary line.
						numbers[w][c][2] = 230; //
					}
				} c = 0; w = 0;
			}
			//=============================================================
			//
			//               WRITE DATA FROM ARRAY TO FILE
			//
			//=============================================================
			
			int a, b, c;                         /* Drawing the array in the console and writing the same data to the
												 .ppm file...*/
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
			}	                              // The file can be found here...\Visual Studio 20xx\Projects\ProjectName\ProjectName\myimage.ppm
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
	int w = 0;
	for (x1 = (x); x1 < (x + 12); x1++) // Top
	{
		for (w = y; w < (y + 3); w++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[w][x1][c] = 171;
			}
		}
	} w = 0; x1 = 0; c = 0;

	for (y1 = (y); y1 < (y + 20); y1++) // Left Side
	{
		for (w = x; w < (x + 3); w++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[y1][w][c] = 171;
			}
		}
	} y1 = 0; c = 0;

	for (y1 = (y); y1 < (y + 20); y1++) // Right Side
	{
		for (w = x; w < (x + 3); w++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[y1][w + 10][c] = 171;
			}
		}
	} y1 = 0; c = 0;

	for (x1 = (x); x1 < (x + 13); x1++) // Bottom
	{
		for (w = y; w < (y + 3); w++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[w + 18][x1][c] = 171;
			}
		}
	} x1 = 0; c = 0;
}

void ones(int y, int x)           // Calculates one's position.
{
	int c;
	int y1 = 0;
	int w = 0;
	for (y1 = (y); y1 < (y + 21); y1++)
	{

		for (w = x; w < (x + 3); w++)
		{

			for (c = 0; c < 3; c++)
			{

				numbers[y1][w][c] = 171;
			}
		}
	}  w = 0; x = 0; y = 0; y1 = 0;
}

void decs(int y, int x)         // Calculates single decimal point position.
{
	int c = 0;
	int x1 = 0;
	int y1 = 0;
	for (x1 = x; x1 < (x + 2); x1++)
	{
		for (y1 = y; y1 < (y + 2); y1++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[y1][x1][c] = 171;
			}
		}
	}
}

void dec2(int y, int x)        // Calculates double (horizontal) decimal point position.
{
	int c = 0;
	int x1 = 0;
	int y1 = 0;

	for (x1 = x; x1 < (x + 7); x1++)
	{
		for (y1 = y; y1 < (y + 3); y1++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[y1][x1][c] = 171;
			}
		}
	}

}

void dec3(int y, int x)        // Calculates triple (vertical) decimal point position.
{
	int c;
	int y1 = 0;
	int w = 0;
	for (y1 = (y); y1 < (y + 10); y1++)
	{
		for (w = x; w < (x + 3); w++)
		{
			for (c = 0; c < 3; c++)
			{
				numbers[y1][w][c] = 171;
			}
		}
	} x = 0; y = 0; y1 = 0;
}

void lslope(int y, int x, int d, int b)     // Calculates left (downwards) slope.
{
	                         
	int two_y = 0;
	int two_x = x;
	int depth = d;
	int big = b;
	for (two_y = y; two_y < (y+depth); two_y+=2) 
	{
		decs(two_y, two_x);               
		if (big == 1)
		{
			two_x -= 2;
		}
		else { two_x--; }
	}                                   
}

void rslope(int y, int x, int d, int b)     // Calculates right (downwards) slope.
{

	int two_y = 0;
	int two_x = x;
	int depth = d;
	int big = b;
	for (two_y = y; two_y < (y + depth); two_y += 2)
	{
		decs(two_y, two_x);
		if (big == 1)
		{
			two_x += 2;
		}
		else { two_x++; }
	}
}
