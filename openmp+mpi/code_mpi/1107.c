#include <mpi.h>
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
int main (int argc, char* argv[])
{
	int rank, size,i,j;
	int namelen;
	char arr[999999];
	char *arr1;
        char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init (&argc, &argv);      /* starts MPI*/
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id*/
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes*/
	MPI_Get_processor_name(processor_name, &namelen);
	//printf( "进程id为%d 共有 %d个进程 on %s\n\n\n", rank, size,processor_name );
	printf(" 进程号 = %d ,共有%d个进程 , 节点 = %s\n",rank,size,processor_name);

	if(rank==0)
	{
		
		FILE *fp;
		fp=fopen("file2","r");
		i=0;
		while(!feof(fp))
		{
			fscanf(fp,"%c",&arr[i++]);
		
		}
                
		j=i;//保存数组中元素个数
                 // if(rank==0)
{
		arr1=(char*)malloc((sizeof(char)*j));
} 
             arr1=arr;
              fclose(fp);	
		//MPI_File_open(MPI_Comm comm, ROMIO_CONST char *filename, int amode,MPI_Info info, MPI_File * fh);
		//MPI_File_open(MPI_COMM_WORLD, ROMIO_CONST char *filename, int amode,MPI_Info info, MPI_File * fh);
	}
	MPI_Bcast(&j,1,MPI_INT,0,MPI_COMM_WORLD);
	if(rank>0)
{arr1=(char*)malloc(sizeof(char)*j);}
	MPI_Bcast(arr1,j,MPI_CHAR,0,MPI_COMM_WORLD);
	printf("%c %c %c",arr1[100],arr1[101],arr1[102]);
//	MPI_Barrier(MPI_COMM_WORLD);
	FILE *fp2;
	fp2=fopen("22222","w");
	i=0;
	
	while(arr1[i]!='\0')
	{
		fprintf(fp2,"%c",arr1[i]);
		i++;
	}
	
	MPI_Finalize();   //MPI end.
	return 0;
}






















