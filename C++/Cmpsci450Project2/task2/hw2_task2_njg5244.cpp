//Nathanael Grix
//This is the optomized version of task 2
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <new>
#include <sys/time.h>

#define N 16384

void get_walltime(double* wcTime){
        struct timeval tp;
        gettimeofday(&tp,NULL);
        *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){
	//values of the old operation
	double *AOld = new double[N*N];
	double *BOld = new double[N*N];
	//values of the new operation
	double *A = new double[N*N];
	double *B = new double[N*N];
	//checks to see if the operations get the same output
	bool same = true;
	//for gettign the walltime
	double S, E;

	//initialize A and B
	for(int i = 0; i < N*N; i++){
		AOld[i] = (float)i;
		BOld[i] = 0.0;
		A[i] = (float)i;
		B[i] = 0.0;
	}
	
	get_walltime(&S);
	// begin my operation
	for (int y = 0; y < N; y++){
		for (int x = 0; x < N; x++){
			BOld[(x * N) + y] = AOld[(y * N) + x];
		}
	}	
	// end my operation
	get_walltime(&E);

	// print out my run time values
	std::cout<< "old run time: " <<  E-S << std::endl;
	
	//New version run and get run time
	get_walltime(&S);
	// begin my operation
	
	//because we know that N is a power of two we can break it down into even chunks
	//do it in 8 by N/4 chunks so it can cash things properly
	for (int y = 0; y < N; y+=8){
		for (int x = 0; x < N/4; x++){
			B[(x * N) + y] = A[(y * N) + x];
			B[(x * N) + (y+1)] = A[((y+1) * N) + x];
			B[(x * N) + (y+2)] = A[((y+2) * N) + x];
			B[(x * N) + (y+3)] = A[((y+3) * N) + x];
			B[(x * N) + (y+4)] = A[((y+4) * N) + x];
			B[(x * N) + (y+5)] = A[((y+5) * N) + x];
			B[(x * N) + (y+6)] = A[((y+6) * N) + x];
			B[(x * N) + (y+7)] = A[((y+7) * N) + x];
		}
		for (int x = N/4; x < N/2; x++){
			B[(x * N) + y] = A[(y * N) + x];
			B[(x * N) + (y+1)] = A[((y+1) * N) + x];
			B[(x * N) + (y+2)] = A[((y+2) * N) + x];
			B[(x * N) + (y+3)] = A[((y+3) * N) + x];
			B[(x * N) + (y+4)] = A[((y+4) * N) + x];
			B[(x * N) + (y+5)] = A[((y+5) * N) + x];
			B[(x * N) + (y+6)] = A[((y+6) * N) + x];
			B[(x * N) + (y+7)] = A[((y+7) * N) + x];
		}
		for (int x = N/2; x < (N/4)*3; x++){
			B[(x * N) + y] = A[(y * N) + x];
			B[(x * N) + (y+1)] = A[((y+1) * N) + x];
			B[(x * N) + (y+2)] = A[((y+2) * N) + x];
			B[(x * N) + (y+3)] = A[((y+3) * N) + x];
			B[(x * N) + (y+4)] = A[((y+4) * N) + x];
			B[(x * N) + (y+5)] = A[((y+5) * N) + x];
			B[(x * N) + (y+6)] = A[((y+6) * N) + x];
			B[(x * N) + (y+7)] = A[((y+7) * N) + x];
		}
		for (int x = (N/4)*3; x < N; x++){
			B[(x * N) + y] = A[(y * N) + x];
			B[(x * N) + (y+1)] = A[((y+1) * N) + x];
			B[(x * N) + (y+2)] = A[((y+2) * N) + x];
			B[(x * N) + (y+3)] = A[((y+3) * N) + x];
			B[(x * N) + (y+4)] = A[((y+4) * N) + x];
			B[(x * N) + (y+5)] = A[((y+5) * N) + x];
			B[(x * N) + (y+6)] = A[((y+6) * N) + x];
			B[(x * N) + (y+7)] = A[((y+7) * N) + x];
		}
	}
	// end my operation
	get_walltime(&E);

	// print out my new values
	std::cout<< "new run time: " << E-S << std::endl;
	
	// check if they are the same	
	for(int i = 0; i < N*N; i++){
		if(A[i] != AOld[i] || B[i] != BOld[i]){
			same = false;
		}
	}
	std::cout << "Old is same as New: " << same << std::endl;

	//If we are using a small testing N then we can print out the arrays
	/*
 	//prints out the arrays 
	for(int i = 0; i<N; i++){
		for(int j = 0; j<N; j++){
			std::cout<< " " << std::setw(5) << A[(i*N) + j];
		}
		std::cout<< std::endl;
	}
	std::cout<< std::endl;
	for(int i = 0; i<N; i++){
		for(int j = 0; j<N; j++){
			std::cout<< " " << std::setw(5) << B[(i*N) + j];
		}
		std::cout<< std::endl;
	}
	*/
	//make sure to delete the things on the heap
	delete[] AOld;
	delete[] BOld;
	delete[] A;
	delete[] B;
	return 0;
}
