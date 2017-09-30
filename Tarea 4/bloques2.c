#include <stdio.h>
#include <stdlib.h>

void llenarMatriz(int*, int, int);

int main(){

    int NC = 2, M = 2, NUM_BLOQUES = 2;
    int *matriz, *u, *resultado;
    int suma;

    matriz = (int*)malloc( NC * NC * sizeof(int));
    u = (int*)malloc( NC * NUM_BLOQUES * M * sizeof(int));
    resultado = (int*)malloc( NC * NUM_BLOQUES * M * sizeof(int));

    llenarMatriz( matriz, NC, NC);
    llenarMatriz( u, NC * NUM_BLOQUES, M);
    #pragma acc data copyin(matriz[0:NC*NC], u[0:NC*NUM_BLOQUES]) copyout(resultado[0:NC*NUM_BLOQUES])
    {
    #pragma acc parallel
    { 
        #pragma acc loop   
        for(int i=0 ; i < NUM_BLOQUES ; i++){
            #pragma acc loop   
            for(int j=0 ; j < NC ; j++){
                #pragma acc loop   
                for(int k=0 ; k < M ; k++){
                    suma = 0;
                    #pragma acc loop   
                    for(int l=0 ; l < NC ; l++){
                        suma += matriz[l + j*NC] * u[k + l*M + i*NC*M];
                    }
                    resultado[k + j*M + i*NC*M] = suma;
                }
            }
        }  
    }
    }
/*
    for(int i=0 ; i < NC*NUM_BLOQUES ; i++){
        for(int j=0 ; j < M ; j++){
            printf("R[%d][%d] = %d\n", i, j, resultado[j+i*M]);
        }
    }
*/
    free(matriz);
    free(u);
    free(resultado);
    return 0;
}

void llenarMatriz(int *mat, int n, int m){
    int *matriz;
    matriz = mat;

    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            matriz[j+i*m] = j+i*m+1;
            //printf("matriz[%d] = %d\n", j+i*m, matriz[j+i*m]);
        }
    }

}