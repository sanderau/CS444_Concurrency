#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

/* definitions */
#define INSERTER 0
#define SEARCHER 1
#define DELETER 2
#define SIZE 32

#define THREADS 9 /* number of threads */

pthread_t threads[THREADS];
pthread_mutex_t mut;

struct lazyQueue {
	int pos;
	int list[SIZE];
};

struct lazyQueue queue;

/*		QUEUE OPERATIONS 		*/


void init_queue(struct lazyQueue *l)
{
	l->pos = 0;

	int i;
	for(i=0;i<SIZE;i++)
	{
		l->list[i]=0;
	}	
}	

void add_queue(struct lazyQueue *q, int val)
{
	if(q->pos >= SIZE)
		q->pos = 0;

	q->list[q->pos] = val;
}	

void remove_queue(struct lazyQueue *q, int position)
{
	/*If there was an error trying to access the queue print error and exit */
	if(position < 0 || position >= SIZE)
	{
		fprintf(stderr, "Problem with deleting from queue\n");
		exit(1);
	}

	q->list[position] = 0; /* "Delete" the value */
	/* I don't want to really write a queue for a program that is about concurrency */
}

int search_queue(struct lazyQueue *q, int value)
{
	int i;
	
	for(i=0;i<SIZE;i++)
	{
		if(q->list[i] == value)
		{
			return i; /*return the index if the value matches */
		}
	}

	return -1; /* return -1 if there was an error */
}

/*		THREAD OPERATIONS		*/

static void *thread_init(void * __id)
{
	int id = (int) __id;

	while(1)
	{
		if(id==INSERTER) {
			int val_ins = rand()%100+1; /*value to insert */
			
			printf("INSERTER: I am adding value %d\n", val_ins);

			add_queue(&queue, val_ins);

			printf("INSERTER: Value successfuly inserted\n");
			
			sleep(1);
		} else if(id==SEARCHER) {

			int search = rand()%100+1; /*value to search for*/

			printf("SEARCHER: Looking for value: %d\n", search);

			int index = search_queue(&queue, search);

			if(index == -1) {
				printf("SEARCHER: Did not find value\n");
			} else {
				printf("SEARCHER: Found value at index %d\n", index);
			}

			sleep(1);
		} else if(id==DELETER) {
			pthread_mutex_lock(&mut);
			
			int index = rand()%32;
			printf("DELETER: Deleting value at %d\n", index);

			remove_queue(&queue, index);

			printf("DELETER: Successfuly deleted listing\n");							

			pthread_mutex_unlock(&mut);


			sleep(1);
		}

	}

	return 0;
}


int main(void)
{
	srand(time(NULL));

	int i;
	init_queue(&queue);

	for(i=0;i<THREADS;i++)
	{
		int id = i%3;
		pthread_create(&threads[i], NULL, thread_init,(void *) id);
	}

	for(i=0;i<THREADS;i++)
	{
		pthread_join(threads[i], NULL);		
	}	

	return 0;
}
