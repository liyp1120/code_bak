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
	MPI_Barrier(MPI_COMM_WORLD);//同步	
	int sendarray[100];
	int *rbuf, *displs, *recvcount;
	//int stride=5;
	int sum=0;
	for(i=0;i<=size-1;i++)  sum+=i;
	rbuf=(int*)malloc(size*sum*sizeof(int));
	displs=(int*)malloc(size*sizeof(int));
	recvcount=(int*)malloc(size*sizeof(int));
		
	for(i=0;i<100;i++) sendarray[i]=i+100;
	for(i=0;i<size;i++) recvcount[i] = i*size;
	displs[0] = 0;
	for(i=1;i<size;i++) displs[i]=displs[i-1]+(i-1)*size;

	MPI_Barrier(MPI_COMM_WORLD);//同步	
	//MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcount,displs, recvtype, root, comm);
	MPI_Gatherv(sendarray, rank*size, MPI_INT, rbuf,recvcount ,displs, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank==0)
	{															
		for(i=0;i<size*sum;i++) printf("rbuf[%d] = %d\n",i,rbuf[i]);
	}
	MPI_Finalize();   //MPI end.
	return 0;
}


