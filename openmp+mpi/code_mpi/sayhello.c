#include <mpi.h>
#include <stdio.h>
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
	//printf(" 进程号 = %d ,共有%d个进程 , 节点 = %s\n",rank,size,processor_name);
	
	if(size<2)
	{
		fprintf(stderr,"systest requires at least 2 processes\n");
		MPI_Abort(MPI_COMM_WORLD,1); //终止与通信器通信相关的所有MPI进程
	}
	fprintf(stderr,"Processes %d is alive on %s\n",rank,processor_name);
	MPI_Barrier(MPI_COMM_WORLD); //同步
	hello();
	MPI_Finalize();   //MPI end.
	return 0;
}

void hello() //任意两个进程间交换问候信息
{
	int type=1,rank,size;
	int buffer[2],node;
	MPI_Status status;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	if(rank==0)  //进程0负责打印提示信息
	{
		printf("\nHello test from all to all\n");
		fflush(stdout);//清空输出缓冲区，并把缓冲区内容输出。
	}	
	for(node=0;node<size;node++)
	{
		if(node!=rank)
		{
			buffer[0]=rank;//将自身标识放入消息中
			buffer[1]=node;//将被问候的进程标识也放入消息中
			MPI_Send(buffer,2,MPI_INT,node,1120,MPI_COMM_WORLD); //首先将问候消息发出
			MPI_Recv(buffer,2,MPI_INT,node,1120,MPI_COMM_WORLD,&status);//然后接收被问候进程对自己发送的问候信息
			if((buffer[0]!=node)||(buffer[1]!=rank))//若接收到的消息的内容不是问候自己的或不是以被问候方的身份问候自己,则出错
			{
				fprintf(stderr,"Hello: %d!=%d or %d!=%d\n",buffer[0],node,buffer[1],rank);
				printf("Mismatch on hello process ids; node=%d\n",node);
			}
			printf("Hello from %d to %d\n",rank,node);
			fflush(stdout);
		}
	}
}




















