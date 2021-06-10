//Nathanael Grix
//Hw5 Cuda Core Benchmark:
//Purpose: Compare cpu and gpu execution time to sum large arrays of numbers on
//	Penn State's computing cluster. I will compare the exectution time from 
//	sets of 1 to 200,000,000.
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define B 1024

//unused implementation
__global__ void scan(float *g_odata, float *g_idata, unsigned long long int n);
//first part of sacning a large array
__global__ void prescan(float *g_odata, float *g_idata, unsigned long long int n, float* blkSum, unsigned long int N);
//last part of scaning a large array
__global__ void postscan(float *g_odata, unsigned long long int n, float* blkSum);

//cpu impementation of a simple scan
void scanCPU(float *f_out, float *f_in, unsigned long long int i_n);

//function for getting the time difference
double myDiffTime(struct timeval &start, struct timeval &end)
{
	double d_start, d_end;
	d_start = (double)(start.tv_sec + start.tv_usec/1000000.0);
	d_end = (double)(end.tv_sec + end.tv_usec/1000000.0);
	return (d_end - d_start);
}

int main(int argc,char** argv) {

	//number of values and threads
	unsigned long int N;

	//default val or not?
	if(argc==2){
		N=atol(argv[1]);
	}
	else{
		N=100000000;
	}

	printf("N=%d, B=%d\n\n",N,B);
	//calculate the number of blocks
	unsigned long long int blkNum =(unsigned long long int)ceil((double)N/((double)(B*2)));
	
	//the values used to calculate the scans
	float *a, *c, *g, *blkSumin, *blkOut;
	//values used to time 
	timeval start, end;
	
	//variables used to talk to memory from cuda
	float *dev_a, *dev_g, *dev_blkSum;
	//sizes of an array of N and the sum of the blocks
	unsigned long long int size = N * sizeof(float);
	unsigned long long int blkSumSize = blkNum * sizeof(float);
	
	//time vars
	double d_gpuTime, d_cpuTime, d_gpuLoadT, d_gpuPreT, d_gpuScaT, d_gpuPostT;
	double load[4];

	//the serial final result
	c=(float*)malloc(size);

	//cuda optomized malloc for vars that cuda will need
	cudaMallocHost((void **) &a, size);
	cudaMallocHost((void **) &g, size);
	cudaMallocHost((void **) &blkSumin, blkSumSize);
	cudaMallocHost((void **) &blkOut, blkSumSize);
	
	// initialize matrices a 
	for (unsigned long long int i = 0; i < N; i++)
	{	
		// fill a with rand floats
		a[i] = (float)(rand() % 1000000) / 1000.0;
	}
	// initialize a and b matrices here
	cudaMalloc((void **) &dev_a, size);
	cudaMalloc((void **) &dev_g, size);
	cudaMalloc((void **) &dev_blkSum, blkSumSize);
	
	//		START TIME
	gettimeofday(&start, NULL);
	
	//		COPY 1 TIME
	// fill the graphics card's memory with a
	cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
	gettimeofday(&end, NULL);
	load[0] = myDiffTime(start, end);

	//		PRE TIME
	// have small scans done on the memory in blocks
	gettimeofday(&start, NULL);
	prescan<<<blkNum,B,2*B*sizeof(float)>>>(dev_g, dev_a, B*2, dev_blkSum, N);
	cudaDeviceSynchronize();
	gettimeofday(&end, NULL);
	d_gpuPreT = myDiffTime(start, end);

	//		COPY 2 TIME
	// get the blocks' sums out of the graphics cards so we can do a small serial
	// 	scan on them.
	gettimeofday(&start, NULL);
	cudaMemcpy(blkSumin, dev_blkSum, blkSumSize, cudaMemcpyDeviceToHost);
	gettimeofday(&end, NULL);
	load[1] += myDiffTime(start, end);
	
	//		SCAN TIME
	// do a small serial scan on the blocks' sums.
	gettimeofday(&start, NULL);
	scanCPU(blkOut, blkSumin, blkNum);
	gettimeofday(&end, NULL);
	d_gpuScaT = myDiffTime(start, end);

	//		COPY 3 TIME
	// give the blocks back to the graphics cards for post.
	gettimeofday(&start, NULL);
	cudaMemcpy(dev_blkSum, blkOut, blkSumSize, cudaMemcpyHostToDevice);
	cudaDeviceSynchronize();
	gettimeofday(&end, NULL);
	load[2] += myDiffTime(start, end);

	//		POST TIME
	// add the new scanned sum to each block to get the scan of the whole array
	gettimeofday(&start, NULL);
	postscan<<<(unsigned long long int)ceil((double)N/(double)(B)),B>>>(dev_g, N, dev_blkSum);
	gettimeofday(&end, NULL);
	d_gpuPostT = myDiffTime(start, end);

	//		COPY 4 TIME
	// finally get the result into normal memory
	gettimeofday(&start, NULL);
	cudaMemcpy(g, dev_g, size, cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();
	
	//		END TIME
	gettimeofday(&end, NULL);
	load[3] += myDiffTime(start, end);

	// get the total time for the loads
	d_gpuLoadT = load[0] + load[1] + load[2] +load[3];
	// get the total time for my implementation
	d_gpuTime = d_gpuLoadT + d_gpuPreT + d_gpuScaT + d_gpuPostT;

	//do a serial scan on the whole array
	gettimeofday(&start, NULL);
	scanCPU(c, a, N);
	
	gettimeofday(&end, NULL);
	d_cpuTime = myDiffTime(start, end);
	
	// free the memory for the graphics card
	cudaFree(dev_a); cudaFree(dev_g); cudaFree(dev_blkSum);

	// display at least 50 sums and compare the error percent of the serial
	// 	vs cuda implentation.
	for (unsigned long long int i = 0; i < N; i++)
	{	if(N>50){
			if(i%(N/50)==0 || i == N-1){
				printf("c[%i] = %0.3f, g[%i] = %0.3f\n", i, c[i], i, g[i]);
				printf("error percent = %f%\n",fabs((g[i]-c[i])/c[i])*100.0);
			}
		}
		else{
			printf("c[%i] = %0.3f, g[%i] = %0.3f\n", i, c[i], i, g[i]);
			printf("error percent = %f%\n",fabs((g[i]-c[i])/c[i])*100.0);
		}
	}
	
	// print out the time for the cpu and the gpu
	printf("CPU Time for scan size %i: %f\n", N, d_cpuTime);
	printf("GPU Time for scan size %i: %f\n", N, d_gpuTime);
	// print out the times of the components of the gpu implementation
	printf("\tgpu Load=%f, Pre=%f, Sca=%f, Post=%f\n",d_gpuLoadT, d_gpuPreT, d_gpuScaT, d_gpuPostT);
	// print out the various loading times for the implementation
	printf("\t\tgpu load times={%f,%f,%f,%f}\n", load[0],load[1],load[2],load[3]);

	// free c from memory
	free(c);
	
	
	// free the rest of the variables
	cudaFreeHost(a);
	cudaFreeHost(g);
	cudaFreeHost(blkSumin);
	cudaFreeHost(blkOut);
}

// old unused verson of scan that was included in the naivescan
__global__ void scan(float *g_odata, float *g_idata, unsigned long long int n)
{
	extern __shared__ float temp[]; // allocated on invocation
	unsigned long long int thid = threadIdx.x;
	unsigned long long int pout = 0, pin = 1;
	// Load input into shared memory.
	// This is exclusive scan, so shift right by one
	// and set first element to 0
	temp[pout*n + thid] = (thid > 0) ? g_idata[thid-1] : 0;
	__syncthreads();
	for (unsigned long long int offset = 1; offset < n; offset *= 2)
	{
		pout = 1 - pout; // swap double buffer indices
		pin = 1 - pout;
		if (thid >= offset)
			temp[pout*n+thid] += temp[pin*n+thid - offset];
		else
			temp[pout*n+thid] = temp[pin*n+thid];

		__syncthreads();
	}
	g_odata[thid] = temp[pout*n+thid]; // write output
}

// function that adds the new blocks' sums back into the scanned blocks
__global__ void postscan(float *g_odata, unsigned long long int n, float* blkSum){
	unsigned long long int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < n) 
	{	// prescan does two vals per block but this one does 1 so 
		// 	adjust the block ID to reflect that.
		g_odata[i] += blkSum[blockIdx.x/2];
	}
}

