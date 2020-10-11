#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PI 3.14159265

const int BMP_WIDTH_OFFSET = 18;
const int BMP_HEIGHT_OFFSET = 22;
const int BMP_DEPTH_OFFSET = 28;
const int HEADER_SIZE_BYTE = 54;
const int COLOR_TABLE_SIZE_BYTE = 1024;
const int IMAGE_WIDTH = 512;
const int IMAGE_HEIGHT = 512;


void HorizontalDerivative(int Width, int Height, unsigned char* img, int* horDericativeImg);
void VerticalDerivative(int Width, int Height, unsigned char* img, int* verDericativeImg);
void EdgeImage(int Width, int Height, int* horDericativeImg, int* verDericativeImg, int* edgeImg);
void NonmaximumSuppresion(int Width,
	int Height,
	int* horizontalDerivativeImg,
	int* verticalDerivativeImg,
	int* edgeImg,
	unsigned char* nonmaximumSuppresionImg);

int main()
{
	FILE* input_image_file = fopen("images/lena512.bmp", "rb");
	//FILE* input_image_file = fopen("images/barbara512.bmp", "rb");

	if (input_image_file == (FILE*)0)
	{
		printf("Input Image can't read!\n");
		return 0;
	}
	// Else no operation.

	// Input Image.
	unsigned char* input_image_header = new unsigned char[HEADER_SIZE_BYTE];
	unsigned char* input_image_color_table = new unsigned char[COLOR_TABLE_SIZE_BYTE];
	unsigned char* input_image_data = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT];

	unsigned char* deneme = new unsigned char[(IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)];

	// Output Image.
	int* horDerOut = new int[(IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)];
	int* verDerOut = new int[(IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)];
	int* edgeOut = new int[(IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)];

	unsigned char* nonMaxSupressOut = new unsigned char[IMAGE_WIDTH * IMAGE_HEIGHT];

	// Get header info.
	for (int i = 0; i < HEADER_SIZE_BYTE; i++)
	{
		input_image_header[i] = getc(input_image_file);
	}
	// End of the for.

	/*---Width/Height/bitDepth Info extract from header file ---*/
	int width = *(int*)&input_image_header[BMP_WIDTH_OFFSET];
	int height = *(int*)&input_image_header[BMP_HEIGHT_OFFSET];
	int bitDepth = *(int*)&input_image_header[BMP_DEPTH_OFFSET];

	/*---If bitDepth is less or equal to 8 color table is received---*/
	if (bitDepth <= 8)
	{
		fread(input_image_color_table, sizeof(unsigned char), COLOR_TABLE_SIZE_BYTE, input_image_file);
	}
	// Else no operation.

	/*---Image data is taken---*/
	fread(input_image_data, sizeof(unsigned char), (IMAGE_WIDTH * IMAGE_HEIGHT), input_image_file);
	// Close input file.
	fclose(input_image_file);


	// WRITING IMAGE.
	FILE* output_image_file = fopen("images/output/lena_out.bmp", "wb");

	if (output_image_file == (FILE*)0)
	{
		printf("Output Image can't read!\n");
		return 0;
	}
	// Else no operation.

	// Write header table.
	fwrite(input_image_header, sizeof(unsigned char), HEADER_SIZE_BYTE, output_image_file);

	/*---If bitDepth is less or equal to 8 color table is written---*/
	if (bitDepth <= 8)
	{
		fwrite(input_image_color_table, sizeof(unsigned char), COLOR_TABLE_SIZE_BYTE, output_image_file);
	}
	// Else no operation.

	// Write image.

	memset(horDerOut, 0, (sizeof(int) * (IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)));
	memset(verDerOut, 0, (sizeof(int) * (IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)));
	memset(edgeOut, 0, (sizeof(int) * (IMAGE_WIDTH - 2) * (IMAGE_HEIGHT - 2)));

	memset(nonMaxSupressOut, 0, (sizeof(unsigned char) * IMAGE_WIDTH * IMAGE_HEIGHT));

	HorizontalDerivative(IMAGE_WIDTH, IMAGE_HEIGHT, input_image_data, horDerOut);
	VerticalDerivative(IMAGE_WIDTH, IMAGE_HEIGHT, input_image_data, verDerOut);
	EdgeImage(IMAGE_WIDTH, IMAGE_HEIGHT, horDerOut, verDerOut, edgeOut);

	NonmaximumSuppresion(IMAGE_WIDTH,
		IMAGE_HEIGHT,
		horDerOut,
		verDerOut,
		edgeOut,
		nonMaxSupressOut);

	fwrite(nonMaxSupressOut, sizeof(unsigned char), (IMAGE_WIDTH) * (IMAGE_HEIGHT), output_image_file);

	fclose(output_image_file);

	system("start images/lena512.bmp");
	//system("start images/barbara512.bmp");
	system("start images/output/lena_out.bmp");

	return 0;
}

