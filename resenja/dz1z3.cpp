#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

typedef struct {
	int id;
} thread_data;

pthread_mutex_t mutex_odd, mutex_even;
long product_odd=1, product_even=1;

void* obrada(void* arg) {
	thread_data* data = (thread_data*) arg;
	
	if (data->id % 2) {
		pthread_mutex_lock(&mutex_odd);
		product_odd *= data->id;
		pthread_mutex_unlock(&mutex_odd);
	}
	else {
		pthread_mutex_lock(&mutex_even);
		product_even *= data->id;
		pthread_mutex_unlock(&mutex_even);
	}
	
	pthread_exit(NULL);
	return 0;
}


int main() {
	int n, rc;
	
	cout << "broj niti koje treba stvoriti: " << endl;
	fflush(stdout);
	cin >> n;
	
	pthread_t threads[n];
	thread_data data_array[n];
	pthread_mutex_init(&mutex_odd, 0);
	pthread_mutex_init(&mutex_even, 0);
	
	for (int i = 0; i < n; i++) {
		data_array[i].id = i+1;
		
		rc = pthread_create(&threads[i], NULL, obrada, (void*) &data_array[i]);
		
		if (rc) {
			cout << "error!";
			exit(-1);
		}
	}
	
	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], 0);
	}
	
	double res_odd, res_even;
	res_odd = pow(product_odd, 1. / ( n/2 + n%1 ));
	res_even = pow(product_even, 1. / ( n/2 ));
	
	cout << "Geometrijska sredina neparne grupe: " << res_odd << endl;
	cout << "Geometrijska sredina parne grupe: " << res_even << endl;
	
	pthread_mutex_destroy(&mutex_odd);
	pthread_mutex_destroy(&mutex_even);
	pthread_exit(NULL);
	
    return 0;
}
