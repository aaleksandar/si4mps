#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

#define NUM_OF_THREADS 5

using namespace std;

typedef struct {
	int id;
	int num_of_dots;
//	int in_circle;
} thread_data;

int in_circle = 0;
pthread_mutex_t mutex;

void* obrada(void* arg) {
	thread_data* data = (thread_data*) arg;
	
	double x, y, xy;
	
	
	srand(time(0));
	for (int i = 0; i < data->num_of_dots; i++) {
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		
		xy = x*x + y*y;
		
		pthread_mutex_lock(&mutex);
		if (xy < 1) in_circle++;
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_exit(NULL);
	
	return 0;
}


int main() {

	pthread_t threads[NUM_OF_THREADS];
	thread_data data_array[NUM_OF_THREADS + 1];
	int num_of_dots;
	
	pthread_mutex_init(&mutex, 0);
	
	int rc;
	
	cout << "broj tacaka za obradu: " << endl;
	fflush(stdout);
	cin >> num_of_dots;
	
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		data_array[i].id = i;
		data_array[i].num_of_dots = num_of_dots / NUM_OF_THREADS;
		
		rc = pthread_create(&threads[i], NULL, obrada, (void*) &data_array[i]);
		
		if (rc) {
			cout << "error!";
			exit(-1);
		}
	}
	
	// poslednja dobija ostatak
	thread_data data = data_array[NUM_OF_THREADS];
	data.num_of_dots = data_array[NUM_OF_THREADS].num_of_dots = num_of_dots % NUM_OF_THREADS;

	srand(time(0));
	double x, y, xy;
	for (int i = 0; i < data.num_of_dots; i++) {
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		
		xy = x*x + y*y;
		
		pthread_mutex_lock(&mutex);
		if (xy < 1) in_circle++;
		pthread_mutex_unlock(&mutex);
	}
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], 0);
	}
	
	double pi = 4.0 * (double)in_circle/(double)num_of_dots;
	
	cout << "Pi: " << pi << endl;
	
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
	
    return 0;
}
