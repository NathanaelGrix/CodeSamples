//Nathanael Grix
//This is the optomized version of task 1
#include <iostream>
#include <cstdlib>
#include <new>
#include <sys/time.h>

//#define N 20
//#define R 10

// get the total time that the program has been running
void get_walltime(double* wcTime){
	struct timeval tp;
	gettimeofday(&tp,NULL);
	*wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}
//dummy function shouldn't execute
double dummy(double a[], double b[], double c[], double d[]){
	std::cout << "something went wrong\n";
	return a[2];
}

int main (int argc, char** argv){
	int N;
	int R;
	//seed the random C values	
	srand(10);
	
	// check if you got values to init
	if(argc == 3){
		N = atoi(argv[1]);
		R = atoi(argv[2]);
	}
	//else fill with defaults
	else{
		N = 1000;
		R = 1000000;
	}
	//initialize array
	double * A = new double[N];
	double * B = new double[N];
	double * C = new double[N];
	double * D = new double[N];

	double S, E, MFLOPS;
	
	//fill arrays
	for(int i = 0; i < N; i++){
		A[i] = 0.0;
		B[i] = 1.0;
		C[i] = -2.0;
		//C[i] = (((float) rand())/((float) RAND_MAX)*2.0)-1.0;
                //std::cout << C[i];
		D[i] = 3.0;	
	}
	
	//time junk
	get_walltime(&S);
	//basic loop stuff
	for(int j = 0; j<R; j++){
		for(int i = 0; i<N; i++){
			if(C[i] < 0.0){
				A[i] = B[i] - C[i] * D[i];
			}
			else{
				A[i] = B[i] + C[i] * D[i];
			}
		}
		if(A[2] < 0.0) std::cout << dummy(A,B,C,D);
	}

	//time junk
	get_walltime(&E);
	long totaliter = R*N;
	MFLOPS = ((double)totaliter)*2.0/((E-S)*1000000);
	std::cout << "MFLOPS: " << MFLOPS << "\n";
	
	//display data that shows it is full
	//std::cout << A[0] << " " << A[19] << "\n";
	//std::cout << D[0] << " " << D[19] << "\n";
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] D;
}
