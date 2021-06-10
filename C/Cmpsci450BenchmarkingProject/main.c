////////////////////////////////////////////////////////////////////////////
// 		Nathanel Grix & Anikate Ganju
// CPU benchmarking program
// -------------------------------------------------------------------------
// Purpose: 	This program goes through various tests to see how it
//			cnanges the preformance of the processor.
// Method:		This program goes throgh addition, subtraction, and
// 			multiplication on three integers. We then repeat for floating
//			point numbers to get then get the integer and floating point
//			preformance of the processor when not memory bound.
//				We then repeat this process when working with a large array
//			to test the preformance of the various caches.
//				Finally we repeat the process with a multithreaded version
//			to see how the processor utalizes multipule cores and threads.
// Notes:		While the score focouses on the preformance of the processor
//			other variables may impact preformance, including the system's 
//			memory, hard drive, and opperating system.
///////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

//function to get the time
static double get_walltime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);
}

int intTest(int numberIter,int seed){
	//printf("insideTest and seed=%d\n",seed);
	//memory locations used for the int test
	unsigned int first,second,third;
	unsigned int temp1,temp2,temp3;
	//two masks to keep the variables at a certain size
	unsigned int bitMask16and = 0xFF;
	unsigned int bitMask16or = 0x00;
	//a counter for the loops
	int i;
	//seeds the random function
	srand(seed);

	//get a random 16 bit integer between our sizes
	first = rand()&bitMask16and|bitMask16or;
	second = rand()&bitMask16and|bitMask16or;
	third = rand()&bitMask16and|bitMask16or;
	//printf("\nInt:\n");
	//show the starting variables to varify that it is working
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	
	//do arthmitic on the variables to test the alu's clock speed
	for(i=0;i<numberIter;i++){
		//do adition, subtraction, and multiplication of the vars
		//	and use the masks to keep the size ok.
		temp1 = (third+second)&bitMask16and|bitMask16or;
		temp2 = (third-first)&bitMask16and|bitMask16or;
		temp3 = (first*second)&bitMask16and|bitMask16or;
		//feed the calculated numbers back into the variables so they
		//	are always new and different
		first=temp1;
		second=temp2;
		third=temp3;
	}
	//varify that math was done
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	//return a new seed
	return (rand()+first);
}
int doubleTest(int numberIter, int seed){
	int i;
	//create new double test variables
	unsigned dTemp1,dTemp2,dTemp3;
	//create a version that is cast to a char array so we can do bit
	//	opperations on them.
	unsigned char *dT1 = (unsigned char *)(&dTemp1);
	unsigned char *dT2 = (unsigned char *)(&dTemp2);
	unsigned char *dT3 = (unsigned char *)(&dTemp3);
	//seed the random vars
	srand(seed);
	//create the starting variables
	double dFirst,dSecond,dThird;

	//fill them with random numbers
	dFirst = (double)(rand())/100.0;
	dSecond = (double)(rand())/100.0;
	dThird = (double)(rand())/100.0;
	//printf("\ndouble:\n");
	//display the starting vars to varify that it was filled
	//printf("first=%f,\t\tsecond=%f,\t\tthird=%f\n",dFirst,dSecond,dThird);
	
	//do the float arithmitic
	for(i=0;i<numberIter;i++){
		//calculate an addition
		dTemp1 = (dSecond+dThird);
		//cut off the top exp and sign bits to keep it from hitting inf or reseting a nan
		dT1[0]=0;
		//calculate using subtration 
		dTemp2 = (dFirst-dThird);
		//cut off the top exp and sign bits to keep it from hitting inf or reseting a nan
		dT2[0]=0;
		//calculate using multiplication
		dTemp3 = (dFirst*dSecond);
		//cut off the top exp and sign bits to keep it from hitting inf or reseting a nan
		dT3[0]=0;
		//get the new starting values
		dFirst=dTemp1;
		dSecond=dTemp2;
		dThird=dTemp3;
	}
	//varify that something was calculated
	//printf("first=%f,\t\tsecond=%f,\t\tthird=%f\n",dFirst,dSecond,dThird);
	//return the new seed
	return rand()+(int)dFirst;
}
int intArrayTest(int numberIter,int seed, int* firstar, int* secondar, int* thirdar){
	//printf("insideTest and seed=%d\n",seed);
	//memory locations used for the int test
	unsigned int temp1,temp2,temp3;
	//two masks to keep the variables at a certain size
	unsigned int bitMask16and = 0xFF;
	//a counter for the loops
	int i;
	//seeds the random function
	srand(seed);

	//printf("\nInt:\n");
	//show the starting variables to varify that it is working
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	
	//do arthmitic on the variables to test the alu's clock speed
	for(i=0;i<numberIter;i++){
		//do adition, subtraction, and multiplication of the vars
		//	and use the masks to keep the size ok.
		temp1=(thirdar[i]+secondar[i])&bitMask16and;
        temp2=(thirdar[i]-firstar[i])&bitMask16and;
        temp3=(firstar[i]*secondar[i])&bitMask16and;
		//feed the calculated numbers back into the variables so they
		//	are always new and different
		firstar[i]=temp1;
		secondar[i]=temp2;
		thirdar[i]=temp3;
	}
	//varify that math was done
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	//return a new seed
	return rand()+firstar[numberIter-1];
}
int doubleArrayTest(int numberIter,int seed, double* firstar, double* secondar, double* thirdar){
	//printf("insideTest and seed=%d\n",seed);
	//memory locations used for the int test
	unsigned int temp1,temp2,temp3;

	//a counter for the loops
	int i;
	//seeds the random function
	srand(seed);

	//printf("\nInt:\n");
	//show the starting variables to varify that it is working
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	
	//do arthmitic on the variables to test the alu's clock speed
	for(i=0;i<numberIter;i++){
		//do adition, subtraction, and multiplication of the vars
		temp1=(thirdar[i]+secondar[i]);
        temp2=(thirdar[i]-firstar[i]);
        temp3=(firstar[i]*secondar[i]);
		//feed the calculated numbers back into the variables so they
		//	are always new and different
		firstar[i]=temp1;
		secondar[i]=temp2;
		thirdar[i]=temp3;
	}
	//varify that math was done
	//printf("first=%d,\t\tsecond=%d,\t\tthird=%d\n",first,second,third);
	//return a new seed
	return rand()+(int)firstar[numberIter-1];
}