void HorizontalDerivative(int Width, int Height, unsigned char* img, int* horDericativeImg)
{
	int index = 0;
	int center = 0;

	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			center = ((i * Width) + j);

			/* 1	0	-1
			   2	0	-2
			   1	0 	-1*/

			   //kenar yönü dikeyde.

			horDericativeImg[index] = (1 * img[(center - Width - 1)] + 0 * img[(center - Width)] + (-1) * img[(center - Width + 1)] +
				2 * img[(center - 1)] + 0 * img[center] + (-2) * img[(center + 1)] +
				1 * img[(center + Width - 1)] + 0 * img[(center + Width)] + (-1) * img[(center + Width + 1)]);
			index++;
		}
	}

	printf("Horizontal Derivative Success\n");
}


void VerticalDerivative(int Width, int Height, unsigned char* img, int* verDericativeImg)
{
	int index = 0;
	int center = 0;

	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			center = ((i * Width) + j);

			/* 1	2	1
			   0	0	0
			  -1   -2  -1*/
			  // kenar yönü yatayda

			verDericativeImg[index] = (1 * img[(center - Width - 1)] + 2 * img[(center - Width)] + 1 * img[(center - Width + 1)] +
				0 * img[(center - 1)] + 0 * img[center] + 0 * img[(center + 1)] +
				(-1) * img[(center + Width - 1)] + (-2) * img[(center + Width)] + (-1) * img[(center + Width + 1)]);
			index++;
		}
	}

	printf("Vertical Derivative Success\n");
}
void EdgeImage(int Width, int Height, int* horDericativeImg, int* verDericativeImg, int* edgeImg)
{
	int index = 0;
	int ii = 0;

	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			index = (i * (Width - 2) + j);

			edgeImg[index] = sqrt(pow(verDericativeImg[index], 2) + pow(horDericativeImg[index],2));
		}
	}
	printf("Edge Image Created\n");
}

