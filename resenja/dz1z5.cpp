#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

typedef struct {
	int id;
	int begin, end;
	int y;
} thread_data;

int** mat;

int max_n, min_n;
pthread_mutex_t mutex_max, mutex_min;

#define NUM_OF_THREADS 5

void* obrada(void* arg) {
	thread_data* data = (thread_data*) arg;
	
	int local_max, local_min;
	
	local_max = local_min = mat[0][data->begin];
	
	for (int i = data->begin; i < data->end; i++) {
		for (int j = 0; j< data->y; j++) {
			if (mat[j][i] > local_max) local_max = mat[j][i];
			if (mat[j][i] < local_min) local_min = mat[j][i];
		}
	}
	while (local_max > max_n) {
		if (pthread_mutex_trylock(&mutex_max) == 0) {
			max_n = local_max;
			pthread_mutex_unlock(&mutex_max);
		}
	}
	
	while (local_min < min_n) {
		if (pthread_mutex_trylock(&mutex_min) == 0) {
			min_n = local_min;
			pthread_mutex_unlock(&mutex_min);
		}
	}
	
	pthread_exit(NULL);
	return 0;
}


int main() {
	int x, y, rc;
	
	cout << "broj kolona: " << endl;
	fflush(stdout);
	cin >> y;
	
	cout << "broj vrsta: " << endl;
	fflush(stdout);
	cin >> x;
	
	cout << "unesite elemente matrice: " << endl;
	mat = new int*[x];
	for (int i=0; i<x; i++) {
		mat[i] = new int[y];
		for (int j=0; j<y; j++) {
			cin >> mat[i][j];
		}
	}
	
	
	
	pthread_t threads[NUM_OF_THREADS];
	thread_data data_array[NUM_OF_THREADS];
	
	pthread_mutex_init(&mutex_max, 0);
	pthread_mutex_init(&mutex_min, 0);
	
	max_n = mat[0][0];
	min_n = mat[0][0];
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		data_array[i].id = i;
		if (NUM_OF_THREADS > y) {
			data_array[i].begin = i;
			data_array[i].end = i+1;
		}
		else {
			data_array[i].begin = i * ( y / (NUM_OF_THREADS) );
			data_array[i].end = (i+1) * ( y / (NUM_OF_THREADS) );	
		}
		if (i == NUM_OF_THREADS-1) data_array[i].end = y;
		
		data_array[i].y = x;
		
		rc = pthread_create(&threads[i], NULL, obrada, (void*) &data_array[i]);
		
		if (rc) {
			cout << "error!";
			exit(-1);
		}
	}
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], 0);
	}	
	
	
	cout << "maksimum: " << max_n << endl;
	cout << "minimum: " << min_n << endl;

	/*
	// ispis matrice
	for (int i=0; i<x; i++) {
		for (int j=0; j<y; j++) {
			cout << mat[i][j] << " ";
		}
		cout << endl;
		free (mat[i]);
	}
	 * */
	
	
	pthread_mutex_destroy(&mutex_max);
	pthread_mutex_destroy(&mutex_min);
	
	free(mat);
	pthread_exit(NULL);
    return 0;
}
