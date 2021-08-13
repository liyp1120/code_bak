#include <mpi.h>
#include <stdio.h>
#include <math.h>
 
int main(int argc,char *argv[])
{
    int rank ,size;
    int namelen;
    int version, subversion;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
 
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
 
    MPI_Get_processor_name(processor_name,&namelen);
    MPI_Get_version(&version, &subversion);
 
 
    printf("Process %d of %d on %s\n",rank, size, processor_name);
    printf("Process %d of %d on %s\n",rank, size, processor_name);
    printf("version %d.%d \n",version, subversion);
 
    MPI_Finalize();
}

