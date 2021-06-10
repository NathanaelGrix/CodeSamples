//Nathanael Grix
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <thread>
#include <mutex> 

//default globals
int N = 1000000000;
int P = 10;
//things for getting function run time
double S, E;

void get_walltime(double* wcTime){
        struct timeval tp;
        gettimeofday(&tp,NULL);
        *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}
//sing thread implementation
void maMiMeSt(double* ar, double* max, double*min, 
		double* mean, double* SD){
	double sum=0.0;
	double* arrayt = new double[N];
	double sumsq=0.0;
	double currMax=0.0;
	double currMin=ar[0];
	get_walltime(&S);
	for(int i = 0; i<N; i++){
		sum += ar[i];
		sumsq += ar[i]*ar[i];
		if(currMax < ar[i]){
			currMax = ar[i];
		}
		else if(currMin > ar[i]){
			currMin = ar[i];
		}
	}
	*max = currMax;
	*mean = sum/N;
	*min = currMin;
	*SD = sqrt((sumsq-((sum)*(sum)/double(N)))/double(N-1));
	get_walltime(&E);
}
//mutexes to prevent threads from running critical sections
//at the same time
std::mutex holdUp;
std::mutex waitAMinut;
std::mutex putALittle;
std::mutex loveInIt;

//a function that is called by the thread to calculate the partials
void wat(double* ar, double* sum, double* sumSq, double* currMax, double* currMin, int n){
	//local variables without conflicts
	double localSum=0.0;
	double localSumSq=0.0;
	double localMax=0.0;
	double localMin=ar[0];;
	int i;
	//for loop that calculates the local min max squared sums and sums
	for(i = 0; i<n; i++){
		if(ar[i] > localMax){
			localMax = ar[i];
		}
		else if(ar[i] < localMin){
			localMin = ar[i];
		}
		localSumSq += ar[i]*ar[i];
		localSum += ar[i];
	}
	//locks the max
	holdUp.lock();
	if(*currMax < localMax){
		*currMax = localMax;
	}
	holdUp.unlock();
	//unlocks the max then locks the min
	waitAMinut.lock();
	if(*currMin > localMin){
		*currMin = localMin;
	}
	waitAMinut.unlock();
	//unlocks the min then locks the sum
	putALittle.lock();
	*sum += localSum;
	putALittle.unlock();
	//unlocks the sums then locks the square sums
	loveInIt.lock();
	*sumSq+=localSumSq;
	loveInIt.unlock();
}

void threadMaMiMeSt(double* ar, double* max, double*min,
		double* mean, double* SD){
	//creates temp values
	double sum=0.0;
	double sumSq=0.0;
	double currMax=0.0;
	double currMin=100.0;
	//creates the threads to be run
	std::thread t[P-1];
	//starts the timer
	get_walltime(&S);
	//runs the other P-1 threads with the function
	for(int i = 0; i<P-1; i++){
		t[i] = std::thread(wat,&ar[(N/P)*i],&sum,&sumSq,&currMax,&currMin,N/P);
	}
	//runs the origional thread with the function
	wat(&ar[(N/P)*(P-1)],&sum,&sumSq,&currMax,&currMin,N/P + N%P);
	//rejoins everything
	for(int i = 0; i<P-1; i++){
		t[i].join();
	}
	//calculates the final answers
	*max = currMax;
	*mean = sum/N;
	*min = currMin;
	*SD = sqrt((sumSq-((sum)*(sum)/double(N)))/(N-1));
	//stops the clock
	get_walltime(&E);
}

int main(int argc, char** argv){
	
	// check if you got values to init
	if(argc == 3){
		N = atoi(argv[1]);
		P = atoi(argv[2]);
	}
	std::cout<<N<<"    "<<P<<std::endl;
	//else be default
	//create the array and all of the locations of the answers
	double* arrayi = new double[N];
	double maxV,minV,meanV,sdV;
	double ompTempMean;
	double ompTempMax;
	double ompTempMin;
	double ompTempSum;
	double ompTempSD;
	//seed the random variables
	srand(3);
	//initilize array with junk with 0-100
	std::cout << "starting to make array \n";
	for(int i = 0; i<N; i++){
		arrayi[i] = ((double)rand()/RAND_MAX)*100.0;
		//std::cout << std::endl << arrayi[i] << std::endl;
	}
	std::cout << "finished making array \n";
	maMiMeSt(arrayi,&maxV,&minV,&meanV,&sdV);
	std::cout << "newS: Max, " << maxV << "  Min, " <<
		minV << "  Mean, " << meanV << "  Sd, " <<
		sdV << std::endl;
	std::cout << (E-S) << std::endl;
	//see how well the thread does and print out the results
	threadMaMiMeSt(arrayi,&maxV,&minV,&meanV,&sdV);
	std::cout << "newP: N, "<< N <<"  P, "<<P <<"  Max, " << maxV << "  Min, " <<
		minV << "  Mean, " << meanV << "  Sd, " <<
		sdV << std::endl;
	std::cout << (E-S) << std::endl;

	delete[] arrayi;
}
