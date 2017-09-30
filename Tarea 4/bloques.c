#include <stdio.h>
#include <stdlib.h>

void llenarVector(int*, int);
void llenarMatriz(int*, int);

int main(){

    int NC = 2, NUM_BLOQUES = 3;
    int *matriz, *u, *resultado;
    int suma;

    matriz = (int*)malloc( NC * NC * sizeof(int));
    u = (int*)malloc( NC * NUM_BLOQUES * sizeof(int));
    resultado = (int*)malloc( NC * NUM_BLOQUES * sizeof(int));

    llenarMatriz( matriz, NC);
    llenarVector( u, NC * NUM_BLOQUES);
    #pragma acc data copyin(matriz[0:NC*NC], u[0:NC*NUM_BLOQUES]) copyout(resultado[0:NC*NUM_BLOQUES])
    {
    #pragma acc parallel
    {    
        #pragma acc loop    
        for(int i=0 ; i < NUM_BLOQUES ; i++){
            #pragma acc loop
            for(int j=0 ; j < NC ; j++){
                suma = 0;
                #pragma acc loop
                for(int k=0 ; k < NC ; k++){
                    suma += matriz[k+j*NC] * u[i*NC+k];
                }
                resultado[i*NC+j] = suma;
            }
        }   
    }
    }

    for(int i=0 ; i < NC*NUM_BLOQUES ; i++){
        printf("R[%d] = %d\n", i, resultado[i]);
    }

    free(matriz);
    free(u);
    free(resultado);
    return 0;
}

void llenarMatriz(int *mat, int n){
    int *matriz;
    matriz = mat;

    for(int i=0 ; i < n ; i++){
        for(int j=0; j < n; j++){
            matriz[j+i*n] = j+i*n+1;
        }
    }

}

void llenarVector(int *vec, int n){
    int *vector;
    vector = vec;

    for(int i=0 ; i < n ; i++){
        vector[i] = 1;
    }
}