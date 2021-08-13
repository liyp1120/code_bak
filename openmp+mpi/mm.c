#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#define N  1000
int main()
{
	//int a[N][N],b[N][N],c[N][N];
	float **a,**b,**c;
	if((a = (float **)malloc(sizeof(float *)*N)) != NULL);
	if((b = (float **)malloc(sizeof(float *)*N)) != NULL);
	if((c = (float **)malloc(sizeof(float *)*N)) != NULL);
	for(int i=0;i<N;i++)
	{
		a[i] = (float *)malloc(sizeof(float)*N);
		b[i] = (float *)malloc(sizeof(float)*N);
		c[i] = (float *)malloc(sizeof(float)*N);
	}
	for(int i=0; i<N; i++) 
	{
		for(int j=0; j<N; j++) 
		{
			a[i][j]=rand()%600 + 1;
			b[i][j]=rand()%2000 + 1;
			c[i][j]=0;	
		}
	}
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				//printf("k = %d\n",k);
				c[i][j]+= a[i][k] * b[k][j];
			}

		}
	}
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			if(i%20==0 && j%20==0)printf("%f\n",c[i][j]);
		}
	}
	for(int i=0;i<N;i++)
	{
		free(a[i]);
		free(b[i]);
		free(c[i]);
	}
	free(a);
	free(b);
	free(c);


}
