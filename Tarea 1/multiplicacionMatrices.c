#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "timer.h"

#define N 500

int main(){

	float a[N][N], b[N][N], c[N][N];
	float suma;

	memset(a, 0, N*N*sizeof(float));
	memset(b, 0, N*N*sizeof(float));
  	memset(c, 0, N*N*sizeof(float));

	for(int i = 0 ; i < N ; i++ ) {
		for(int j = 0 ; j < N ; j++ ) {
	  		a[i][j] = 1.0f;
	  		b[i][j] = 1.0f;
	  	}
	}


	StartTimer();
	#pragma acc data copyin(a, b), copyout(c)
	{
		#pragma acc parallels
		{
		  	for (int i = 0; i < N; i++){
		    	for (int j = 0; j < N; j++){
					suma = 0.0;
					for (int k = 0; k < N; k++){
		        		suma += a[i][k] * b[k][j];
		      		}
					c[i][j] = suma;
		    	}
     	  	}
		}
	}
/*
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("c[%i][%i]: %f\n", i, j, c[i][j]);
    }
  }
*/
	double runtime = GetTimer();
	printf(" total: %f s\n", runtime / 1000.f);
  return 0;
}
