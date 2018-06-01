//included libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//definitions

#define N_CHAIRS 8
#define WORKING 2
#define ASLEEP 1
#define AWAKE 0

/*================================================================*/

/***		Data Structures, and global variables 		***/


pthread_t threads[N_CHAIRS];
pthread_mutex_t mut;

struct Barber {
	int status;
	int working_chair;
}typedef Barber;


/***		End of Data Structs + Vars			***/

/*===============================================================*/

/***		Functions					***/


/***		End of function					***/



int main(void)
{
	printf("Hello world\n");

	return 0;
}
