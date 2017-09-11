#include <stdio.h>
#define N 3
#define M 4 

int main(){

    int matriz[N][M], nnz = 0;
    int *val, *aux, *col_ind, *row_ptr;
    memset(matriz, 0, N*M*sizeof(int));
    // llenar la matriz
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( i == j || j > i){
                matriz[i][j] = 1;
            }
        }
    }
    // contar los no ceros (nnz)
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[i][j] != 0){
                nnz++;
            }
        }
    }
    // llenar val
    val = malloc( nnz * sizeof(int));
    aux = val;
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[i][j] != 0){
                *aux = matriz[i][j];
                aux++;     
            }
        }
    }

}