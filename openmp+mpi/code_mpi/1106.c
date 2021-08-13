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
	
		
		
		
	MPI_Finalize();   //MPI end.
	return 0;
}






















