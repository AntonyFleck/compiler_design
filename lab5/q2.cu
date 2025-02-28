#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__device__ int getGTID() {
    int blockid = blockIdx.x + blockIdx.y * gridDim.x + blockIdx.z * gridDim.x * gridDim.y;
    int threadid = blockid * (blockDim.x * blockDim.y * blockDim.z) + 
                   (threadIdx.z * blockDim.x * blockDim.y) + 
                   (threadIdx.y * blockDim.x) + 
                   threadIdx.x;
    return threadid;
}


__global__ void add(int *a,int *b, int *c,int n){
    int gtid=getGTID();
    if(gtid< n)
        c[gtid]=a[gtid]+b[gtid];

}


int main(){

    int *a,*b,*c;
	int *d_A, *d_B, *d_C;
   	printf("Enter no. of elements: ");
	int  N;
	scanf("%d",&N);
    	int S = N * sizeof(int);
	a = (int*)malloc(S);
	b = (int*)malloc(S);
	c = (int*)malloc(S);
	cudaMalloc((void**)&d_A , S);
	cudaMalloc((void**)&d_B , S);
	cudaMalloc((void**)&d_C , S);

printf("220905440\n");
    printf("Enter elements in A: ");
	for(int i=0; i<N; i++)
	{
		a[i]=rand()%10;
        printf("%d \t",a[i]);
	}
    printf("\n");

    printf("Enter elements in B: ");
	for(int i=0; i<N; i++)
	{
		b[i]=rand()%10;
        printf("%d \t",b[i]);
	}

	cudaMemcpy(d_A,a,S,cudaMemcpyHostToDevice);
	cudaMemcpy(d_B,b,S,cudaMemcpyHostToDevice);
       int blocks = (int)ceil((float)N / 256); 
	add<<<blocks,256>>>(d_A,d_B,d_C,N);

	cudaMemcpy(c,d_C,S,cudaMemcpyDeviceToHost);
   printf("\n");
   printf("No. of blocks is %d\n",blocks);
      printf("No. of unused threads %d\n",blocks*256-N);
	printf("Result: ");
	for(int i=0; i<N; i++)
	{
		printf("%d ",c[i]);
	}
	printf("\n");

	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	free(a);
	free(b);
	free(c);
	return 0;
    



}