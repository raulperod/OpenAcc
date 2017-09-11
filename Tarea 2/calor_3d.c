#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <accelmath.h> // Note: when compiling in linux, also type -lm for the math.h to work
#include <openacc.h>
#include "timer.h"
//#include <math.h>

void main(){

   int i,j,k,ntime,nsteps,nx=256,ny=256,nz=256;
   float u[nx+1][ny+1][nz+1],ut[nx][ny][nz];
   float gama,PI;
   float lap,h,h2,dt;
   char nombre[20];
   FILE *fl;
   /* Parameters to be used in the model */
   PI=3.1416f;
   gama=0.001f;
   /* Numerical constants for the euler method */
   h=(float)(1.0f/nx); //0.025;
   h2=h*h;
   dt=0.001;
   nsteps=3000000;
   // Setting the device
   // acc_set_device_num(0,acc_device_nvidia);
   // Condición inicial
   for( k = 1; k < nz; k++){
      for( i = 1; i < nx; i++ ){
         for( j = 1; j < ny; j++ ){
            if ( (i)*(i)+(j)*(j) > 1500 && (i)*(i)+(j)*(j) < 2500 ){
               u[i][j][k] = 3.0f;
            }else{
               u[i][j][k] = 0.0f;
            }
         }
      }
   }
   StartTimer();
   #pragma acc data copyin(u,ut)
   {
      /* Main Iteration loop */
      for( ntime = 0; ntime < 500; ntime++){
         /* Boundary Conditions: No-flux*/
         #pragma acc kernels
         {
            for(k = 1; k < nz; k++){
               for( i = 1; i < nx; i++ ){
                  u[i][1][k] = u[i][3][k];
                  u[i][ny-1][k] = u[i][ny-2][k];
               }
            }
         }
         #pragma acc kernels
         {
            for (k=1; k < nz; k++){
               for( j = 1; j < ny; j++ ){
                  u[1][j][k] = u[3][j][k];
                  u[nx-1][j][k] = u[nx-2][j][k];
               }
            }
         }
         /* Euler Scheme */
         #pragma acc kernels
         {
            for( k = 1; k < nz; k++) {
               for( i = 1; i < nx; i++ ){
                  for( j = 1; j < ny; j++ ){
                     lap = u[i+1][j][k] + u[i-1][j][k] + u[i][j-1][k] + u[i][j+1][k]
                           + u[i][j][k+1] + u[i][j][k-1] - 6.*u[i][j][k];
                     ut[i][j][k] = u[i][j][k]+lap*dt*gama/h2;
                  }
               }
            }
         }
         /* Update of the mesh */
         #pragma acc kernels
         {
            for ( k = 1; k < nz; k++){
               for( i = 1; i < nx; i++ ){
                  for( j = 1; j < ny; j++ ){
                     u[i][j][k] = ut[i][j][k];
                  }
               }
            }
         }
         if(ntime%100 == 0){
            printf("%d iteraciones \n", ntime);
            sprintf(nombre,"calor_3d_%i.csv",ntime);
            puts(nombre);
            #pragma acc update host(u)
            fl=fopen(nombre,"w");
            for ( k = 1; k < nz; k++){
               for( i = 1; i < nx; i++ ){
                  for( j = 1; j < ny; j++ ){
                     if (u[i][j][k] > 0.1f){
                        fprintf(fl, "%d , %d , %d , %f \n", i,j,k,u[i][j][k]);
                     }
                  }
               }
            }
         }   //este es del if
      }
   }
   close(fl);
   double runtime = GetTimer();
   printf(" total: %f s\n", runtime / 1000.f);
   printf("Ya terminamos!!!!!! \n");
   return;
}