//this is a wrapper function for intTest so it can be threaded
void* multiInt(void* args){
	int* values[2];
	values[0]= ((int**)args)[0];
	values[1]= ((int**)args)[1];
	//printf("values=%d,%d args=%d\n",*(values[0]),*(values[1]),args);
	//unpack the args and start the function
	*(values[1]) = intTest(*(values[0]),*(values[1]));
}

void* multiIntArray(void* args){
	int* values[2];
	int** arrays[3];
	values[0]= ((int**)args)[0];
	values[1]= ((int**)args)[1];
	arrays[0]= ((int***)args)[2];
	arrays[1]= ((int***)args)[3];
	arrays[2]= ((int***)args)[4];
	//printf("values=%d,%d args=%d\n",*(values[0]),*(values[1]),args);
	//unpack the args and start the function
	*(values[1]) = intArrayTest(*(values[0]),*(values[1]),*(arrays[0]),*(arrays[1]),*(arrays[2]));
}

void* multiDouble(void* args){
	int* values[2];
	values[0]= ((int**)args)[0];
	values[1]= ((int**)args)[1];
	//printf("values=%d,%d args=%d\n",*(values[0]),*(values[1]),args);
	//unpack the args and start the function
	*(values[1]) = doubleTest(*(values[0]),*(values[1]));
}

//this is a wrapper function for doubleArray so it can be threaded
void* multiDoubleArray(void* args){
	int* values[2];
	double** arrays[3];
	values[0]= ((int**)args)[0];
	values[1]= ((int**)args)[1];
	arrays[0]= ((double***)args)[2];
	arrays[1]= ((double***)args)[3];
	arrays[2]= ((double***)args)[4];
	//printf("values=%d,%d args=%d\n",*(values[0]),*(values[1]),args);
	//unpack the args and start the function
	*(values[1]) = doubleArrayTest(*(values[0]),*(values[1]),*(arrays[0]),*(arrays[1]),*(arrays[2]));
}

