#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"
#define N 500

int main(){

	float a[N*N], b[N*N], c[N*N];
	float suma;

	memset(a, 0, N*N*sizeof(float));
	memset(b, 0, N*N*sizeof(float));
	memset(c, 0, N*N*sizeof(float));

	for(int i = 0 ; i < N*N  ; i++) {
    	a[i] = 1.0f;
  		b[i] = 1.0f;
	}
	StartTimer();
	#pragma acc data copyin(a, b), copyout(c)
	{
   #pragma acc parallels
   {
      #pragma acc loop
      for (int i = 0; i < N; i++){
         #pragma acc loop
         for (int j = 0; j < N; j++){
            suma = 0.0;
            #pragma acc loop
            for (int k = 0; k < N; k++){
               suma += a[k+i*N] * b[j+k*N];
            }
            c[j+i*N] = suma;
         }
      }
	}
	}
   /*
   for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
         printf("c[%i][%i]: %f\n", i, j, c[j+i*N]);
      }
   }
   */
	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
	return 0;
}
