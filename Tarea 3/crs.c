#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#define N 500
#define M 500

int main(){

    int matriz[N*M], nnz = 0, contador = 0, indice = 0, fila = -1;
    int *val, *col_ind, *row_ptr, *u, *resultado;
    memset(matriz, 0, N*M*sizeof(int));
    // llenar la matriz
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( i==j ){
                matriz[j+i*M] = 1;
            }
        }
    }
    // contar los no ceros (nnz)
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[j+i*M] != 0){
                nnz++;
            }
        }
    }
    StartTimer();
    // llenar val y col_ptr
    val = (int*)malloc( nnz * sizeof(int));
    col_ind = (int*)malloc( nnz * sizeof(int) );
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( matriz[j+i*M] != 0){
                val[contador] = matriz[j+i*M];
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
            if( matriz[j+i*M] != 0){
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
    /*
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
    */
    // ---------------------------------------------
    // lleno el vector
    u = (int*)malloc( M * sizeof(int));
    for(int i=0;i<M;i++){
        u[i] = 1;
    }
    // Multiplicacion Matriz * u
    resultado = (int*)malloc( N * sizeof(int));
    #pragma acc data copyin(val[0:nnz],u[0:N],col_ind[0:nnz],row_ptr[0:N+1]) copyout(resultado[0:N])
    {
    #pragma acc parallel
    {   
        indice=0;
        #pragma acc loop
        for(int i=0 ; i < N ; i++){
            resultado[i] = 0;
            #pragma acc loop
            for(int j=0 ; j < (row_ptr[i+1] - row_ptr[i]) ; j++){
                resultado[i] += val[indice] * u[ col_ind[indice]-1 ];
                indice++;
            }
        }
    }            
    }
    /*
    // imprimir resultado
    for(int i=0; i < N; i++){
        printf("resultado[%d] = %d\n", i, resultado[i]);
    }
    */
    double runtime = GetTimer();
    printf(" total: %f s\n", runtime / 1000.f);
    
    free(val);
    free(col_ind);
    free(row_ptr);
    free(u);
    free(resultado);
}