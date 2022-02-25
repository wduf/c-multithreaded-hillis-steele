#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#define MAX_LINE_SIZE 256
pthread_mutex_t lock;
pthread_cond_t cond;
static int size_g, nt_g;
static int nt_g;
int* input_g;
int* mids_g;
int* sums_g;
int tr_g, set_g = 0;
void printSums(int size){
	for(int i = 0; i < size; i++){
		printf("%d\n", sums_g[i]);
	}
}
void barrier(){
	pthread_mutex_lock(&lock);
	tr_g++;
	if(tr_g < nt_g){
		while(pthread_cond_wait(&cond, &lock) != 0);
	} else{
		tr_g = 0;
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&lock);
	return;
}
void copyMids(int step){
	pthread_mutex_lock(&lock);
	if(set_g != step)
	{
		memcpy(input_g, mids_g, (size_g * sizeof(int)));
		set_g++;
	}
	pthread_mutex_unlock(&lock);
}
void* threadFunction(void* arg)
{
	int tn = *((long*) arg);
	int step = 1;
	int jump = 1;
	int jumps_left;
	int index;
	while(jump < size_g){
		index = ((size_g - 1) - (tn - 1));
		for(jumps_left = (size_g - jump); jumps_left >= nt_g; jumps_left -= nt_g){
			mids_g[index] = (input_g[index] + input_g[index - jump]);
			index -= nt_g;
		}
		if(jumps_left >= tn){
			mids_g[index] = (input_g[index] + input_g[index - jump]);
		}
		barrier();
		copyMids(step);
		barrier();
		step++;
		jump = (1 << (step - 1));
	}
	for(int i = 0; i < size_g; i++){
		sums_g[i] = input_g[i];
	}
	return 0;
}
void production(int* input, int size, int nt){
	pthread_t* threads = (pthread_t*) malloc(nt * sizeof (pthread_t));
	int tns[nt];
	for(int i = 0; i < nt; i++){
		tns[i] = (i + 1);
		pthread_create(&threads[i], NULL, threadFunction, (void*) &tns[i]);
	}
	for(int i = 0; i < nt; i++){
		pthread_join(threads[i], NULL);
	}
	printSums(size);
	free(threads);
	return;
}

int main(){
	static int size = 20;
	static int nt = 4;
	int array_size = (size * sizeof(int));
	int* input = (int*) malloc(array_size);
	int* mids = (int*) malloc(array_size);
	int* sums = (int*) malloc(array_size);
	for(int i = 0; i < size; i++){
		input[i] = 1;
	}
	memcpy(mids, input, array_size);
	memcpy(sums, input, array_size);
	size_g = size;
	nt_g = nt;
	input_g = input;
	mids_g = mids;
	sums_g = sums;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	production(input, size, nt);
	free(input);
	free(mids);
	free(sums);
	return 0;
}
