//included libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

//definitions

#define MAX_WAIT_TIME 10
#define RUNNING 1
#define MAX_CHAIRS 10
#define MAX_THREADS 32

#define SLEEPING 1
#define AWAKE 0

/*================================================================*/

/***		Data Structures, and global variables 		***/

int curr_threads=0;
int barber_status=0;
int haircut_time;
int curr_customers=0;
int customers_served=0;
char *seconds[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};

/* Mutexes for accessing the shop, getting a haircut, and sleeping*/
pthread_mutex_t barber_shop_mut;
pthread_mutex_t hair_cut_mut;
pthread_mutex_t barber_status_mut;

/* Conditions for sleeping and working */
pthread_cond_t barber_working_cond;
pthread_cond_t barber_sleep_cond;


/***		End of Data Structs + Vars			***/

/*===============================================================*/

/***		Functions					***/

void get_hair_cut();
void *barber_shop();
void *create_customers();
void *enter_customer();
void customer();

void get_hair_cut()
{
	/* Iterate the number of customers served */
	customers_served++;
	
	if(haircut_time==1) /*if wait time is one second print correct time message, else print the other */
		printf("Customer #%d is getting a haircut, it will take about %s second...\n", customers_served, seconds[haircut_time-1]);
	else
		printf("Customer #%d is getting a haircut, it will take about %s seconds...\n", customers_served, seconds[haircut_time-1]);
		
	sleep(haircut_time);
}

void *create_customers()
{
	pthread_t possible_customers[MAX_CHAIRS*3];
	pthread_attr_t customers_attr[MAX_CHAIRS*3];

	while(RUNNING) {
		
		int sleep_time = rand()%5+1;

		sleep(sleep_time);

		if(curr_threads < MAX_THREADS) {
			curr_threads++;
			pthread_create(&possible_customers[curr_threads-1], &customers_attr[curr_threads-1], enter_customer, NULL);
		}

	}
}

void *enter_customer() 
{
	pthread_mutex_lock(&barber_shop_mut);
	curr_customers++;
	printf("Customer has entered the shop, they are checking for open seats...\n");
	customer();
}

void *barber_shop()
{
	while(RUNNING)
	{
		/* If no customers, got to sleep. Else service the customers */
		if(curr_customers == 0) {
			pthread_mutex_lock(&barber_status_mut);
			
			barber_status = SLEEPING;
			printf("The barber has no customers to help, so he is going to take a nap...\n");
			
			pthread_cond_wait(&barber_sleep_cond, &barber_status_mut);
			
			barber_status = AWAKE;
			printf("A customer woke up the barber...\n");
		} else {
			haircut_time = rand()%MAX_WAIT_TIME+1;
			printf("The barber is helping the next customer...\n");
			pthread_cond_signal(&barber_working_cond);
			
			if(haircut_time==0)
				printf("The barber is cutting the hair for %s second...\n", seconds[haircut_time-1]);
			else 
				printf("The barber is cutting the hair for %s seconds...\n", seconds[haircut_time-1]);

			sleep(haircut_time);
		}
	}
}

void customer()
{
	if(curr_customers<MAX_CHAIRS && curr_customers >= 0) {
		if(barber_status == SLEEPING) {
			printf("The customer pokes the snoring barber to wake him up...\n");
			pthread_cond_signal(&barber_sleep_cond);
		}	

		pthread_mutex_unlock(&barber_shop_mut);

		pthread_mutex_lock(&hair_cut_mut);
		pthread_cond_wait(&barber_working_cond, &hair_cut_mut);
		get_hair_cut();
		pthread_mutex_unlock(&hair_cut_mut);

		pthread_exit(0);
		curr_customers--;

	} else if(curr_customers>=MAX_CHAIRS){
		pthread_mutex_unlock(&barber_shop_mut);
		curr_customers--;
		printf("There are no more chairs, the customer will now leave...\n");
		pthread_exit(0);
	} else {
		fprintf(stderr, "An error occured\nExiting...\n");
		exit(1);
	}
}

/***		End of function					***/



int main(void)
{
	printf("HEllo world!\n");
	srand(time(NULL));
	
	pthread_t barber;
	pthread_t customers;

	pthread_attr_t barber_attr;
	pthread_attr_t customers_attr;

	pthread_attr_init(&barber_attr);
	pthread_attr_init(&customers_attr);

	pthread_create(&barber, &barber_attr, barber_shop, NULL);
	pthread_create(&customers, &customers_attr, create_customers, NULL);

	return 0;
}
