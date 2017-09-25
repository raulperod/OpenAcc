#include <stdio.h>
#include <stdlib.h>

#define N 10
#define M 10
#define L 10

int main(){

    int nnz = 0, contador = 0, indice = 0, fila = -1, suma = 0;
    int *matriz, *val, *col_ind, *row_ptr, *u, *resultado;
    // llenar la matriz
    matriz = (int*)malloc( N * M * sizeof(int));
    for(int i=0 ; i < N ; i++){
        for(int j=0; j < M; j++){
            if( j >= i ){
                matriz[j+i*M] = 1;
            }else{
                matriz[j+i*M] = 0;
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
    // lleno la matriz
    u = (int*)malloc( M * L * sizeof(int));
    for(int i = 0 ; i < M ; i++){
        for(int j = 0 ; j < L ; j++){
            u[j+i*L] = 1;
        }
    }
    // Multiplicacion CRS * Matriz
    resultado = (int*)malloc( N * L * sizeof(int));
    #pragma acc data copyin(val[0:nnz],u[0:M*L],col_ind[0:nnz],row_ptr[0:N+1]) copyout(resultado[0:N*L])
    {
    #pragma acc parallel
    {
        #pragma acc loop
		for (int i = 0; i < N; i++){
			#pragma acc loop
			for (int j = 0; j < L; j++){
				suma = 0;
                #pragma acc loop
				for (int k = row_ptr[i]-1; k < row_ptr[i+1]-1; k++){
                    suma += val[k] * u[j + ( (col_ind[k]-1) * L) ];
                }
				resultado[j+i*L] = suma;
			}
		}
    }            
    }
    
    // imprimir resultado
    for(int i=0; i < N; i++){
        for(int j=0 ; j < L ; j++){
            printf("resultado[%d][%d] = %d\n", i, j, resultado[j+i*L]);
        }
    }
    
    free(val);
    free(col_ind);
    free(row_ptr);
    free(u);
    free(resultado);
}