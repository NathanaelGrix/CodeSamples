#include <iostream>
#include <bits/stdc++.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void get_walltime(double* wcTime) {

     struct timeval tp;

     gettimeofday(&tp, NULL);

     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);

}
/****************************************************************
 *	dec2bin
 *	
 *	Populates a structure with a binary array representing the value.
 *
 *	ex: input = 0xDCBA
 *	output A[] = 0 1 0 1   1 1 0 1   0 0 1 1   1 0 1 1
 *	         A         B         C         D
 *
 */
void dec2binOLD(long int decimalValue, bool *binaryArray, int numBits)
{
	int bp = numBits;
	long int N = decimalValue;

	for (int x = 0; x < numBits; x++)
		binaryArray[x] = 0;

	while(N > 0)
	{
		while(N < (pow(2.0, bp)))
		{
			bp = bp - 1.0;
		};
		binaryArray[(int)bp] = 1;
		N = N - pow(2.0, bp);
	};
}
//we can avoid doing things that take effort if we just use bitwise opperations
void dec2bin(long int decimalValue, bool *binaryArray, int numBits)
{
	int i = 0;
	for (i = 0; i < numBits - 8; i+=8){
		//just grab the least significant bit
		//put it into the lowest part of the array for a bite
		binaryArray[i] = 1 & decimalValue;
		binaryArray[i+1] = 2 & decimalValue;
		binaryArray[i+2] = 4 & decimalValue;
		binaryArray[i+3] = 8 & decimalValue;
		binaryArray[i+4] = 16 & decimalValue;
		binaryArray[i+5] = 32 & decimalValue;
		binaryArray[i+6] = 64 & decimalValue;
		binaryArray[i+7] = 128 & decimalValue;
		//shift everything down a bite
		decimalValue = decimalValue >> 8;
	}
	//if there is anything left over just do that
	for (i ;i < numBits; i++){
		binaryArray[i] = 1 & decimalValue;
		decimalValue = decimalValue >> 1;
	}
}

// print the contents of an array
void printArray(bool *array, int numBits)
{
	for (int ii = 0; ii < numBits; ii++)
		printf("%i ", array[numBits - ii - 1]);
	printf("\n");
}

int main(int argc, char **argv)
{
	bool binArray[32];
	long int decimal;

	double d_S1, d_E1;

	int i_R = 10000000;

	srandom(153);
	
	get_walltime(&d_S1);
	for (uint32_t ii = 0; ii < i_R; ii++)
	{
		decimal = random();

		dec2bin(decimal, binArray, 32);
	}	
	get_walltime(&d_E1);

	//#ifdef DEBUG
	decimal = random();

	printf("The new array version sample output:\n");
	dec2bin(decimal, binArray, 32);
	printArray(binArray, 32);

	printf("the old array version sample output:\n");
	dec2binOLD(decimal, binArray, 32);
	printArray(binArray, 32);
	printf("Time dec2bin: %f\n", d_E1-d_S1);

	return 0;
}
