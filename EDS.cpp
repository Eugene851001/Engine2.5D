#include "EDS.h"

#define sqr(a) a * a

int EDS::GetHash(char* data, int size)
{
	int h0 = SecretStartVaue;
	int h;
	for (int i = 0; i < size; i++)
	{
		h = (sqr(h0 + (unsigned char) data[i])) % R;
		h0 = h;
	}
	return h0;
}
