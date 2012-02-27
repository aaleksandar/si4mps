#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace std;

int n;
pthread_mutex_t mutex;
pthread_cond_t cond_read;
pthread_cond_t cond_write;

#define NUM_OF_THREADS 2

void* nit1(void* arg) {
	while (n >= 0) {
		cout << "daj ceo broj: " << endl;
		fflush(stdout);
		cin >> n;
		
		pthread_mutex_lock(&mutex);
		
		pthread_cond_signal(&cond_write);
		
		pthread_cond_wait(&cond_read, &mutex);
		
		pthread_mutex_unlock(&mutex);
	}
	
	pthread_exit(NULL);
	return 0;
}

void* nit2(void* arg) {
	do {
		pthread_mutex_lock(&mutex);
		
		pthread_cond_wait(&cond_write, &mutex);
		cout << "unet broj: " << n << endl;
		
		pthread_cond_signal(&cond_read);
		
		pthread_mutex_unlock(&mutex);
	} while(n >= 0);
	
	pthread_exit(NULL);
	return 0;
}




int main() {
	pthread_t threads[NUM_OF_THREADS];
	
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&cond_read, 0);
	pthread_cond_init(&cond_write, 0);
	
	pthread_create(&threads[0], 0, nit1, 0);
	pthread_create(&threads[1], 0, nit2, 0);
	
	for (int i=0; i < NUM_OF_THREADS; i++) pthread_join(threads[i], 0);
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond_read);
	pthread_cond_destroy(&cond_write);
	
	pthread_exit(NULL);
    return 0;
}
