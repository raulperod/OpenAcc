#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "timer.h"
// Para PI, Producto punto
#define N 20000
// Para las matrices
#define M_N 100
#define M_M 100
#define M_L 100
// Para los calor 2d
#define NX_2D 100
#define NY_2D 100
// Para los Bloques
#define B_NC 10
#define B_M 10
#define B_NUM_BLOQUES 100
// Repeticiones y numero de n's
#define NUM_N 10
#define REP 100

void calcularTiempoPI(int);
void calcularTimepoProductoPunto(int);
void calcularTiempoSumaMatrices(int, int);
void calcularTiempoMultiplicacionMatrices(int, int, int);
void calcularTiempoCalor2D(int, int);
void calcularTiempoCRS(int, int, int);
void calcularTiempoBloques(int, int, int);

int main(){

	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoPI(N*i);
	}
  	
	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTimepoProductoPunto(N*i);
	}

    for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoSumaMatrices(M_N*i, M_M*i);
	}

	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoMultiplicacionMatrices(M_N*i, M_M*i, M_L*i);
	}

	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoCalor2D(NX_2D, NY_2D);
	}

	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoCRS(M_N*i, M_M*i, M_L*i);
	}

	for(int i=1 ; i < NUM_N+1 ; i++){
		calcularTiempoBloques(B_NC*i, B_M, B_NUM_BLOQUES*i);
	}

   	return 0;
}

