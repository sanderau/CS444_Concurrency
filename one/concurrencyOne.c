#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#include "mt19937ar.c"

#define BUFFER_MAX 32

//structs

typedef struct {
	int wait; // how long the consumer thread has to wait
	int value; // the value it will print
} Event;

typedef struct {
	Event eventBuffer[BUFFER_MAX];//the actual buffer
	pthread_mutex_t lock;//the mutex lock
	unsigned int consumerIndex;
	unsigned int producerIndex;
	pthread_cond_t consumerCondition;
	pthread_cond_t producerCondition;
} Buffer;

//global variables

int BITS; // see what kind of system this is so we use the appropriate f(x)
Buffer buffer; // buffer data struct that will hold the all the needed data for the two pthreads


//functions

void howManyBitsIsItLetsFindOut(void)
/***************************************************
 * Function: howManyBitsIsItLetsFindOut
 * Description: this function lets the program know
 * * if it is working with a 32 bit system, or a 64 bit
 * * system. This will effect the random number function
 * Input: none;
 * Output: none
 * Warnings: this will effect the global variable BITS
 * ***************************************************/
{
	unsigned int eax= 0x01;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	__asm__ __volatile__(
				"cpuid;"
				: "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
				: "a"(eax)
				);

	if(ecx & 0x40000000)
	{
		BITS = 1;//the register only has 32 bits, so 32 bit system
	}

	else
	{
		BITS = 0; // 64 bit system
	}
}

int randNum(int high, int low)
/*********************************************
 * Function: randNum
 * Description: produces a random number in the given
 * * range
 * Input: the high and low
 * Output: the random number
 * *********************************************/
{
	unsigned long num = 0;

	if(BITS == 0)
	{
		num = (int)genrand_int32(); //32-bit system us mt
	}

	else
	{
		__asm__("rdrand %0" 
					: "=r"(num));//64bit system use inline asm and rdrand
	}


	printf("num: %d \n", num);
	num = (num %(high-low));

	if(num < low)
	{
		return low;
	}	

	return num;
}

void *consFunction(void *lol)
/***********************************************************
 * Function: consFUnction
 * Description: the consumer
 * input: none
 * output: none
 * ********************************************************/
{
	while(1)
	{
		
	}
}

void *prodFunction(void *lol)
/*************************************************************
 * Function: prodFunction
 * Description: producer function
 * input: none
 * output: none
 *************************************************************/
{
	while(1)
	{
		pthread_mutex_lock(&buffer.lock);
	}
}


/***************************MAIN****************************/
int main(int argc, char **argv)
{
	//intiate pthreads
	pthread_t threads[2];

	howManyBitsIsItLetsFindOut();//figure out what kind of registers we are working witth
	buffer.producerIndex = buffer.consumerIndex = 0; // set the index to 0

	

	return 0;
}
