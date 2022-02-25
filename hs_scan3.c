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
	pthread_mutex_t* lock;  // mutually exclusive lock for threads
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

void initArgs(args_t* args, int* input, int* mids, int* sums, int* size, int* n_t, int t_n, int* t_r)
{
	args->input = input;
	args->mids = mids;
	args->sums = sums;
	args->size = size;
	args->n_t = n_t;
	args->t_n = t_n;
	args->t_r = t_r;
}

void* threadFunction(void* arg)
{
	args_t args = *((args_t*) arg);  // arguments for this function
	int step = 1;  // step counter
	int jump = 1;  // jump size, 2^0 = 1
	int jumps_left;  // variable to decrease total calculations
	int index;  // variable to decrease total calculations
	int index2;  // variable to decrease total calculations
	int i;  // accumulator for for loop that we need for edge case

	while(jump < *args.size)
	{
		printf("%d: jump = %d\n", args.t_n, jump); fflush(stdout);
		jumps_left = (*args.size - jump);
		printf("%d: jumps_left starts at %d\n", args.t_n, jumps_left); fflush(stdout);
		index = ((*args.size - 1) - (args.t_n - 1));
		for(i = 0; jumps_left >= *args.n_t; i++)
		{  // if this thread is responsible for another jump
			index2 = (index - (i * *args.n_t));
			//printf("%d: before: mid[%d] = %d\n", args.t_n, index2, args.mids[index2]);
			printf("%d: input[%d] = %d, input[%d] = %d\n", args.t_n, index2, args.input[index2], (index2 - jump), args.input[index2 - jump]); fflush(stdout);
			args.mids[index2] = (args.input[index2] + args.input[index2 - jump]);
			printf("%d: after: mid[%d] = %d\n", args.t_n, index2, args.mids[index2]); fflush(stdout);
			printf("decreasing jumps_left (%d) by %d\n", jumps_left, *args.n_t); fflush(stdout);
			jumps_left -= *args.n_t;
			printf("%d: now jumps left = %d\n", args.t_n, jumps_left); fflush(stdout);
			
		}
		printf("%d: index2 = %d\n", args.t_n, index2);
		printf("%d: checking edge case\n", args.t_n); fflush(stdout);
		if(jumps_left >= args.t_n)
		{  // edge case
			printf("%d: %d >= %d\n", args.t_n, jumps_left, args.t_n);
			args.mids[index2 - *args.n_t] = (args.input[index2 - *args.n_t] + args.input[index2 - *args.n_t - jump]);fflush(stdout);
			printf("settings mid[%d] = input[%d] (%d) + input[%d] (%d)\n", index2 - *args.n_t, index2 - *args.n_t, args.input[index2 - *args.n_t], index2 -*args.n_t - jump, args.input[index2 - *args.n_t - jump]); fflush(stdout);
			printf("%d: Edge after: mid[%d] = %d\n", args.t_n, index2 - *args.n_t, args.mids[index2 - *args.n_t]); fflush(stdout);
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

		step++;
		jump = (1 << (step - 1));
		sleep(0.1);
	}
	for(int i = 0; i < *args.size; i++)
	{  // set values in prefix sum array
		if(args.sums[i] != args.input[i])
		{
			printf("%d sets sums[%d] to %d\n", args.t_n, i, args.input[i]); fflush(stdout);
			args.sums[i] = args.input[i];
		}
	}

	printf("%d: args values: %d, %d, %d, %p\n", args.t_n, *args.size, *args.n_t, args.t_n, args.t_r); fflush(stdout);

	return 0;
}

void production(int* input, int* mids, int* sums, int* size, int* n_t, int* t_r)
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
		initArgs(&args[i], input, mids, sums, size, n_t, (i + 1), t_r);
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

/*
	this program shits itself if n_t >= (0.5 * size)
*/
int main()
{
	int input[8] = {1, 1, 1, 1, 1, 1, 1, 1};  // input array
	int size = 8;  // size of input
	int n_t = 4;  // number of threads
	int cond = 0;  // conditional variable for threads
	int* mids = (int*) malloc(size * sizeof(int));
	int* sums = (int*) malloc(size * sizeof(int));
	for(int i = 0; i < size; i++)
	{
		mids[i] = 0;
		sums[i] = 0;
	}
	memcpy(mids, input, (size * sizeof(int)));
	int t_r = 0;  // counter for threads ready

	production(input, mids, sums, &size, &n_t, &t_r);

	return 0;
}
