#include <stdio.h>
#include <stdlib.h>

void llenarMatriz1(int*, int, int);
void llenarMatriz2(int*, int, int);
void generarCRS(int*, int*, int*, int*, int*, int, int);

int main(){

    int N = 4, M = 4, L = 4;
    int nnz1 = 0, nnz2 = 0, suma = 0;
    int *matriz1, *val1, *col_ind1, *row_ptr1;
    int *matriz2, *val2, *col_ind2, *row_ptr2;
    int *resultado;
    // llenar la matrices
    matriz1 = (int*)malloc( N * M * sizeof(int));
    matriz2 = (int*)malloc( M * L * sizeof(int));
    llenarMatriz1(matriz1, N, M);
    llenarMatriz2(matriz2, M, L);
    //-------------------
    //generar val,col_ind,row_ptr
    generarCRS(matriz1, val1, col_ind1, row_ptr1, &nnz1, N, M);
    generarCRS(matriz2, val2, col_ind2, row_ptr2, &nnz2, M, L);
    //---------------------------
    // Multiplicacion CRS * CRS
    
    resultado = (int*)malloc( N * L * sizeof(int));
    #pragma acc data copyin(val1[0:nnz1],col_ind1[0:nnz1],row_ptr1[0:N+1],val2[0:nnz2],col_ind2[0:nnz2],row_ptr2[0:M+1] ) copyout(resultado[0:N*L])
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
        
    free(matriz1);
    free(val1);
    free(col_ind1);
    free(row_ptr1);
    free(matriz2);
    free(val2);
    free(col_ind2);
    free(row_ptr2);
    free(resultado);
}

void llenarMatriz1(int *mat, int n, int m){

    int *matriz;
    matriz = mat;

    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( j >= i ){
                matriz[j+i*m] = 1;
            }else{
                matriz[j+i*m] = 0;
            }
        }
    }
}

void llenarMatriz2(int *mat, int n, int m){

    int *matriz;
    matriz = mat;

    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( j <= i ){
                matriz[j+i*m] = 1;
            }else{
                matriz[j+i*m] = 0;
            }
        }
    }     
}

void generarCRS(int *mat, int *v, int *c, int *p, int *nz, int n, int m){

    int *matriz, *val, *col_ind, *row_ptr;
    int nnz = 0, contador = 0, indice = 0, fila = -1;
    matriz = mat;
    val = v;
    col_ind = c;
    row_ptr = p;

    // contar los no ceros (nnz)
    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( matriz[j+i*m] != 0){
                nnz++;
            }
        }
    }
    // llenar val y col_ptr
    val = (int*)malloc( nnz * sizeof(int));
    col_ind = (int*)malloc( nnz * sizeof(int) );
    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( matriz[j+i*m] != 0){
                val[contador] = matriz[j+i*m];
                col_ind[contador] = j+1;
                contador++;
            }
        }
    }
    // llenar row_ptr
    row_ptr = (int*)malloc( (n+1) * sizeof(int) );
    contador = 0; // contador = 0, indice = 0, fila = -1
    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( matriz[j+i*m] != 0){
                contador++;
                if( i != fila){
                    row_ptr[indice] = contador;
                    fila = i;
                    indice++;
                }
            }
        }
    }
    row_ptr[n] = nnz+1;
    *nz = nnz;
    /*
    // imprimir val
    for(int i=0; i < nnz; i++){
        printf("val[%d] = %d\n", i, val[i]);
    }
    // imprimir col_ind
    for(int i=0; i < nnz; i++){
        printf("col_ind[%d] = %d\n", i, col_ind[i]);
    }
    // imprimir row_ptr
    for(int i=0; i < (n+1); i++){
        printf("row_ptr[%d] = %d\n", i, row_ptr[i]);
    }
    printf("------------------------------\n");
    */    
}