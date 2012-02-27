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
	int in_circle;
} thread_data;

void* obrada(void* arg) {
	thread_data* data = (thread_data*) arg;
	
	double x, y, xy;
	
	
	srand(time(0));
	for (int i = 0; i < data->num_of_dots; i++) {
		x = (double)rand() / (double)RAND_MAX;
		y = (double)rand() / (double)RAND_MAX;
		
		xy = x*x + y*y;
		if (xy < 1) data->in_circle++;
	}
	
	pthread_exit(NULL);
	
	return 0;
}


int main() {

	pthread_t threads[NUM_OF_THREADS];
	
	int num_of_dots;
	thread_data data[NUM_OF_THREADS];
	
	int rc;
	
	cout << "broj tacaka za obradu: ";
	fflush(stdout);
	cin >> num_of_dots;
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		data[i].id = i;
		data[i].num_of_dots = num_of_dots / NUM_OF_THREADS;
		data[i].in_circle = 0;
		
		// poslednja dobija ostatak
		if (i == NUM_OF_THREADS - 1) {
			data[i].num_of_dots = num_of_dots / NUM_OF_THREADS + num_of_dots % NUM_OF_THREADS;
		}
		
		rc = pthread_create(&threads[i], NULL, obrada, (void*) &data[i]);
		
		if (rc) {
			cout << "error!";
			exit(-1);
		}
	}
	
	int sum_in_circle =0;
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], 0);
		sum_in_circle += data[i].in_circle;
	}
	
	double pi = 4.0 * (double)sum_in_circle/(double)num_of_dots;
	
	cout << pi << endl;
	
	pthread_exit(NULL);
	
    return 0;
}

