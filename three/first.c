//included libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//defintions
#define LOCKED  1 // value for LOCK if resource is unlocked
#define UNLOCKED  0 // value for LOCK if resource is locked
#define PROCESSES 8 // the number of process in the program
#define RESOURCES 2 // the number of resources in the program
#define SLOTS 3 // the number of slots a resource has

/***		DATA STRUCTURES 		**/

pthread_t threads[PROCESSES];
pthread_mutex_t mut;

struct Resource {
	int ID;
	pthread_t *curr_using[SLOTS]; // pointers to the threads using resource
	int LOCK; //resource locked until all three are done using it
};

struct Resource resources[RESOURCES]; // our systems resources

/*** 		FUNCTIONS 			**/

void init_resource(struct Resource *r)
/***********************************************
 * Name: init_resource
 * Description: Initializes a new resource struct
 * Parameters: none
 * Returns: a fresh Resource struct
 * Warning: none
 * *********************************************/
{
//	struct Resource r;
	r->ID = -1;

	int i;
	for(i=0; i<3; i++)
	{
		r->curr_using[i] = NULL;
	}

	r->LOCK = UNLOCKED;
}

int resource_open(struct Resource r)
/*********************************************
 * Name: resource_open
 * Description: checks to see if a resource is
 ** avaliable for a thread to use
 * parameters: the resource to check
 * returns: -1 if not open, if open it returns
 ** the index of the resource available, -2 there was an error
 * Warnins: none
 *********************************************/
{
	if(r.LOCK) {
		return -1; // resource is taken
	}

	int i;

	for(i=0;i<SLOTS;i++) {
		if(r.curr_using[i] == NULL) {
			return i;
		}
	}

	return -2; // all resources are being used, but function should never execute this line
}

int resource_lock(struct Resource *r, int index)
{
	if(r->curr_using[index] != NULL) {
		return -1;
	} else {
		r->curr_using[index] = 1;
	}
	
	int i, l=1;
	/* Check to see if this resource should be locked */
	for(i=0;i<SLOTS;i++)
	{
		if(r->curr_using[i] == NULL)
			l=0; /*still free resources*/
	}

	if(l)
		r->LOCK = LOCKED;

	return 0;
}

int resource_unlock(struct Resource *r, int index)
{
	r->curr_using[index] = NULL; /*Free the resource*/

	/* If the resource is locked, check to see if it is elligible to be unlocked */
	if(r->LOCK) {
		int i,u=0;
		for(i=0;i<SLOTS;i++)
		{
			if(r->curr_using[i] != NULL)
				u = 1; /* Not ready to be unlocked */
		}
		if(u)
			r->LOCK = UNLOCKED;

	}
	return 0;
}

static void * init_process(void * i)
{
	int id = (int) i;
	/* The Process will run forever here */
	while(1)
	{
//		pthread_mutex_lock(&mut);
		/* Check to see if there is a resource to use */
		int i, index;
		for(i=0;i<RESOURCES;i++)
		{
			index = resource_open(resources[i]);
			if(index != -1)
				break;
		}
		/* If there is a resource to use, use it. Else sleep and check again later */
		if(index != -1) {
			/*Found a resource to use. Use it */
			printf("THREAD ID %d: found resource (%d) to use\n", id, i);

			resource_lock(&resources[i], index);

			int to_sleep = rand()%10+1;
			printf("THREAD ID %d: Locking resource and sleeping for %d second(s)\n", id, to_sleep);
			sleep(to_sleep);

			printf("THREAD ID %d: Done with resource %d, now freeing\n", id, i);
			resource_unlock(&resources[i], index);
			
		} else {
			/* sleep */

			int to_sleep = rand()%8+1;
			printf("THREAD ID %d: All resources in use. Sleeping for %d second(s)\n", id, to_sleep); 
			sleep(to_sleep); 
		}		

//		pthread_mutex_unlock(&mut);
	}

	return;
}

/***			MAIN 			***/
int main(void)
{
	srand(time(NULL));
	int i;
	
	for(i=0; i<RESOURCES; i++)
	{
		init_resource(&resources[i]);
	}

	for(i=0;i<PROCESSES;i++)
	{
		pthread_create(&threads[i], NULL, init_process, (void *) i);
	}

	for(i=0;i<PROCESSES;i++)
	{	
		pthread_join(threads[i], NULL);
	}	


	return 0;
}
