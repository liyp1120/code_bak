#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char* argv[])
{
	int rank, size,i,j;
	int namelen;
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
		FILE *fp = NULL;
		int array[100];
		fp = fopen("file1", "r");
		for(i=0;i<10;i++)   fscanf(fp,"%d",&array[i]);
		for(i=0;i<10;i++)   printf("%d\n",array[i]);
		fclose(fp);
		fp=fopen("file1_binary","wb");
		fwrite(&array[0],sizeof(int),10,fp);
		MPI_Send(array,10,MPI_INT,1,1,MPI_COMM_WORLD);
		MPI_Send(array,10,MPI_INT,2,1,MPI_COMM_WORLD);
		MPI_Send(array,10,MPI_INT,3,1,MPI_COMM_WORLD);
		fclose(fp);
	}
	else
	{
		printf("---------------------------------------------------------\n");
		FILE *fp = NULL;
		fp = fopen("file1_binary","w");
		//int brray[100];
		//MPI_Recv(buf,count,datatype,source,tag,comm,status)
		MPI_Recv(fp,10,MPI_INT,0,1,MPI_COMM_WORLD,&status);
		//for(i=0;i<10;i++)   printf("--------%d\n",brray[i]);
		//fwrite(&brray[0],sizeof(int),10,fp);
		fclose(fp);
	}

	MPI_Finalize();   //MPI end.
	return 0;
}






















