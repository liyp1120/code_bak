#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
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
	
	int  *rbuf;
	int sendarray[100];
	for(i=0;i<100;i++) sendarray[i]=i;
	rbuf=(int*)malloc(size*100*sizeof(int));

	MPI_Gather(sendarray, 100, MPI_INT, rbuf, 100, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank==0)
	{
		for(j=0;j<size*100;j++) printf("rbuf[%d] = %d\n",j,rbuf[j]);
	}
	
	free(rbuf);
	
	MPI_Finalize();   //MPI end.
	return 0;
}






















