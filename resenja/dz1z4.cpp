#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

typedef struct {
	int id;
	int begin, end;
} thread_data;

int* niz;

#define NUM_OF_THREADS 5

void* obrada(void* arg) {
	thread_data* data = (thread_data*) arg;
	
	for (int i = data->begin; i < data->end; i++) {
		niz[i] *= -1;
	}
	
	pthread_exit(NULL);
	return 0;
}


int main() {
	int n, rc;
	
	cout << "velicina niza: " << endl;
	fflush(stdout);
	cin >> n;
	
	cout << "unesite elemente niza: " << endl;
	niz = new int[n];
	for (int i=0; i<n; i++) {
		fflush(stdout);
		cin >> niz[i];
	}
	
	pthread_t threads[NUM_OF_THREADS];
	thread_data data_array[NUM_OF_THREADS];
	
	// ovo je "ravnomerno" za n % NUM_OF_THREADS > 0
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		data_array[i].id = i;
		data_array[i].begin = i * ( n / (NUM_OF_THREADS) );
		data_array[i].end = (i+1) * ( n / (NUM_OF_THREADS) );
		
		rc = pthread_create(&threads[i], NULL, obrada, (void*) &data_array[i]);
		
		if (rc) {
			cout << "error!";
			exit(-1);
		}
	}
	
	for (int i = n - n % NUM_OF_THREADS; i < n; i++) {
		niz[i] *= -1;
	}
	
	
	for (int i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], 0);
	}
	
	for (int i = 0; i < n; i++) {
		cout << niz[i] << " ";
	}

	cout << endl;
	
	
	
	free(niz);
	pthread_exit(NULL);
	
    return 0;
}
