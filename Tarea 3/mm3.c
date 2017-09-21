#include <stdio.h>
#include <stdlib.h>
#define N 500
#define M 500
#define L 1

int main(){

	float a[N*M], b[M*L], c[N*L];
	float suma;

	memset(a, 0, N*M*sizeof(float));
	memset(b, 0, M*L*sizeof(float));
  	memset(c, 0, N*L*sizeof(float));

	for(int i = 0 ; i < N ; i++) {
		for(int j=0 ; j < M ; j++){
			if( i == j ){
				a[j + i*M] = 1.0f;
			}
		}
	}

	for(int i = 0 ; i < M*L ; i++) {
    	b[i] = 1.0f;
	}

	#pragma acc data copyin(a, b), copyout(c)
	{
	#pragma acc parallels
	{
		#pragma acc loop
		for (int i = 0; i < N; i++){
			#pragma acc loop
			for (int j = 0; j < L; j++){
				suma = 0.0;
				#pragma acc loop
				for (int k = 0; k < M; k++){
					suma += a[k+i*M] * b[j+k*L];
				}
				c[j+i*L] = suma;
			}
		}
	}
	}
	
	for (int i= 0; i < N*L; i++){
		printf("c[%i]: %f\n", i, c[i]);
	}
		
  	return 0;
}
