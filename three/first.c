//included libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

//defintions
#define LOCKED  1
#define UNLOCKED  0
#define PROCESSES 8
#define RESOURCES 2

/***		DATA STRUCTURES 		**/

struct Process {
	int ID;
	pthread_t proc; // the actual process
	struct Resource *curr_resource; // current resource it is using
};

struct Resource {
	int ID;
	struct Process *curr_using[3]; // pointers to the threads using resource
	int LOCK; //resource locked until all three are done using it
};


struct Process processes[PROCESSES]; // our systems running threads
struct Resource resources[RESOURCES]; // our systems resources

/*** 		FUNCTIONS 			**/

struct Resource init_resource(int id)
/***********************************************
 * Name: init_resource
 * Description: Initializes a new resource struct
 * Parameters: none
 * Returns: a fresh Resource struct
 * Warning: none
 * *********************************************/
{
	struct Resource r;
	r.ID = id;

	int i;
	for(i=0; i<3; i++)
	{
		r.curr_using[i] = NULL;
	}

	r.LOCK = UNLOCKED;

	return r;
}

struct Process init_process(int id)
/********************************************
 * Name: init_process
 * Description: Initialize a new process structure
 * parameters: none
 * Returns: a freshly initialize process
 * Warnings: none
 * *******************************************/
{
	struct Process p;
	p.ID = id;	

	//TODO: bind p_thread
	
	p.curr_resource = NULL;

	return p;
}

/***			MAIN 			***/
int main(void)
{
	int i;
	
	for(i=0; i<RESOURCES; i++)
	{
		resources[i] = init_resource(i);
	}	

	return 0;
}
