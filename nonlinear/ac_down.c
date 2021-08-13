#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void fillMatrix(int n, double * matrix);
double * createMatrix(int n);

int main(int argc, char * argv[]) {
	unsigned int mSize = 0, i, k;
	double * a, * b, * c;
	double wtime;

	//mSize = 270;
	mSize = 600;

	a = (double*)createMatrix(mSize);
	b = (double*)createMatrix(mSize);
	c = (double*)createMatrix(mSize);

	fillMatrix(mSize, a);
	fillMatrix(mSize, b);

	printf ( "Adjoint convolution benchmark with matrix size %d\n", mSize );

	wtime = omp_get_wtime ( );
	//#pragma omp parallel for shared(a,b,c) private(i,k) schedule(dynamic,1000) num_threads(15)
	#pragma omp parallel for  private(i,k) schedule(guided,100) num_threads(9)
	for (i = 0; i < mSize*mSize; i++) {
		c[i]= 0;
		for (k = i; k < mSize*mSize; k++) {
			//c[i] += 5.5*b[k]*a[k-i];
			c[i] += b[k]*a[k-i];
		}
	}

	wtime = omp_get_wtime ( ) - wtime;
	printf ( "\n" );
	printf ( "  Time = %g seconds.\n", wtime );

	free(a);
	free(b);
	free(c);
}

double * createMatrix(int n) {

	double * m = (double*) malloc(n*n*sizeof(double));

	return m;
}

void fillMatrix(int n, double * matrix) {
	int i;
	for (i = 0; i < n*n; i++) {
		matrix[i] = (rand()%10) - 5; //between -5 and 4
	}
}

