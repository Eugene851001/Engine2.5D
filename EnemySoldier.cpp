#include "EnemySoldier.h"

void EnemySoldier::InitTexture()
{
	char tempTexture[4][4] = {  {'$', '$', '$', '$'},
								{'$', ' ', ' ', '$'},
								{'$', ' ', ' ', '$'},
								{'$', '$', '$', '$'} };
	int i;
	int j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			texture[i][j] = tempTexture[i][j];
		}
}
