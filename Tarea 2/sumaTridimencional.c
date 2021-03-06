#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"

#define N 80
#define M 80
#define L 80

int main(){

	float a[N][M][L], b[N][M][L], c[N][M][L];

	memset(a, 0, N*M*L*sizeof(float));
	memset(b, 0, N*M*L*sizeof(float));
   memset(c, 0, N*M*L*sizeof(float));

	for(int i = 0 ; i < N ; i++) {
      for(int j = 0 ; j < M ; j++) {
         for(int k = 0 ; k < L ; k++) {
            a[i][j][k] = 1.0f;
            b[i][j][k] = 1.0f;
         }
      }
	}
	StartTimer();
	#pragma acc data copyin(a, b), copyout(c)
	{
   #pragma acc parallels
   {
      #pragma acc loop
      for (int i = 0; i < N; i++){
         #pragma acc loop
         for (int j = 0; j < M; j++){
            #pragma acc loop
            for (int k = 0; k < L; k++){
               c[i][j][k] = a[i][j][k] + b[i][j][k];
            }
         }
	 	}
	}
	}
	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
   return 0;
}
