#include <stdio.h>
#include <stdlib.h>

void llenarVector(int*, int);
void llenarMatriz(int*, int);

int main(){

    int NC = 3, NUM_BLOQUES = 2;
    int *matriz, *u, *resultado;

    matriz = (int*)malloc( NC * NC * sizeof(int));
    u = (int*)malloc( NC * NUM_BLOQUES * sizeof(int));
    resultado = (int*)malloc( NC * NUM_BLOQUES * sizeof(int));

    llenarMatriz( matriz, NC);
    llenarVector( u, NC * NUM_BLOQUES);

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
            matriz[j+i*n] = j + i * n + 1;
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