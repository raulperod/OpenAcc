#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"

int main( int argc, char* argv[] )
{
	int N = 100000;
	float a[N], b[N];
  	float productoPunto = 0.0f;
	// pido memoria
	memset(a, 0, N*sizeof(float));
	memset(b, 0, N*sizeof(float));

	for(int i = 0 ; i < N ; i++){
      a[i] = 1.0f;
      b[i] = 2.0f;
	}

   StartTimer();

	#pragma acc data copyin(a,b), copyout(productoPunto)
	{
	#pragma acc kernels
	{
		//	Calculo el vector con los productos
		for(int i = 0 ; i < N ; i++){
			productoPunto += a[i] * b[i];
		}
	}
	}

	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
	//printf("El producto punto es: %f\n", productoPunto);
   return 0;
}
