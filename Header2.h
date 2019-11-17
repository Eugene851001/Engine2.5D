#pragma once

#define LETTER_HEIGHT 7
#define LETTER_WIDTH 7

const char LetterS[LETTER_HEIGHT][LETTER_WIDTH] = { {0, 0, 0, 'S', 0, 0, 0},
													{0, 0, 'S', 0, 'S', 0, 0},
													{0, 0, 'S', 0, 0, 0, 0},
													{0, 0, 0, 'S', 0, 0, 0},
													{0, 0, 0, 0, 'S', 0, 0},
													{0, 0, 'S', 0, 'S', 0, 0},
													{0, 0, 0, 'S', 0, 0, 0 } };

void DrawLetterS(char* screen, int ScreenWidth, int ScreenHeight, int LetterWidth, 
	int LetterHeight, int x, int y)
{
	int i, j;
	for (i = 0; i < LetterHeight; i++)
			for (j = 0; j < LetterWidth; j++)
				screen[(i + y) * ScreenWidth + (j + x)] =
}
