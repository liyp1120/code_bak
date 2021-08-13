//*computethepotentialenergyofacollectionof*/
//*particlesinteractingviapairwisepotential*/
// compile -lm   //liyp 10.5
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
//#include<windows.h>
#include<time.h>
#include<omp.h> //liyp 10.05
//#define NPARTS 1000
#define NPARTS 10000
#define NITER  201
#define DIMS   3

int  rand(void);
double  computePot();
void initPositions(void);
void updatePositions(void);

double r[DIMS][NPARTS];
double pot;
double distx,disty,distz,dist;
double **poot=NULL;


int main(){
	int i;
	double time = 0.0;
	poot = (double **)malloc(sizeof(double *)*NPARTS);
	for(i=0; i<NPARTS; i++) *(poot+i)=(double *)malloc(sizeof(double)*NPARTS);
	
	initPositions();
	updatePositions();

	for(i=0;i<NITER;i++)
	{
		pot = 0;
		time += computePot();
		if(i%10 == 0) printf("%5d:Potential:%10.7f \n",i,pot);
		updatePositions();
	}
	printf ( "  Time = %g seconds.\n", time);
	for(i=0; i<NPARTS; i++) free(*(poot+i));
	free(poot);
}


void initPositions(){
	int i,j;

	for(i=0;i<DIMS;i++)
		for(j=0;j<NPARTS;j++)
			r[i][j] = 0.5+((double)rand()/(double)RAND_MAX);
}


void updatePositions(){
	int i,j;

	for(i=0;i<DIMS;i++)
		for(j=0;j<NPARTS;j++)
			r[i][j] -= 0.5+((double)rand()/(double)RAND_MAX);
}


double computePot(){
	int i,j;
	double wtime;
	wtime = omp_get_wtime();
	#pragma omp parallel for schedule(static) private(distx,disty,distz,dist,i,j)  num_threads(7)
	for(i=0;i<NPARTS;i++){
		for(j=0;j<i-1;j++){
			distx = pow((r[0][j]-r[0][i]),2);
			disty = pow((r[1][j]-r[1][i]),2);
			distz = pow((r[2][j]-r[2][i]),2);
			dist  = sqrt(distx+disty+distz);
			//pot   = pot + 1.0/dist;
			poot[i][j] = 1.0/dist;
		}
	}
	wtime = omp_get_wtime() - wtime;
	for(i=0;i<NPARTS;i++){
		for(j=0;j<i-1;j++){
			pot = pot + poot[i][j];
		}
	}
	return wtime;
}