// scans a block of the data
__global__ void prescan(float *g_odata, float *g_idata, unsigned long long int n, float* blkSum, unsigned long int N) 
{ 
	// name the newly allocated memory as temp.
	extern  __shared__  float temp[];
	
	//get the begining of the array for this block
	unsigned long long int bldata = blockIdx.x*(blockDim.x);

	// get the thread's id for the block
	unsigned long long int thid = threadIdx.x; 
	
	// initialize the offset to 1
	unsigned long long int offset = 1;

	// fill the temp array with the input data because the math can be executed
	// 	much faster than other forms of meory
	temp[2*thid]   = 0.0; 
    	temp[2*thid+1] = 0.0;
	if(2*(thid+bldata)<N){
		temp[2*thid]   = g_idata[2*(thid+bldata)]; 
	}
	if(2*(thid+bldata)+1<N){
    		temp[2*thid+1] = g_idata[2*(thid+bldata)+1];
	}
	
	//log for loop
	for (unsigned long long int d = n>>1; d > 0; d >>= 1) 
	// build sum in place up the tree 
    	{ 
        	__syncthreads(); 
		if (thid < d){    
        		unsigned long long int ai = offset*(2*thid+1)-1; 
			unsigned long long int bi = ai+offset; 
		    	temp[bi] += temp[ai];
  		} 
		// exponential offset
        	offset <<= 1; 
    	}
 
	// set the lst value and get the total sum
	if (thid == 0){
		blkSum[blockIdx.x] = temp[n-1];
		temp[n-1] = 0;
	}
 
	for (unsigned long long int d = 1; d < n; d <<= 1) 
	// traverse down tree & build scan 
    	{ 
        	offset >>= 1; 
        	__syncthreads(); 
		if (thid < d){
			unsigned long long int ai = offset*(2*thid+1)-1; 
			unsigned long long int bi = ai+offset; 
			float t   = temp[ai]; 
            		temp[ai]  = temp[bi]; 
            		temp[bi] += t; 
        	} 
    	}
	__syncthreads();

	// write results to device memory 
	if(2*(thid+bldata)<N){
		g_odata[2*(thid+bldata)]   = temp[2*thid];
	}
	if(2*(thid+bldata)+1<N){
		g_odata[2*(thid+bldata)+1] = temp[2*thid+1];
	}
}
 
void scanCPU(float *f_out, float *f_in, unsigned long long int i_n)
{
	f_out[0] = 0;
	for (unsigned long long int i = 1; i < i_n; i++)
		f_out[i] = f_out[i-1] + f_in[i-1];

}
