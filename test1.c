#include<stdio.h>
void proc(long a1, long* a1p, int a2, int* a2p, short a3, short* a3p, char a4, char* a4p) {
	*a1p += a1;
	*a2p += a2;
	*a3p += a3;
	*a4p += a4;
}

int f(int a) 
{
	long x =  12224;
	int y = 3243;
	short z = 12;
	char t = 'a';
	proc(x, &x, y,&y, z, &z, t, &t);
	return 1;
}

