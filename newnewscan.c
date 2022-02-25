// INCLUDES:

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.c>

// GLOBALS:

pthread_mutex_t mutex;  // mutually exclusive lock
int* input_g;  // global input array
int* sums_g;  // global prefix sum array
int size_g;  // global size
int nt_g;  // global number of threads
int t_ready_g = 0;  // how many threads waiting at barrier
int t_ready2_g = 0;

// FUNCTIONS:

void printSums()
{
	for(int i = 0; i < size_g; i++)
	{
		printf("%d\n", sums_g[i]);
	}
}

void* threadFunction(void* arg)
{
	int t_n = *((long*) arg);  // thread number of this thread
	int loop = 1;  // loop counter
	int pos = 0;  // starting position in array
	int index;  // variable used to decrease total calculations

	for(;;)
	{  // infinite loop we will break out of
		index = (pos + (t_n - 1));
		if(index >= size_g)
		{  // if index is out of bounds
			break;  // this thread has done all of its calculations
		}
		sums_g[index] = sums_g[pos - 1];  // set this sum to the last sum of previous loop]
		for(int i = 0; i < t_n; i++)
		{  // add up sums 
			sums_g[index] += input_g[pos + i];
		}
		t_ready_g++;
		while(t_ready_g < (nt_g * loop)); // barrier
		pos += nt_g;
		loop++;
	}
}

void production(int* input, int size, int nt)
{
	pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t));
	int t_ns[size];  // thread numbers/ids

	for(int i = 0; i < nt; i++)
	{  // create (nt) threads
		t_ns[i] = (i + 1);  // thread number/id
		pthread_create(&threads[i], NULL, threadFunction, (void*) &t_ns[i]);
	}
	for(int i = 0; i < nt; i++)
	{  // wait for all threads to finish executing
		pthread_join(threads[i], NULL);
	}
	printSums();

	return;
}

int main()
{
	int input[14] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	int size = 14;
	int nt = 2;

	int sums[size];  // put on heap
	for(int i = 0; i < size; i++)
	{
		sums[i] = 0;
	}

	input_g = input;
	sums_g = sums;
	size_g = size;
	nt_g = nt;

	production(input, size, nt);

	return 0;
}