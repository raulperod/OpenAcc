#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"

int main( int argc, char* argv[] )
{
	int N =1000;
	int M = 1000;
	float a[N*M], b[N*M], c[N*M];
	// pido memoria
	memset(a, 0, N*M*sizeof(float));
	memset(b, 0, N*M*sizeof(float));
	memset(c, 0, N*M*sizeof(float));
	// Lleno las matrices
	for(int i = 0 ; i < N*M ; i++){
			a[i] = 1.0f;
			b[i] = 2.0f;
	}

	StartTimer();

	#pragma acc data copyin(a,b), copyout(c)
	{
	#pragma acc parallels // kernels
	{
	#pragma acc loop
		for(int i = 0 ; i < N ; i++){
	#pragma acc loop
			for(int j = 0; j < M; j++){
				c[j+i*N] = a[j+i*N] + b[j+i*N];
			}
		}
	}
	}

	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
	//printf("%f %f \n", c[0], c[N*M-1]);
  return 0;
}
