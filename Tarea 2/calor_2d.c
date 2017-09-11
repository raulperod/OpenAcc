#include <stdio.h>
#include <accelmath.h>
#include <openacc.h>
#include "timer.h"
//#include "dislin.h"

void main(){

   int i,j,ntime,nsteps,nx=256,ny=256;
   float u[nx+1][ny+1],ut[nx][ny];
   float gama,PI;
   float lap,h,h2,dt;
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
   for( i = 1; i < nx; i++ ){
      for( j = 1; j < ny; j++ ){
         if ( (i)*(i)+(j)*(j) > 1500 && (i)*(i)+(j)*(j) < 2500 ){
            u[i][j] = 3.0f;
         }else{
            u[i][j]=0.0f;
         }
      }
   }

   StartTimer();
   #pragma acc data copyin(u,ut)
   {
      /* Main Iteration loop */
      for( ntime = 0; ntime < 100000; ntime++){
         /* Boundary Conditions: No-flux*/
         #pragma acc kernels
         {
            for( i = 1; i < nx; i++ ){
               u[i][1] = u[i][3];
               u[i][ny-1] = u[i][ny-2];
            }
         }
         #pragma acc kernels
         {
            for( j = 1; j < ny; j++ ){
               u[1][j] = u[3][j];
               u[nx-1][j] = u[nx-2][j];
            }
         }
         /* Euler Scheme */
         #pragma acc kernels
         {
            for( i = 1; i < nx; i++ ){
               for( j = 1; j < ny; j++ ){
                  lap = u[i+1][j] + u[i-1][j] + u[i][j-1] + u[i][j+1] - 4.*u[i][j];
                  ut[i][j] = u[i][j] + lap * dt * gama / h2;
               }
            }
         }
         /* Update of the mesh */
         #pragma acc kernels
         {
            for( i = 1; i < nx; i++ ){
               for( j = 1; j < ny; j++ ){
                  u[i][j]=ut[i][j];
               }
            }
         }
         /*
         if(ntime%500 == 0){
            printf("%d \n", ntime);

            #pragma acc update host(u)
            // Dislin Plotting

            metafl("GL");
            disini();
            pagera();

            titlin("Ecuación del calor en 2-D",2);

            axspos(450, 1800);
            axslen(2200, 1200);

            name("Eje X", "x");
            name("Eje Y", "y");

            intax();
            autres(nx,ny);
            axspos(600,1850);
            ax3len(1500,1500,1500);

            graf3(0.0,nx,0.0,100.0,1.0,ny,0.0,100.0,0.0,1.5,0.0,1.1);

            crvmat((float *) u,nx+1,ny+1,1,1);

            height(50);
            title();
            endgrf();
            erase();
         }
         */
      }
   }
   double runtime = GetTimer();
   printf(" total: %f s\n", runtime / 1000.f);
   printf("Ya terminamos!!!!!! \n");
   return;
}
