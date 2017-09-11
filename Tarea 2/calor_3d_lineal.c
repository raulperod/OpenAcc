#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <accelmath.h> // Note: when compiling in linux, also type -lm for the math.h to work
#include <openacc.h>
#include "timer.h"
//#include <math.h>

void main(){

   int i,j,k,ntime,nsteps,nx=256,ny=256,nz=256;
   float u[(nx+2)*(ny+2)*(nz+2)],ut[(nx+2)*(ny+2)*(nz+2)];
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
   // Condición inicial u
   for( i = 1; i < nx+1; i++){
      for( j = 1; j < ny+1; j++ ){
         for( k = 1; k < nz+1; k++ ){
            if ( (i)*(i)+(j)*(j) > 1500 && (i)*(i)+(j)*(j) < 2500 ){
               u[k+nz*(j+i*ny)] = 3.0f;
            }else{
               u[k+nz*(j+i*ny)] = 0.0f;
            }
         }
      }
   }
	// Condición inicial ut
   for( i = 0; i < nx; i++){
      for( j = 0; j < ny; j++ ){
         for( k = 0; k < nz; k++ ){
            ut[k+nz*(j+i*ny)] = 0.0f;
         }
      }
   }
   StartTimer();
   #pragma acc data copyin(u,ut)
   {
      /* Main Iteration loop */
      for( ntime = 0; ntime < 500; ntime++){
         /* Boundary Conditions: No-flux*/
         #pragma acc parallel
         {
            #pragma acc loop
            for( i = 1; i < nx+1; i++ ){
               #pragma acc loop
               for(k = 1; k < nz+1; k++){
                  u[k+nz*(1+i*ny)] = u[k+nz*(3+i*ny)];
                  u[k+nz*((ny-1)+i*ny)] = u[k+nz*((ny-2)+i*ny)];
               }
            }
         }
         #pragma acc parallel
         {
            #pragma acc loop
            for( j = 0; j < ny+1; j++ ){
               #pragma acc loop
               for (k=0; k < nz+1; k++){
                  u[k+nz*(j+1*ny)] = u[k+nx*(j+3*ny)];
                  u[k+nz*(j+(nx-1)*ny)] = u[k+nz*(j+(nx-1)*ny)];
               }
            }
         }
         /* Euler Scheme */
         #pragma acc parallel
         {
            #pragma acc loop
            for( i = 1; i < nx+2; i++ ){
               #pragma acc loop
               for( j = 1; j < ny+2; j++ ){
                  #pragma acc loop
                  for( k = 1; k < nz+2; k++){
                     lap = u[k+nz*(j+(i+1)*ny)] + u[k+nz*(j+(i-1)*ny)] + u[k+nz*((j-1)+i*ny)] + u[k+nz*((j+1)+i*ny)]
                           + u[(k+1)+nz*(j+i*ny)] + u[(k-1)+nz*(j+i*ny)] - 6.*u[k+nz*(j+i*ny)];
                     ut[k+nz*(j+i*ny)] = u[k+nz*(j+i*ny)] + lap * dt * gama / h2;
                  }
               }
            }
         }
         /* Update of the mesh */
         #pragma acc parallel
         {
            #pragma acc loop
            for( i = 1; i < nx; i++ ){
               #pragma acc loop
               for( j = 1; j < ny; j++ ){
                  #pragma acc loop
                  for ( k = 1; k < nz; k++){
                     u[k+nz*(j+i*ny)]=ut[k+nz*(j+i*ny)];
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
            for( i = 1; i < nx; i++ ){
               for( j = 1; j < ny; j++ ){
                  for ( k = 1; k < nz; k++){
                     if (u[k+nz*(j+i*ny)] > 0.1f){
                        fprintf(fl, "%d , %d , %d , %f \n", i, j, k, u[k+nz*(j+i*ny)]);
                     }
                  }
               }
            }
         }   //este es del if
      }
   }
   double runtime = GetTimer();
   printf(" total: %f s\n", runtime / 1000.f);
   close(fl);
   printf("Ya terminamos!!!!!! \n");
   return;
}
