#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

int n=0;
pthread_mutex_t mutex;
pthread_cond_t main_cond;

#define NUM_OF_THREADS 5

typedef struct {
	int id, start;
	pthread_cond_t cond;
} thread_data;



void* nit(void* arg) {
	thread_data* data = (thread_data*)arg;
	
	pthread_mutex_lock(&mutex);
	if (data->start == 0) {
		pthread_cond_wait(&data->cond, &mutex);
	}
	
	int k;
	cout << "unesi broj za nit " << data->id << ": "; 
	fflush(stdout);
	cin >> k;
	n += k;
	
	if (data->id == NUM_OF_THREADS-1) {
		pthread_cond_signal(&main_cond);
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	
	data[1].start = 1;
	pthread_cond_signal(&(data[1].cond));
		
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(NULL);
	return 0;
}


int main() {
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&main_cond, 0);
	
	pthread_t threads[NUM_OF_THREADS];
	thread_data data[NUM_OF_THREADS];
	
	for (int i=0; i<NUM_OF_THREADS; i++) {
		data[i].id = i;
		data[i].start = 0;
		pthread_cond_init(&(data[i].cond), 0);
		pthread_create(&threads[i], 0, nit, &data[i]);
	}
	
	data[0].start = 1;
	
	pthread_mutex_lock(&mutex);
	//
	pthread_cond_signal(&(data[0].cond));
	pthread_cond_wait(&main_cond, &mutex);
	//
	pthread_mutex_unlock(&mutex);
	
	cout << "suma: " << n << endl;
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&main_cond);
	
	pthread_exit(NULL);
    return 0;
}