int main(){
	//the score of the current test
	int testScore=0;
	//the final score of the program
	int totalScore=0;
	//the total time of the whole program
	double totalTime=0.0;
	//the total number of iterations through each test
	int numberIter=100000000;
	//start and end time of each test
	double S,E;
	//create the threads
	pthread_t threads[31];
	//get an array of pointers for each thread
	void* threadArgs[31*2*sizeof(int*)];
	int threadIter[31];
	int threadSeed[31];
	void* arrayThreadArgs[31*5*sizeof(void*)];

	//for loops
	int i;
	//seeds the random vars
	int seed = time(NULL);
	
	//get the start time of the test///////////////////////////////////////////////////////////////////////////////////////////intTest
	S=get_walltime();
	//start the test
	seed = intTest(numberIter,seed);
	//see the final time after testing the calculation speed
	E=get_walltime();

	//create a arbitrary testing score
	testScore=(int) ((double)numberIter*0.68027210884/(1000000.0*(E-S)));
	//add the score to the total
	totalScore+=testScore;
	//get the total testing time
	totalTime+=(E-S);
	//show the user the time
	printf("int score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);

	int* firstin=malloc(sizeof(int)*numberIter);
	int* secondin=malloc(sizeof(int)*numberIter);
	int* thirdin=malloc(sizeof(int)*numberIter);

	for(i=0;i<numberIter;i++){
		firstin[i] = rand()&0xFF;
		secondin[i] = rand()&0xFF;
		thirdin[i] = rand()&0xFF;
	}

	S=get_walltime();/////////////////////////////////////////////////////////////////////////////////////////////////////////intArrayTest
	seed=intArrayTest(numberIter,seed,firstin,secondin,thirdin);
	E=get_walltime();

	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*0.84033613445/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=(E-S);
	//display the test score an the new totals
	printf("int array score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);

		//get the score for the integer multi threaded
	for(i=0;i<31;i++){
		threadIter[i]=numberIter/32;
		threadSeed[i]=seed+i;
		//printf("seed=%d\n",threadSeed[i]);
		threadArgs[i*2]=&threadIter[i];
		threadArgs[i*2+1]=&threadSeed[i];
	}
	S=get_walltime();//////////////////////////////////////////////////////////////////////////////////////////multiInt
	for(i=0;i<31;i++){
		int isWorking = pthread_create(&threads[i], NULL, multiInt, &threadArgs[i*2]);
		if(isWorking!=0){
			printf("Something went worng with threads ERR=%d\n",isWorking);
		}
	}
	seed=intTest(numberIter/32,seed+32);
	for(i=0;i<31;i++){
		int isWorking = pthread_join(threads[i],NULL);
		if(isWorking!=0){
			printf("Something went wrong finishing threads ERR=%d\n",isWorking);
		}
	}
	E=get_walltime();
	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*0.16995241332/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=E-S;
	//display the test score an the new totals
	printf("multithreaded int score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);




	for(i=0;i<numberIter;i++){
		firstin[i] = rand()&0xFF;
		secondin[i] = rand()&0xFF;
		thirdin[i] = rand()&0xFF;
	}

	int* threadFirstin[31];
	int* threadSecondin[31];
	int* threadThirdin[31];
	for(i=0;i<31;i++){
		threadIter[i]=numberIter/32;
		threadSeed[i]=seed+i;
		threadFirstin[i]=&firstin[(numberIter/32)*i];
		threadSecondin[i]=&secondin[(numberIter/32)*i];
		threadThirdin[i]=&thirdin[(numberIter/32)*i];
		arrayThreadArgs[i*5]=&threadIter[i];
		arrayThreadArgs[i*5+1]=&threadSeed[i];
		arrayThreadArgs[i*5+2]=&threadFirstin[i];
		arrayThreadArgs[i*5+3]=&threadSecondin[i];
		arrayThreadArgs[i*5+4]=&threadThirdin[i];
	}
	S=get_walltime();///////////////////////////////////////////////////////////////////////////////////////////multiIntArray
	for(i=0;i<31;i++){
		int isWorking = pthread_create(&threads[i], NULL, multiIntArray, &arrayThreadArgs[i*5]);
		if(isWorking!=0){
			printf("Something went worng with threads ERR=%d\n",isWorking);
		}
	}
	seed=intArrayTest(numberIter/32,seed+32,&firstin[(numberIter/32)*31],&secondin[(numberIter/32)*i],&thirdin[(numberIter/32)*i]);
	for(i=0;i<31;i++){
		int isWorking = pthread_join(threads[i],NULL);
		if(isWorking!=0){
			printf("Something went wrong finishing threads ERR=%d\n",isWorking);
		}
	}
	E=get_walltime();
	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*0.77519379845/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=E-S;
	//display the test score an the new totals
	printf("multithreaded array int score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);





	free(firstin);
	firstin=NULL;
	free(secondin);
	secondin=NULL;
	free(thirdin);
	thirdin=NULL;

	//do a test but for floating point arithmitic
	S=get_walltime();////////////////////////////////////////////////////////////////////////////////////////////////////////doubleTest
	seed=doubleTest(numberIter,seed);
	E=get_walltime();

	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*6.41025641026/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=E-S;
	//display the test score an the new totals
	printf("float score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);
	
	double* firstar=malloc(sizeof(double)*numberIter);
	double* secondar=malloc(sizeof(double)*numberIter);
	double* thirdar=malloc(sizeof(double)*numberIter);

	for(i=0;i<numberIter;i++){
		firstar[i] = (double)(rand())/100.0;
		secondar[i] = (double)(rand())/100.0;
		thirdar[i] = (double)(rand())/100.0;
	}

	S=get_walltime();/////////////////////////////////////////////////////////////////////////////////////////////////////doubleArrayTest
	seed=doubleArrayTest(numberIter,seed,firstar,secondar,thirdar);
	E=get_walltime();


	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*2.63157894737/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=(E-S);
	//display the test score an the new totals
	printf("double array score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);

	//get the score for the double multi threaded
	for(i=0;i<31;i++){
		threadIter[i]=numberIter/32;
		threadSeed[i]=seed+i;
		//printf("seed=%d\n",threadSeed[i]);
		threadArgs[i*2]=&threadIter[i];
		threadArgs[i*2+1]=&threadSeed[i];
	}
	S=get_walltime();/////////////////////////////////////////////////////////////////////////////////multiDouble
	for(i=0;i<31;i++){
		int isWorking = pthread_create(&threads[i], NULL, multiDouble, &threadArgs[i*2]);
		if(isWorking!=0){
			printf("Something went worng with threads ERR=%d\n",isWorking);
		}
	}
	seed=doubleTest(numberIter/32,seed+32);
	for(i=0;i<31;i++){
		int isWorking = pthread_join(threads[i],NULL);
		if(isWorking!=0){
			printf("Something went wrong finishing threads ERR=%d\n",isWorking);
		}
	}
	E=get_walltime();
	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*1.82481751825/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=E-S;
	//display the test score an the new totals
	printf("multithreaded double score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);

	for(i=0;i<numberIter;i++){
		firstar[i] = (double)(rand())/100.0;
		secondar[i] = (double)(rand())/100.0;
		thirdar[i] = (double)(rand())/100.0;
	}

	double* threadFirstar[31];
	double* threadSecondar[31];
	double* threadThirdar[31];
	for(i=0;i<31;i++){
		threadIter[i]=numberIter/32;
		threadSeed[i]=seed+i;
		threadFirstar[i]=&firstar[(numberIter/32)*i];
		threadSecondar[i]=&secondar[(numberIter/32)*i];
		threadThirdar[i]=&thirdar[(numberIter/32)*i];
		arrayThreadArgs[i*5]=&threadIter[i];
		arrayThreadArgs[i*5+1]=&threadSeed[i];
		arrayThreadArgs[i*5+2]=&threadFirstar[i];
		arrayThreadArgs[i*5+3]=&threadSecondar[i];
		arrayThreadArgs[i*5+4]=&threadThirdar[i];
	}
	S=get_walltime();///////////////////////////////////////////////////////////////////////////////////////////multiDoubleArray
	for(i=0;i<31;i++){
		int isWorking = pthread_create(&threads[i], NULL, multiDoubleArray, &arrayThreadArgs[i*5]);
		if(isWorking!=0){
			printf("Something went worng with threads ERR=%d\n",isWorking);
		}
	}
	seed=doubleArrayTest(numberIter/32,seed+32,&firstar[(numberIter/32)*31],&secondar[(numberIter/32)*i],&thirdar[(numberIter/32)*i]);
	for(i=0;i<31;i++){
		int isWorking = pthread_join(threads[i],NULL);
		if(isWorking!=0){
			printf("Something went wrong finishing threads ERR=%d\n",isWorking);
		}
	}
	E=get_walltime();
	//get the arbitrary test score
	testScore=(int)(((double)numberIter)*1.54320987654/(1000000.0*(E-S)));
	//calculate the total test score
	totalScore+=testScore;
	//get the total time
	totalTime+=E-S;
	//display the test score an the new totals
	printf("multithreaded array double score=%d, time=%f\n",testScore,E-S);
	printf("\ttotal score=%d, total time=%f\n",totalScore,totalTime);

	free(firstar);
	firstar=NULL;
	free(secondar);
	secondar=NULL;
	free(thirdar);
	thirdar=NULL;

	return 0;
}