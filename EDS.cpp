#include "EDS.h"

#define sqr(a) a * a

int EDS::GetHash(char* data, int size)
{
	int h0 = 133;
	int h;
	for (int i = 0; i < size; i++)
	{
		h = sqr(h0 + data[i]) % R;
		h0 = h;
	}
	return h0;
}
