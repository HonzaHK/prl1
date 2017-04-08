//VUTBR - FIT - PRL - project1
//Jan Kubis / xkubis13

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define reg_x 0
#define reg_y 0
#define reg_z 0

typedef struct {
	uint8_t val;
	uint8_t idx;
} param_t;

int main(int argc, char *argv[]){

	int numprocs = 0; //pocet procesoru
	int myid = 0; //muj rank
	param_t x,y,z;
	int c=0;
	MPI_Status stat; //struct- obsahuje kod- source, tag, error

	//MPI INIT
	MPI_Init(&argc,&argv); // inicializace MPI 
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs); // zjistíme, kolik procesů běží 
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); // zjistíme id svého procesu 
	

	int masterid = numprocs-1;
	
	if(myid==masterid){
	    FILE *inFilePtr = NULL;


	    if((inFilePtr = fopen("numbers", "rb"))==NULL){
	    	printf("Error opening input file\n");
	    	return 0;
	    } 
	    fseek(inFilePtr, 0, SEEK_END);
	    int filelen = ftell(inFilePtr);
	    rewind(inFilePtr);

		uint8_t *nums; 
		int numsCnt = filelen;
		if ((nums = malloc(sizeof(uint8_t) * numsCnt)) == NULL) {
			printf("Error allocating memory");
		}
		memset(nums, 0, sizeof(uint8_t)*numsCnt);
		
		fread(nums, filelen, 1, inFilePtr); 
		for(int i=0; i<numsCnt; i++) {
			printf("%d ",nums[i]);
		}
		printf("\n");

		for(int i=0; i<numprocs-1; i++){
			param_t param;
			param.val = nums[i];
			param.idx = i;
			//printf("%d: %d\n", i, nums[i]);
			MPI_Send(&param,2,MPI_BYTE,i,reg_x,MPI_COMM_WORLD);
			MPI_Send(&param,2,MPI_BYTE,0,reg_y,MPI_COMM_WORLD);
		}


		fclose(inFilePtr);
	}

	if(myid!=masterid){
		
		MPI_Recv(&x, 2, MPI_BYTE, masterid, reg_x, MPI_COMM_WORLD, &stat);
		
		for(int i=0;i<numprocs-1; i++){
			int srcid = myid==0 ? masterid : myid-1;
			MPI_Recv(&y, 2, MPI_BYTE, srcid, reg_y, MPI_COMM_WORLD, &stat);

			c+= x.val<y.val || (x.val==y.val && x.idx<y.idx) ;

			if(myid!=masterid-1){
				MPI_Send(&y, 2, MPI_BYTE, myid+1, reg_y, MPI_COMM_WORLD);
			}
		}

		MPI_Send(&x, 2, MPI_BYTE, c, reg_z, MPI_COMM_WORLD);
		MPI_Recv(&z, 2, MPI_BYTE, MPI_ANY_SOURCE, reg_z, MPI_COMM_WORLD, &stat);	

	}

	MPI_Barrier(MPI_COMM_WORLD);
	for(int i=0;i<numprocs-1;i++){
		if(myid!=masterid){
			MPI_Send(&z, 2, MPI_BYTE, myid+1, reg_z, MPI_COMM_WORLD);
		}

		if(myid!=0){
			MPI_Recv(&z, 2, MPI_BYTE, myid-1, reg_z, MPI_COMM_WORLD,&stat);
		}

		if(myid==masterid){
			printf("%d\n", z.val);
		}
	}

	MPI_Finalize();

	return 0;
}
