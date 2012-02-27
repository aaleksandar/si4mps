#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

int n;
pthread_mutex_t mutex;
pthread_cond_t has_space;
pthread_cond_t has_items;

#define NUM_OF_PRODUCERS 2
#define NUM_OF_CONSUMERS 5
#define CAPACITY 6

#define MAX 50

double buffer[CAPACITY];
int fullness=0, num=0;


void* producer(void* arg) {
	cout << "producer";
	srand(time(0));
	
	while (1) {	
		pthread_mutex_lock(&mutex);
		
		while (fullness >= CAPACITY) {
			pthread_cond_wait(&has_space, &mutex);
		}
		
		if (num >= MAX) {
			pthread_cond_signal(&has_items);
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		double rand_n = (rand() / (double)RAND_MAX) * 100;
		buffer[fullness++] = rand_n;
		
		
		pthread_cond_signal(&has_items);
		pthread_mutex_unlock(&mutex);
	}
	
	
	pthread_exit(NULL);
	return 0;
}

void* consumer(void* arg) {
	
	while (1) {
		pthread_mutex_lock(&mutex);
		
		while (fullness <= 0) {
			pthread_cond_wait(&has_items, &mutex);
		}
		
		if (num >= MAX) {
			pthread_cond_signal(&has_space);
			pthread_mutex_unlock(&mutex);
			break;
		}
		
		cout << buffer[fullness-1] << endl;
		fullness--;
		

		num++;
		
		pthread_cond_signal(&has_space);
		pthread_mutex_unlock(&mutex);
	 	
	}
	pthread_exit(NULL);
	return 0;
}




int main() {
	pthread_t producers[NUM_OF_PRODUCERS];
	pthread_t consumers[NUM_OF_CONSUMERS];
	
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&has_space, 0);
	pthread_cond_init(&has_items, 0);

	for (int i=0; i < NUM_OF_PRODUCERS; i++)
		pthread_create(&producers[i], 0, producer, 0);
	for (int i=0; i < NUM_OF_CONSUMERS; i++)
		pthread_create(&consumers[i], 0, consumer, 0);

	for (int i=0; i < NUM_OF_PRODUCERS; i++) pthread_join(producers[i], 0);
	for (int i=0; i < NUM_OF_CONSUMERS; i++) pthread_join(consumers[i], 0);
	
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&has_space);
	pthread_cond_destroy(&has_items);
	
	pthread_exit(NULL);
    return 0;
}