void NonmaximumSuppresion(int Width, int Height,
	int* horizontalDerivativeImg,
	int* verticalDerivativeImg,
	int* edgeImg,
	unsigned char* nonmaximumSuppresionImg)
{
	int C = 0;    //center
	int index = 0;
	float Qangle = 0;
	int max_pixel = 0;

	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = (i * (Width - 2) + j);

			Qangle = atan2(verticalDerivativeImg[C], horizontalDerivativeImg[C]) * 180 / PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient direction

			// Açi bu aralikta ise 0' a yuvarla.
			if ((0 <= Qangle && Qangle < 22.5) || (337.5 <= Qangle && Qangle <= 360) ||
				(157.5 <= Qangle && Qangle < 202.5) || (0 > Qangle&& Qangle > -22.5) ||
				(-157.5 >= Qangle && Qangle > -202.5) || (-337.5 >= Qangle && Qangle >= -360))
			{
				// Ilk sutun.
				if (j == 0)
				{
					if (edgeImg[C] > edgeImg[C + 1])
					{
						edgeImg[C] = edgeImg[C];
					}
					else
					{
						edgeImg[C] = 0;
					}
				}
				// Son sutun
				else if (j == (Width - 2) - 1)
				{
					if (edgeImg[C] > edgeImg[C - 1])
					{
						edgeImg[C] = edgeImg[C];
					}
					else
					{
						edgeImg[C] = 0;
					}
				}
				else
				{
					if (edgeImg[C] > edgeImg[C - 1] && edgeImg[C] > edgeImg[C + 1])
					{
						edgeImg[C] = edgeImg[C];
					}
					else
					{
						edgeImg[C] = 0;
					}
				}
			}

			else if ((22.5 <= Qangle && Qangle < 67.5) ||
				(202.5 <= Qangle && Qangle < 247.5) ||
				(-112.5 >= Qangle && Qangle > -157.5) ||
				(-292.5 >= Qangle && Qangle > -337.5))		// acý bu araliktaysa caprazda(45 derece)
			{

				if ((j == 0 && i == 0) || (i == (Height - 2 - 1) && j == (Width - 2) - 1))
				{
					edgeImg[C] = edgeImg[C];
				}

				else if ((i == 0) || (j == (Width - 2) - 1))
				{
					if (edgeImg[C] > edgeImg[C + (Width - 2) - 1])
					{
						edgeImg[C] = edgeImg[C];
					}
					else
					{
						edgeImg[C] = 0;
					}
				}

				else if ((j == 0) || (i == (Height - 2) - 1))
				{
					if (edgeImg[C] > edgeImg[(C - (Width - 2) + 1)])
					{
						edgeImg[C] = edgeImg[C];
					}
					else
					{
						edgeImg[C] = 0;
					}
				}
				else
				{
					if (edgeImg[C] > edgeImg[(C - (Width - 2) + 1)] && edgeImg[C] > edgeImg[C + (Width - 2) - 1])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}

			}

			else if ((67.5 <= Qangle && Qangle < 112.5) || (247.5 <= Qangle && Qangle < 292.5) || (-67.5 >= Qangle && Qangle > -112.5) || (-247.5 >= Qangle && Qangle > -292.5))	// acý bu aralikta ise dikeyde (90 derece)
			{
				if (i == 0)
				{
					if (edgeImg[C] > edgeImg[C + (Width - 2)])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}
				else if (i == (Height - 2 - 1))
				{
					if (edgeImg[C] > edgeImg[(C - (Width - 2))])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}
				else
				{
					if (edgeImg[C] > edgeImg[C - (Width - 2)] && edgeImg[C] > edgeImg[C + (Width - 2)])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}
			}

			else if ((112.5 <= Qangle && Qangle < 157.5) || (292.5 <= Qangle && Qangle < 337.5) || (-22.5 >= Qangle && Qangle > -67.5) || (-202.5 >= Qangle && Qangle > -247.5))	// acý bu aralikta ise caprazda (135 derece)
			{
				if ((j == 0 && i == (Height - 2) - 1) || (i == 0 && j == (Width - 2) - 1))
				{
					edgeImg[C] = edgeImg[C];
				}

				else if (j == 0 || i == 0)
				{
					if (edgeImg[C] > edgeImg[C + (Width - 2) + 1])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}

				else if (i == (Height - 2 - 1) || j == (Width - 2 - 1))
				{
					if (edgeImg[C] > edgeImg[(C - (Width - 2) - 1)])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}

				else
				{
					if (edgeImg[C] > edgeImg[(C - (Width - 2) - 1)] && edgeImg[C] > edgeImg[C + (Width - 2) + 1])
						edgeImg[C] = edgeImg[C];
					else
						edgeImg[C] = 0;
				}

			}

		}
	}


	
	//goruntudeki max pixel degerini bulma
	max_pixel = 0;

	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			C = i * (Width) + j;

			if (C <= 260100)
			{
				if (edgeImg[C] > max_pixel)
				{
					max_pixel = edgeImg[C];
				}
			}
		}
	}
	


	for (int i = 1; i < Height - 1; i++)
	{
		for (int j = 1; j < Width - 1; j++)
		{
			C = i * (Width)+j;
			nonmaximumSuppresionImg[C] = 0;
		}
	}



	//goruntuyu binary cevirme
	for (int i = 0; i < Height - 2; i++)
	{
		for (int j = 0; j < Width - 2; j++)
		{
			C = i * (Width - 2) + j;
			int C2 = (i + 1) * Width + (j + 1);

			if (edgeImg[C] <= max_pixel / 8)
			{
				nonmaximumSuppresionImg[C2] = 0;
			}
			else
			{
				nonmaximumSuppresionImg[C2] = 255;
			}

		}
	}

	printf("Nonmaximum Suppresion Success\n");
}
