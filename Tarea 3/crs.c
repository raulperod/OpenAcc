#include <stdio.h>
#define N 3
#define M 4 

int main(){

    int matriz[N][M], nnz = 0, contador = 0, indice = 0, fila = -1;
    int *val, *col_ind, *row_ptr;
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
    // llenar val y col_ptr
    val = (int*)malloc( nnz * sizeof(int));
    col_ind = (int*)malloc( nnz * sizeof(int) );
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[i][j] != 0){
                val[contador] = matriz[i][j];
                col_ind[contador] = j+1;
                contador++;
            }
        }
    }
    // llenar row_ptr
    row_ptr = (int*)malloc( (N+1) * sizeof(int) );
    contador = 0; // contador = 0, indice = 0, fila = -1
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[i][j] != 0){
                contador++;
                if( i != fila){
                    row_ptr[indice] = contador;
                    fila = i;
                    indice++;
                }
            }
        }
    }
    row_ptr[N] = nnz+1;
    // imprimir val
    for(int i=0; i < nnz; i++){
        printf("val[%d] = %d\n", i, val[i]);
    }
    // imprimir col_ind
    for(int i=0; i < nnz; i++){
        printf("col_ind[%d] = %d\n", i, col_ind[i]);
    }
    // imprimir row_ptr
    for(int i=0; i < (N+1); i++){
        printf("row_ptr[%d] = %d\n", i, row_ptr[i]);
    }
}