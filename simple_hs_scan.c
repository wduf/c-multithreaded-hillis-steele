/*
	optimizations:
	- small things like declaring same type variables on one line
	- removing comments
	- code paragraph
	- printing as we go (maybe? runtime ends when main returns)
	- make while loops into weird for loops
	- only save the values from each loop to sums (not the entire input array), THIS WOULD BE HUGE
*/

// PICTURE OF TA GUIN

/*⠀
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠋⠉⠈⠉⠉⠉⠉⠛⠻⢿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣿⣿
⣿⣿⣿⣿⡏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿
⣿⣿⣿⢏⣴⣿⣷⠀⠀⠀⠀⠀⢾⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿
⣿⣿⣟⣾⣿⡟⠁⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣷⢢⠀⠀⠀⠀⠀⠀⠀⢸⣿
⣿⣿⣿⣿⣟⠀⡴⠄⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⣿
⣿⣿⣿⠟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⢴⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⣿
⣿⣁⡀⠀⠀⢰⢠⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⡄⠀⣴⣶⣿⡄⣿
⣿⡋⠀⠀⠀⠎⢸⣿⡆⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠗⢘⣿⣟⠛⠿⣼
⣿⣿⠋⢀⡌⢰⣿⡿⢿⡀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣧⢀⣼
⣿⣿⣷⢻⠄⠘⠛⠋⠛⠃⠀⠀⠀⠀⠀⢿⣧⠈⠉⠙⠛⠋⠀⠀⠀⣿⣿⣿⣿⣿
⣿⣿⣧⠀⠈⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠟⠀⠀⠀⠀⢀⢃⠀⠀⢸⣿⣿⣿⣿
⣿⣿⡿⠀⠴⢗⣠⣤⣴⡶⠶⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡸⠀⣿⣿⣿⣿
⣿⣿⣿⡀⢠⣾⣿⠏⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⠀⣿⣿⣿⣿
⣿⣿⣿⣧⠈⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿
⣿⣿⣿⣿⡄⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣦⣄⣀⣀⣀⣀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠙⣿⣿⡟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠁⠀⠀⠹⣿⠃⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢐⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⠿⠛⠉⠉⠁⠀⢻⣿⡇⠀⠀⠀⠀⠀⠀⢀⠈⣿⣿⡿⠉⠛⠛⠛⠉⠉
⣿⡿⠋⠁⠀⠀⢀⣀⣠⡴⣸⣿⣇⡄⠀⠀⠀⠀⢀⡿⠄⠙⠛⠀⣀⣠⣤⣤⠄⠀
*/

// INCLUDES:

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>

// DEFINES:

#define MAX_LINE_SIZE 256

// GLOBALS:

pthread_mutex_t lock;  // mutually exclusive lock
pthread_cond_t cond;  // conditional variable
static int size_g;  // global size
static int nt_g;  // global number of threads
int* input_g;  // global input array
int* mids_g;  // global middle sums array
int* sums_g;  // global prefix sums array
int tr_g = 0;  // counter for thready ready
int set_g = 0;  // flag for setting input_g to mid_g

// FUNCTIONS

void printSums(int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d\n", sums_g[i]);
	}
}

void barrier()
{
	pthread_mutex_lock(&lock);
	tr_g++;
	if(tr_g < nt_g)
	{  // if not all threads ready
		pthread_cond_wait(&cond, &lock);  // waiting
	} else
	{
		tr_g = 0;
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&lock);

	return;
}

void copyMids(int step)
{
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
	int tn = *((long*) arg);  // thread number
	int step = 1;  // step counter
	int jump = 1;  // jump size, 2^0 = 1
	int jumps_left;  // variable to decrease repetition
	int index;  // variable to decrease repetition

	while(jump < size_g)
	{
		jumps_left = (size_g - jump);
		index = ((size_g - 1) - (tn - 1));
		while(jumps_left >= nt_g)
		{  // if this thread is responsible for another jump
			mids_g[index] = (input_g[index] + input_g[index - jump]);
			jumps_left -= nt_g;
			index -= nt_g;
		}
		if(jumps_left >= tn)
		{  // edge case
			mids_g[index] = (input_g[index] + input_g[index - jump]);
		}
		barrier();  // barrier 1
		copyMids(step);
		//memcpy(input_g, mids_g, (size_g * sizeof(int)));
		barrier();  // barrier 2
		step++;
		jump = (1 << (step - 1));
	}
	// save values to sums_g
	for(int i = 0; i < size_g; i++)
	{  // set values in prefix sum array
		sums_g[i] = input_g[i];
	}

	return 0;
}

void production(int* input, int size, int nt)
{
	pthread_t* threads = (pthread_t*) malloc(nt * sizeof (pthread_t));  // array of threads
	int tns[nt];  // array of thread numbers/ids

	for(int i = 0; i < nt; i++)
	{  // create threads
		tns[i] = (i + 1);
		pthread_create(&threads[i], NULL, threadFunction, (void*) &tns[i]);
		//printf("%d: created\n", tns[i]);
	}
	for(int i = 0; i < nt; i++)
	{  // wait for threads to finish executing
		pthread_join(threads[i], NULL);
	}
	printSums(size);

	free(threads);

	return;
}

int main()
{
	static int size = 200;  // size of input array
	static int nt = 4;  // number of threads

	// initialize arrays
	int array_size = (size * sizeof(int));  // variable to decrease repetition
	int* input = (int*) malloc(array_size);
	int* mids = (int*) malloc(array_size);
	int* sums = (int*) malloc(array_size);

	// set array values
	for(int i = 0; i < size; i++)
	{  // set all input values to 1, allocate space for mids and sums
		input[i] = 1;
	}
	memcpy(mids, input, array_size);
	memcpy(sums, input, array_size);

	// set globals
	size_g = size;
	nt_g = nt;
	input_g = input;
	mids_g = mids;
	sums_g = sums;

	// init vars
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&cond, NULL);
	
	production(input, size, nt);

	// free heap
	free(input);
	free(mids);
	free(sums);

	return 0;
}
