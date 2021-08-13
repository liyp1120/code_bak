#include <mpi.h>
#include <stdio.h>
#include <omp.h>
int main (int argc, char* argv[])
{
	int rank, size,i,j,value;
	int namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Init (&argc, &argv);      /* starts MPI*/
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id*/
	MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes*/
	MPI_Get_processor_name(processor_name, &namelen);
	//printf( "进程id为%d 共有 %d个进程 on %s\n\n\n", rank, size,processor_name );
	printf(" 进程号 = %d ,共有%d个进程 , 节点 = %s\n",rank,size,processor_name);
	
	do
	{
		if(rank==0)
		{
			fprintf(stderr,"\nPlease give new int value=");
			scanf("%d",&value);
			fprintf(stderr,"%d  read  <-<-  (%d)\n",rank,value);
			if(size>1)
			{
				MPI_Send(&value,1,MPI_INT,rank+1,1120,MPI_COMM_WORLD);
				fprintf(stderr,"%d  send  (%d) ->->  %d\n",rank,value,rank+1);
			}
		}
		else
		{
			MPI_Recv(&value,1,MPI_INT,rank-1,1120,MPI_COMM_WORLD,&status);
			fprintf(stderr,"%d  receive  (%d) <-<-  %d\n",rank,value,rank-1);
			if(rank<size-1) //若当前进程不是最后一个进程,则将数据继续向后传递
			{
				MPI_Send(&value,1,MPI_INT,rank+1,1120,MPI_COMM_WORLD);
				fprintf(stderr,"%d  send  (%d) ->->  %d\n",rank,value,rank+1);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);//执行一下同步,主要是为了将前后两次数据传递分开

	}while(value>=0);


	MPI_Finalize();   //MPI end.
	return 0;
}






















