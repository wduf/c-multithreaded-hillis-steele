// INCLUDES:

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// STRUCTS:

typedef struct __args_t
{
	int* input;  // input array
	int* mids;  // middle sums array
	int* sums;  // final prefix sums array
	int* size;  // size of input array
	int* n_t;  // number of threads
	int t_n;  // thread number
	int* t_r;  // counter for ready threads
	int* t_r2;  // for second barrier
} args_t;

// GLOBALS:

pthread_mutex_t lock; // mutually exclusive lock for threads

// FUNCTIONS:

void printSums(int* sums, int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d\n", sums[i]);
	}
}

void initArgs(args_t* args, int* input, int* mids, int* sums, int* size, int* n_t, int t_n, int* t_r, int* t_r2)
{
	args->input = input;
	args->mids = mids;
	args->sums = sums;
	args->size = size;
	args->n_t = n_t;
	args->t_n = t_n;
	args->t_r = t_r;
	args->t_r2 = t_r2;
}

void* threadFunction(void* arg)
{
	args_t args = *((args_t*) arg);  // arguments for this function
	int step = 1;  // step counter
	int jump = 1;  // jump size, 2^0 = 1
	int jumps_left;  // variable to decrease total calculations
	int index;  // variable to decrease total calculations

	while(jump < *args.size)
	{
		printf("%d: jump = %d\n", args.t_n, jump); fflush(stdout);
		jumps_left = (*args.size - jump);
		printf("%d: jumps_left starts at %d\n", args.t_n, jumps_left); fflush(stdout);
		index = ((*args.size - 1) - (args.t_n - 1));
		printf("%d: index1 = %d\n", args.t_n, index); fflush(stdout);
		while(jumps_left >= *args.n_t)
		{  // if this thread is responsible for another jump
			args.mids[index] = (args.input[index] + args.input[index - jump]);
			printf("%d: setting mid[%d] = %d, ", args.t_n, index, args.mids[index]); fflush(stdout);
			printf("input[%d] = %d, input[%d] = %d\n", index, args.mids[index], index - jump, args.mids[index - jump]);
			jumps_left -= *args.n_t;
			printf("%d: now jumps left = %d\n", args.t_n, jumps_left); fflush(stdout);
			index -= *args.n_t;
			
		}
		//index -= *args.n_t;
		printf("%d: checking edge case, index here = %d\n", args.t_n, index); fflush(stdout);
		if(jumps_left >= args.t_n)
		{  // edge case
			printf("%d: %d >= %d\n", args.t_n, jumps_left, args.t_n);
			printf("%d: mids[%d] = index[%d] + index[%d]\n", args.t_n, index, index, index - jump);
			args.mids[index] = (args.input[index] + args.input[index - jump]);
			printf("%d: setting mid[%d] = %d\n", args.t_n, index, args.mids[index]); fflush(stdout);
		}
		pthread_mutex_lock(&lock);
		//printf("%d: locked, before t_r = %d\n", args.t_n, *args.t_r); fflush(stdout);
		*args.t_r += 1;
		pthread_mutex_unlock(&lock);
		//printf("%d: waiting..., t_r = %d\n", args.t_n, *args.t_r); fflush(stdout);
		while(*args.t_r < (step * *args.n_t));  // barrier
		//printf("%d: before, input[0] = %d\n", args.t_n, args.input[0]); fflush(stdout);
		memcpy(args.input, args.mids, (*args.size * sizeof(int)));
		//printf("-> %d: moving on\n", args.t_n);
		pthread_mutex_lock(&lock);
		//printf("%d: locked, before t_r = %d\n", args.t_n, *args.t_r); fflush(stdout);
		*args.t_r2 += 1;
		pthread_mutex_unlock(&lock);
		while(*args.t_r2 < (step * *args.n_t));
		step++;
		jump = (1 << (step - 1));
	}
	for(int i = 0; i < *args.size; i++)
	{  // set values in prefix sum array
		if(args.sums[i] != args.input[i])
		{
			printf("%d: sets sums[%d] to %d\n", args.t_n, i, args.input[i]); fflush(stdout);
			args.sums[i] = args.input[i];
		}
	}

	return 0;
}

void production(int* input, int* mids, int* sums, int* size, int* n_t, int* t_r, int* t_r2)
{
	pthread_t* threads = (pthread_t*) malloc(*n_t * sizeof(pthread_t));  // thread array
	args_t args[*size];  // struct array
	/*
		@todo
		OPTIONS:
		- have one struct that we pass the address of and hope the threads save it before it is altered
		- use init in pthread_create, but i'm not sure how the address of this would work (return a static?)
	*/

	//printf("got here\n"); fflush(stdout);
	for(int i = 0; i < *n_t; i++)
	{  // create threads
		initArgs(&args[i], input, mids, sums, size, n_t, (i + 1), t_r, t_r2);
		pthread_create(&threads[i], NULL, threadFunction, (void*) &args[i]);
	}
	//printf("got here\n"); fflush(stdout);
	for(int i = 0; i < *n_t; i++)
	{  // wait for threads to finish executing
		pthread_join(threads[i], NULL);
	}
	printSums(input, *size);  // maybe make it so we print as we go along
	//printf("got here\n");
}

int main()
{
	int input[32] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};  // input array
	int size = 16;  // size of input
	int n_t = 8;  // number of threads
	int* mids = (int*) malloc(size * sizeof(int));
	int* sums = (int*) malloc(size * sizeof(int));
	for(int i = 0; i < size; i++)
	{
		mids[i] = 0;
		sums[i] = 0;
	}
	memcpy(mids, input, (size * sizeof(int)));
	int t_r = 0;  // counter for threads ready
	int t_r2 = 0;  // counter for second barrier

	production(input, mids, sums, &size, &n_t, &t_r, &t_r2);

	return 0;
}
