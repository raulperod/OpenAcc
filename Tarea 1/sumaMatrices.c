#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"

int main( int argc, char* argv[] )
{
	int N =1000;
	float a[N][N], b[N][N], c[N][N];
	// pido memoria
	memset(a, 0, N*N*sizeof(float));
	memset(b, 0, N*N*sizeof(float));
	memset(c, 0, N*N*sizeof(float));
	// Lleno las matrices
	for(int i = 0 ; i < N ; i++){
		for(int j = 0; j < N; j++){
			a[i][j] = 1.0f;
			b[i][j] = 2.0f;
		}
	}

	StartTimer();

	#pragma acc data copyin(a,b), copyout(c)
	{
	#pragma acc kernels
	{
		//	Realizo la suma
		for(int i = 0 ; i < N ; i++){
			for(int j = 0; j < N; j++){
				c[i][j] = a[i][j] + b[i][j];
			}
		}
	}
	}

	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
	//printf("%f %f \n", c[0][0], c[999][999]);
    return 0;
}
