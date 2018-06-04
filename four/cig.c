//included libraries
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define RUNNING 1
#define ON_TABLE 1
#define OFF_TABLE 0

pthread_cond_t smoke_cig;
pthread_mutex_t table_mut;

struct Table {
	int tabacco;
	int paper;
	int matches;
} typedef Table;

Table table;
char *options[3] = {"Tabacco", "Paper", "Matches"};

void *agent_function();
void *matches_function();
void *paper_function();
void *tabacco_function();

void *agent_function()
{
	while(RUNNING) {
		pthread_mutex_lock(&table_mut);
		
		printf("== The Agent is clearing the table...\n");
		table.tabacco = table.paper = table.matches = OFF_TABLE; /* Clear the table */
		int one, two;
		
		one = rand()%3+1;

		do {
			two = rand()%3+1;
		} while(one == two); /* Ensure unique values */
		
		printf("== The Agent randomly picked %s and %s\n", options[one-1], options[two-1]);

		if(one == 1 || two == 1) {
			table.tabacco = ON_TABLE;
		} if(one == 2 || two == 2) {
			table.paper = ON_TABLE;
		} if(one == 3 || two == 3) {
			table.matches = ON_TABLE;
		}

		sleep(1);

		pthread_cond_wait(&smoke_cig, &table_mut);
	
		pthread_mutex_unlock(&table_mut);
	}
}

void *matches_function() 
{
	while(RUNNING) {
		pthread_mutex_lock(&table_mut);

		printf("The man with matches is checking the table...\n");

		sleep(1);		

		if(table.tabacco == ON_TABLE && table.paper == ON_TABLE) {
			printf("== The man with matches can make a cigarette!\n== The man with matches is smoking a cigarette\n");
			pthread_cond_signal(&smoke_cig);
		} else {
			printf("The man with matches cannot make a cigarette\n");
		}

		pthread_mutex_unlock(&table_mut);

		sleep(2);
	}
}

void *tabacco_function() 
{
	while(RUNNING) {
		pthread_mutex_lock(&table_mut);

		printf("The man with tabacco is checking the table...\n");

		sleep(1);		

		if(table.matches == ON_TABLE && table.paper == ON_TABLE) {
			printf("== The man with tabacco can make a cigarette!\n== The man with tabacco is smoking a cigarette\n");
			pthread_cond_signal(&smoke_cig);
		} else {
			printf("The man with tabacco cannot make a cigarette\n");
		}

		pthread_mutex_unlock(&table_mut);
	
		sleep(2);
	}
}

void *paper_function() 
{
	while(RUNNING) {
		pthread_mutex_lock(&table_mut);

		printf("The man with paper is checking the table...\n");

		sleep(1);		

		if(table.tabacco == ON_TABLE && table.matches == ON_TABLE) {
			printf("== The man with paper can make a cigarette!\n== The man with paper is smoking a cigarette\n");
			pthread_cond_signal(&smoke_cig);
		} else {
			printf("The man with paper cannot make a cigarette\n");
		}

		pthread_mutex_unlock(&table_mut);
	
		sleep(2);
	}
}




int main(void)
{
	srand(time(NULL));
	table.tabacco = table.paper = table.matches = OFF_TABLE;

	pthread_t Agent;
	pthread_t Matches;
	pthread_t Paper;
	pthread_t Tabacco;

	pthread_create(&Agent, NULL, agent_function, NULL);
	pthread_create(&Matches, NULL, matches_function, NULL);
	pthread_create(&Paper, NULL, paper_function, NULL);
	pthread_create(&Tabacco, NULL, tabacco_function, NULL);

	pthread_join(Agent, NULL);
	pthread_join(Matches, NULL);
	pthread_join(Tabacco, NULL);
	pthread_join(Paper, NULL);

	return 0;
}
