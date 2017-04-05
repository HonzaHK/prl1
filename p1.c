//VUTBR - FIT - PRL - project1
//Jan Kubis / xkubis13

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFSIZE 128
#define TAG 0

int main(int argc, char *argv[]){

	int numprocs = 0; //pocet procesoru
    int myid = 0; //muj rank

    FILE *inFilePtr = fopen("numbers", "rb"); 
    fseek(inFilePtr, 0, SEEK_END);
    int filelen = ftell(inFilePtr);
    rewind(inFilePtr);


	unsigned char *nums; 
	int numsCnt = filelen;

	if ((nums = malloc(sizeof(unsigned char) * numsCnt)) == NULL) {
		perror("Error allocating memory");
		abort();
	}
	memset(nums, 0, sizeof(int)*numsCnt);
	
	fread(nums, filelen, 1, inFilePtr); 
	for(int i = 0; i < numsCnt; ++i) {
		printf("%d ",nums[i]);
	}






	//MPI INIT
	MPI_Init(&argc,&argv); // inicializace MPI 
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // zjistíme, kolik procesů běží 
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); // zjistíme id svého procesu 

	if(myid==0){
		printf("MASTA");
	}



	MPI_Finalize(); 
	printf("\n");
	return 0;
}
