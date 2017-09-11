#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "timer.h"

#define N 100000

int main(){

	float x[N], y[N];
	int contador = 0;

	memset(x, 0, N*sizeof(float));
	memset(y, 0, N*sizeof(float));

	srand(time(NULL));
	for(int i = 0 ; i < N ; i++ ) {
		x[i] = rand() / (RAND_MAX + 1.0f);
		y[i] = rand() / (RAND_MAX + 1.0f);
		// printf("Punto (X%i, Y%i) = (%f, %f)\n", i, i, x[i], y[i]);
	}

	StartTimer();
	#pragma acc data copyin(x, y), copyout(contador)
	{
   #pragma acc parallels
   {
      for(int i = 0 ; i < N ; i++){
         if( (x[i] * x[i] + y[i] * y[i]) <= 1.0f){
            contador++;
         }
      }
	}
	}
	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
	//printf("Pi: %f\n", 4.0 * contador / N );
   return 0;
}
