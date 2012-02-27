#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

int n=0;
int main_start=0;
pthread_mutex_t mutex;
#define NUM_OF_THREADS 5

typedef struct {
	int id, start;
} thread_data;



void* nit(void* arg) {
	thread_data* data = (thread_data*)arg;

	pthread_mutex_lock(&mutex);
	while (data->start == 0) {
		pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex);
	}
	
	int k;
	cout << "unesi broj za nit " << data->id << ": "; 
	fflush(stdout);
	cin >> k;
	n += k;
	
	if (data->id == NUM_OF_THREADS-1) {
		main_start = 1;
		pthread_mutex_unlock(&mutex);
		pthread_exit(NULL);
		return 0;
	}
	
	data[1].start = 1;
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
	return 0;
}


int main() {
	pthread_mutex_init(&mutex, 0);
	
	pthread_t threads[NUM_OF_THREADS];
	thread_data data[NUM_OF_THREADS];
	
	pthread_mutex_lock(&mutex);
	
	for (int i=0; i<NUM_OF_THREADS; i++) {
		data[i].id = i;
		data[i].start = 0;
		pthread_create(&threads[i], 0, nit, &data[i]);
	}
	
	data[0].start = 1;
	
	while (!main_start) {
		pthread_mutex_unlock(&mutex);
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	cout << "suma: " << n << endl;
	
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
    return 0;
}