void calcularTiempoPI(int n){
	float *x, *y;
	int contador = 0;
	double runtime = 0;

	x = (float*)malloc( n * sizeof(float));
	y = (float*)malloc( n * sizeof(float));
	
	srand(time(NULL));
	for(int i = 0 ; i < n ; i++ ) {
		x[i] = rand() / (RAND_MAX + 1.0f);
		y[i] = rand() / (RAND_MAX + 1.0f);
	}

	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(x[0:n], y[0:n]), copyout(contador)
		{
		#pragma acc parallels
		{
			for(int i = 0 ; i < n ; i++){
				if( (x[i] * x[i] + y[i] * y[i]) <= 1.0f){
					contador++;
				}
			}
		}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("PI ) Tiempo Total [N = %d]: %f s\n", n, runtime / 1000.f);
	//printf("Pi: %f\n", 4.0 * contador / N );
	free(x);
	free(y);
}

void calcularTimepoProductoPunto(int n){
	float *a, *b;
	float productoPunto = 0.0f;
	double runtime = 0;

	a = (float*)malloc( n * sizeof(float));
	b = (float*)malloc( n * sizeof(float));

	for(int i = 0 ; i < n ; i++){
      a[i] = 1.0f;
      b[i] = 2.0f;
	}
	
	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(a[0:n], b[0:n]), copyout(productoPunto)
		{
		#pragma acc kernels
		{
			//	Calculo el vector con los productos
			for(int i = 0 ; i < n ; i++){
				productoPunto += a[i] * b[i];
			}
		}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("PP ) Tiempo Total [N = %d]: %f s\n", n, runtime / 1000.f);
	//printf("El producto punto es: %f\n", productoPunto);
	free(a);
	free(b);
}

void calcularTiempoSumaMatrices(int n, int m){
	float *a, *b, *c;
	double runtime = 0;
	// pido memoria
	a = (float*)malloc( n * m * sizeof(float));
	b = (float*)malloc( n * m * sizeof(float));
	c = (float*)malloc( n * m * sizeof(float));
	// Lleno las matrices
	for(int i = 0 ; i < n*m ; i++){
		a[i] = 1.0f;
		b[i] = 2.0f;
	}
	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(a[0:n*m],b[0:n*m]), copyout(c[0:n*m])
		{
		#pragma acc parallels // kernels
		{
			#pragma acc loop
			for(int i = 0 ; i < n*m ; i++){
				c[i] = a[i] + b[i];
			}
		}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("SM ) Tiempo Total [N = %d, M = %d]: %f s\n", n, m, runtime / 1000.f);
	//printf("%f %f \n", c[0], c[N*M-1]);
	free(a);
	free(b);
	free(c);
}

void calcularTiempoMultiplicacionMatrices(int n, int m, int l){
	float *a, *b, *c;
	float suma = 0;
    double runtime = 0;
	// pido memoria
	a = (float*)malloc( n * m * sizeof(float));
	b = (float*)malloc( m * l * sizeof(float));
	c = (float*)malloc( n * l * sizeof(float));

    for(int i = 0 ; i < n*m ; i++) {
        a[i] = 1.0f;
    }

    for(int i = 0 ; i < m*l ; i++) {
        b[i] = 1.0f;
    }
   	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(a[0:n*m],b[0:n*m]), copyout(c[0:n*m])
		{
		#pragma acc parallels
		{
			#pragma acc loop
			for(int i = 0; i < n; i++){
				#pragma acc loop
				for(int j = 0; j < l; j++){
					suma = 0.0;
					#pragma acc loop
					for(int k = 0; k < m; k++){
						suma += a[k+i*m] * b[j+k*n];
					}
					c[j+i*n] = suma;
				}
			}
		}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("MM ) Tiempo Total [N = %d, M = %d, L = %d]: %f s\n", n, m, runtime / 1000.f);
	/*
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("c[%i][%i]: %f\n", i, j, c[j+i*N]);
        }
    }
    */
	free(a);
	free(b);
	free(c);
}

void calcularTiempoCalor2D(int nx, int ny){
	float *u, *ut;
	double runtime = 0;
	float gama = 0.001f;
	float PI = 3.1416f;
	float h = (float)(1.0f/nx); //0.025;
	float h2 = h * h;
	float dt=0.001;
	float lap;

	u = (float*)malloc( nx * ny * sizeof(float));
	ut = (float*)malloc( nx * ny * sizeof(float));
	
	// Condición inicial
	for(int i = 1; i < nx; i++ ){
		for(int j = 1; j < ny; j++ ){
			if ( (i)*(i)+(j)*(j) > nx*ny && (i)*(i)+(j)*(j) < 2*nx*ny ){
				u[j+i*ny] = 3.0f;
			}else{
				u[j+i*ny] = 0.0f;
			}
		}
	}
	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(u[0:nx*ny], ut[0:nx*ny])
		{
			/* Main Iteration loop */
			for(int ntime = 0; ntime < 100000; ntime++){
				/* Boundary Conditions: No-flux*/
				#pragma acc kernels
				{
					for(int i = 1; i < nx; i++ ){
						u[1+i*ny] = u[2+i*ny];
						u[(nx)+i*ny] = u[(nx-1)+i*ny];
					}
				}
				#pragma acc kernels
				{
					for(int i = 1; i < ny; i++ ){
						u[i+ny] = u[i+ny*2];
						u[i+ny*(ny-1)] = u[i+ny*ny];
					}
				}
				/* Euler Scheme */
				#pragma acc kernels
				{
					for(int i = 0; i < nx; i++ ){
						for(int j = 0; j < ny; j++ ){
							if(i*(ny-i-1)*j*(ny-j-1)!=0){
								ut[i+ny*j] = u[i+ny*j] + (u[i-1+ny*j]+u[i+1+ny*j]+u[i+(j-1)*ny]
									+u[i+(j+1)*ny] -4.0*u[i+ny*j])*dt*gama/h2;
							}
						}
					}
				}
				/* Update of the mesh */
				#pragma acc kernels
				{
					for(int i = 1; i < nx; i++ ){
						for(int j = 1; j < ny; j++ ){
							u[j+i*ny] = ut[j+i*ny];
						}
					}
				}

			}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("Calor 2D ) Tiempo Total [Nx = %d, Ny = %d]: %f s\n", nx, ny, runtime / 1000.f);
	free(u);
	free(ut);
}

void calcularTiempoCRS(int n, int m, int l){
	double runtime = 0;
	int nnz = 0, contador = 0, indice = 0, fila = -1, suma = 0;
    int *matriz, *val, *col_ind, *row_ptr, *u, *resultado;
    // llenar la matriz
    matriz = (int*)malloc( n * m * sizeof(int));
    for(int i=0 ; i < n ; i++){
        for(int j=0; j < m; j++){
            if( j >= i ){
                matriz[j+i*m] = 1;
            }else{
                matriz[j+i*m] = 0;
            }
        }
    }
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
    
    // lleno la matriz
    u = (int*)malloc( m * l * sizeof(int));
    for(int i = 0 ; i < m ; i++){
        for(int j = 0 ; j < l ; j++){
            u[j+i*l] = 1;
        }
    }
    
    // Multiplicacion CRS * Matriz
    resultado = (int*)malloc( n * l * sizeof(int));
	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(val[0:nnz],u[0:m*l],col_ind[0:nnz],row_ptr[0:n+1]) copyout(resultado[0:n*l])
		{
		#pragma acc parallel
		{
			#pragma acc loop
			for (int i = 0; i < n; i++){
				#pragma acc loop
				for (int j = 0; j < l; j++){
					suma = 0;
					#pragma acc loop
					for (int k = row_ptr[i]-1; k < row_ptr[i+1]-1; k++){
						suma += val[k] * u[j + ( (col_ind[k]-1) * l) ];
					}
					resultado[j+i*l] = suma;
				}
			}
		}            
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("CRS ) Tiempo Total [N = %d, M = %d, L = %d]: %f s\n", n, m, l, runtime / 1000.f);

	free(val);
    free(col_ind);
    free(row_ptr);
    free(u);
    free(resultado);
}

void calcularTiempoBloques(int nc, int m, int num_bloques){
	double runtime = 0;
    int *matriz, *u, *resultado;
    int suma;

    matriz = (int*)malloc( nc * nc * sizeof(int));
    u = (int*)malloc( nc * num_bloques * m * sizeof(int));
    resultado = (int*)malloc( nc * num_bloques * m * sizeof(int));

    for(int i=0 ; i < nc ; i++){
        for(int j=0; j < nc; j++){
            matriz[j+i*nc] = j+i*nc+1;
        }
    }
    
	for(int i=0 ; i < nc*num_bloques ; i++){
        for(int j=0; j < m; j++){
            u[j+i*m] = j+i*m+1;
        }
    }

	for(int r=0 ; r<REP ; r++){
		StartTimer();
		#pragma acc data copyin(matriz[0:nc*nc], u[0:nc*num_bloques]) copyout(resultado[0:nc*num_bloques])
		{
		#pragma acc parallel
		{ 
			#pragma acc loop   
			for(int i=0 ; i < num_bloques ; i++){
				#pragma acc loop   
				for(int j=0 ; j < nc ; j++){
					#pragma acc loop   
					for(int k=0 ; k < m ; k++){
						suma = 0;
						#pragma acc loop   
						for(int l=0 ; l < nc ; l++){
							suma += matriz[l + j*nc] * u[k + l*m + i*nc*m];
						}
						resultado[k + j*m + i*nc*m] = suma;
					}
				}
			}  
		}
		}
		runtime += GetTimer();
	}
	runtime /= REP;
	printf("BLOQUES ) Tiempo Total [NC = %d, M = %d, NUM_BLOQUES = %d]: %f s\n", nc, m, num_bloques, runtime / 1000.f);
	/*
		for(int i=0 ; i < nc*num_bloques ; i++){
			for(int j=0 ; j < m ; j++){
				printf("R[%d][%d] = %d\n", i, j, resultado[j+i*m]);
			}
		}
	*/
    free(matriz);
    free(u);
    free(resultado);
}