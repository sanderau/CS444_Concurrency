//libraries
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <semaphore.h>

//Defined variables that I will need
#define PHILOSOPHERS 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (i + 4) % PHILOSOPHERS
#define RIGHT (i + 1) % PHILOSOPHERS

//needed types of data

int state_of_philos[PHILOSOPHERS] = {0,0,0,0,0};
int philos[PHILOSOPHERS] = {0,1,2,3,4};
char *names_of_philosophers[PHILOSOPHERS] = {"Socrates", "Locke", "Kant", "Wittgenstein", "Nietzsche"};

sem_t mutex;
sem_t philo_sem[PHILOSOPHERS];

//functions

void can_eat(int i)
{
	if(state_of_philos[i] == HUNGRY && state_of_philos[LEFT] != EATING && state_of_philos[RIGHT] != EATING) {
		state_of_philos[i] = EATING;
		int time_to_eat = (rand()%7)+2;

		printf("%s(%d) is taking forks %d %d\n",
			names_of_philosophers[i], i+1 ,LEFT, RIGHT);
		printf("%s(%d) is eating for %d seconds\n",
			names_of_philosophers[i], i+1, time_to_eat);

		sleep(time_to_eat);

		sem_post(&philo_sem[i]);
	}
}

void take_fork(int i)
{
	sem_wait(&mutex);

	state_of_philos[i] = HUNGRY;

	printf("%s(%d) is hungies\n", 
		names_of_philosophers[i], i+1);

	can_eat(i);

	sem_post(&mutex);

	sem_wait(&philo_sem[i]);

	sleep(1);
}

void put_fork(int i)
{
	sem_wait(&mutex);

	state_of_philos[i] = THINKING;
	
	printf("%s(%d) is putting the forks down (R: %d, L: %d)\n",
		names_of_philosophers[i], i+1, RIGHT, LEFT);
	can_eat(LEFT);
	can_eat(RIGHT);

	sem_post(&mutex);
}


void *philosopher(void *index) 
{
	while(1) {
		int *i = index;
		int time_to_think = (rand()%20)+1;

		printf("Philosopher %s(%d) is thinking for %d seconds\n", 
			names_of_philosophers[*i], (*i)+1, time_to_think);
		sleep(time_to_think);
		
		take_fork(*i);
		
		sleep(0);

		put_fork(*i);
	}
}

//main
int main(void)
{
	srand(time(NULL));
	int i;
	pthread_t thread_id[PHILOSOPHERS];

	//init sems

	sem_init(&mutex, 0, 1);

	for(i = 0; i < PHILOSOPHERS; i++) {
		sem_init(&philo_sem[i], 0, 0);
	}

	//init philosophers

	for(i = 0; i < PHILOSOPHERS; i++) { 
		pthread_create(&thread_id[i], NULL, philosopher, &philo_sem[i]);
	}

	for(i = 0; i < PHILOSOPHERS; i++) {
		pthread_join(thread_id[i], NULL);
	}

	return 0;
}
