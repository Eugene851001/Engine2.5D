#pragma once

class EDS
{
	const int R = 2827;
	const int SecretStartVaue = 133;
public:
	int GetHash(char *date, int size);
//	int ExtendedEuclid(int a, int b, int& x, int& y);
//	void GetKeys(int& e, int& d);
//	int FastPow(int a, int z, int n);
};

