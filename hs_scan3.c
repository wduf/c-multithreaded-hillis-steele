// INCLUDES:

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// DEFINES:

#define MAX_LINE_SIZE 256

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
		jumps_left = (*args.size - jump);
		index = ((*args.size - 1) - (args.t_n - 1));
		while(jumps_left >= *args.n_t)
		{  // if this thread is responsible for another jump
			args.mids[index] = (args.input[index] + args.input[index - jump]);
			jumps_left -= *args.n_t;
			index -= *args.n_t;
		}
		if(jumps_left >= args.t_n)
		{  // edge case
			args.mids[index] = (args.input[index] + args.input[index - jump]);
		}
		pthread_mutex_lock(&lock);
		*args.t_r += 1;
		pthread_mutex_unlock(&lock);
		while(*args.t_r < (step * *args.n_t));  // barrier
		memcpy(args.input, args.mids, (*args.size * sizeof(int)));
		pthread_mutex_lock(&lock);
		*args.t_r2 += 1;
		pthread_mutex_unlock(&lock);
		while(*args.t_r2 < (step * *args.n_t));  // barrier 2
		step++;
		jump = (1 << (step - 1));
	}
	for(int i = 0; i < *args.size; i++)
	{  // set values in prefix sum array
		if(args.sums[i] != args.input[i])
		{
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

	for(int i = 0; i < *n_t; i++)
	{  // create threads
		initArgs(&args[i], input, mids, sums, size, n_t, (i + 1), t_r, t_r2);
		pthread_create(&threads[i], NULL, threadFunction, (void*) &args[i]);
	}
	for(int i = 0; i < *n_t; i++)
	{  // wait for threads to finish executing
		pthread_join(threads[i], NULL);
	}
	printSums(input, *size);  // maybe make it so we print as we go along

	return;
}

void read_input_vector(const char* filename, int n, int* array)
{
	FILE *fp;
	char *line = malloc(MAX_LINE_SIZE+1);
	size_t len = MAX_LINE_SIZE;
	ssize_t read;

	fp = strcmp(filename, "-") ? fopen(filename, "r") : stdin;

	assert(fp != NULL && line != NULL);

	int index = 0;

	while ((read = getline(&line, &len, fp)) != -1)
	{
		array[index] = atoi(line);
		index++;
	}

	free(line);
	fclose(fp);
}

/*
	this program shits itself if n_t >= (0.5 * size)
*/
int main(int argc, char** argv)
{
	// starter code
	
	char* filename = argv[1];
	int size = atoi(argv[2]);
	int n_t = atoi(argv[3]);

	if(size < 2)
	{
		exit(EXIT_FAILURE);
	}

	int* input= malloc(size * sizeof(int));

	read_input_vector(filename, size, input);

	
	// end of starter code
	
	int* mids = (int*) malloc(size * sizeof(int));
	int* sums = (int*) malloc(size * sizeof(int));
	for(int i = 0; i < size; i++)
	{
		mids[i] = 0;
		sums[i] = 0;
	}
	memcpy(mids, input, (size * sizeof(int)));
		
	int t_r = 0;  // counter for threads ready
	int t_r2 = 0;  // for second barrier

	production(input, mids, sums, &size, &n_t, &t_r, &t_r2);

	return 0;
}
