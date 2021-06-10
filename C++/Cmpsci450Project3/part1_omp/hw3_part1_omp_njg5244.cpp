//Nathanael Grix
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>
#include <vector>

//global vars for default values
int N = 1000000000;
int P = 10;
double T = 50.0;

//function that gets preformance
double S, E;
void get_walltime(double* wcTime){
        struct timeval tp;
        gettimeofday(&tp,NULL);
        *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}
//serial function that gets max min mean and standard deviation
void maMiMeSt(double* ar, double* max, double*min, 
		double* mean, double* SD){
	double sum=0.0;
	double sumsq=0.0;
	double currMax=0.0;
	double currMin=100.0;
	//start checking the time
	get_walltime(&S);
	for(int i = 0; i<N; i++){
		//sum up everything in array
		sum += ar[i];
		//square then sum
		sumsq += ar[i]*ar[i];
		//update the current max
		if(currMax < ar[i]){
			currMax = ar[i];
		}
		//update the current min
		if(currMin > ar[i]){
			currMin = ar[i];
		}
	}
	//return the final values
	*max = currMax;
	*mean = sum/N;
	*min = currMin;
	*SD = sqrt((sumsq-((sum)*(sum)/double(N)))/double(N-1));
	get_walltime(&E);
}

//open MP version of min max mean standard deviation
void ompMaMiMeSt(double* ar, double* max, double*min,
		double* mean, double* SD){
	//temp holding values
	double sum=0.0;
	double sumsq=0.0;
	double finalMax=0.0;
	double finalMin=100.0;
	//get the starting time of the function
	get_walltime(&S);
	//split into threads
	#pragma omp parallel num_threads(P)
	{	//create some vars local to the threads
		double currMax=0.0;
		double currMin=ar[0];
		//split up the for loop for each thread
		//combine all the sums and the squared sums
		#pragma omp for simd reduction(+:sum,sumsq)
		for(int i = 0; i<N; i++){
			sum += ar[i];
			sumsq += ar[i]*ar[i];
			//save time by only checking 
			//the min if it isn't already the max
			if(currMax < ar[i]){
				currMax = ar[i];
			}
			else if(currMin > ar[i]){
				currMin = ar[i];
			}
		}
		//combine these in a section that is safe
		#pragma omp critical
		{
			if(finalMax < currMax){
				finalMax = currMax;
			}
			if(finalMin > currMin){
				finalMin = currMin;
			}
		}
	}
	//return the final values and get the final time
	*max = finalMax;
	*mean = sum/N;
	*min = finalMin;
	*SD = sqrt((sumsq-((sum)*(sum)/double(N)))/(N-1));
	get_walltime(&E);
}

int main(int argc, char** argv){
	
	//check if you got values passed to you
	if(argc == 3){
		N = atoi(argv[1]);
		P = atoi(argv[2]);
	}
	else if(argc == 4){
		N = atoi(argv[1]);
		P = atoi(argv[2]);
		T = atof(argv[3]);
	}
	std::cout<<N<<"    "<<P<<std::endl;
	//create an array for the doubles
	double* arrayi = new double[N];
	//create vectors to hold the indexes and values
	std::vector<int> ind;
	std::vector<double> val;
	//versions of variables to hold the parallel and
	//non parallel versions of the functions to compare
	double maxV,minV,meanV,sdV;
	double ompTempMean;
	double ompTempMax;
	double ompTempMin;
	double ompTempSum;
	double ompTempSD;
	//seed with a number for testing
	srand(3);
	//initilize array with junk
	for(int i = 0; i<N; i++){
		arrayi[i] = ((double)rand()/RAND_MAX)*100.0;
		//if the array is small enough then print it
		//std::cout << std::endl << arrayi[i] << std::endl;
	}

	//parallel version
	ompMaMiMeSt(arrayi,&maxV,&minV,&meanV,&sdV);
	std::cout << "newP: Max, " << maxV << " Min, " <<
		minV << " Mean, " << meanV << " Sd, " <<
		sdV << std::endl;
	std::cout << (E-S) << std::endl;

	delete[] arrayi;
}
