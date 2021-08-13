#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<omp.h>
#define row 2500
#define col 2500
int main() {
	
	int i,j,k;
	double wtime;
	int **a=NULL,**b=NULL,**c=NULL;

	printf("1.Allocate memory------\n");
	/*先申请每一行指针的内存*/
	a = (int **)malloc(sizeof(int *)*row);
	b = (int **)malloc(sizeof(int *)*row);
	c = (int **)malloc(sizeof(int *)*row);
	/*申请各行内存*/
	for(i=0; i<row; i++) {
		*(a+i)=(int *)malloc(sizeof(int)*col);
		*(b+i)=(int *)malloc(sizeof(int)*col);
		*(c+i)=(int *)malloc(sizeof(int)*col);
	}


	printf("2.Initialize array------\n");
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			a[i][j]=rand()%600 + 1;
			b[i][j]=rand()%2000 + 1;
			c[i][j]=0;
		}
	}
	printf("3.Start matrix multiplication------\n");
	wtime = omp_get_wtime ();
	#pragma omp parallel for schedule(guided,100) private(i,j,k) num_threads(3)
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			for(k=0; k<row; k++) {
				c[i][j]+=a[i][k]*b[k][j];
			}
		}
	}
	wtime = omp_get_wtime () - wtime;
	printf("4.Output result------\n");
	for(i=0; i<row; i++) {
		for(j=0; j<col; j++) {
			if(i%200==0 && j%200==0)printf("%d\n",c[i][j]);
		}
	}
	printf ( "\n" );
	printf ( "  Time = %g seconds.\n", wtime );
	/*
	 *释放内存
	 */
	for(i=0; i<col; i++) {
		free(*(a+i));
		free(*(b+i));
		free(*(c+i));
	}
	free(a);
	free(b);
	free(c);
	return 0;
}
