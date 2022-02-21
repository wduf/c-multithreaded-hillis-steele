#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>

//////

#define MAX_LINE_SIZE 256

//////

typedef struct __lock_t
{
	int ticket;
	int turn;
} lock_t;

//////

lock_t thread_lock;

//////

// starter
void read_input_vector(const char* filename, int n, int* array)
{
	FILE* fp;
	char* line = malloc(MAX_LINE_SIZE + 1);
	size_t len = MAX_LINE_SIZE;
	ssize_t read;
	int index = 0;

	fp = (strcmp(filename, "-") ? fopen(filename, "r") : stdin);

	assert((fp != NULL) && (line != NULL));

	while((read = getline(&line, &len, fp)) != -1)
	{
		array[index] = atoi(line);
		index++;
	}

	free(line);
	fclose(fp);
}

// slides 9
int FetchAndAdd(int* ptr)
{  // returns value at ptr, then increments it
	int old = *ptr;
	*ptr = old + 1;
	return old;
}

// slides 9
void lock_init(lock_t* thread_lock)
{  // initializes a spinlock
	thread_lock->ticket = 0;
	thread_lock->turn = 0;
}

// slides 9
void lock(lock_t* thread_lock)
{  // locks a lock
	int myturn = FetchAndAdd(&thread_lock->ticket);
	while (thread_lock->turn != myturn);  //spin
}

// slides 9
void unlock(lock_t* thread_lock)
{  // unlocks a lock
	thread_lock->turn += 1;
}

/*
	WORK IN PROGRESS
	- needs to contain locks
	
	NEED TO CHANGE UP ENTIRE SYSTEM:
	PTHREAD_CREATE TAKES (VOID*) FCN(VOID*)
*/
int carlBestBrawler(pthread_t* threads, int thread_number, int position, int size)  // position is starting position in array
{  // calculates and prints prefix sums
	int R = 1;  // default more values in array, set to 0 once end of array is reached
	int sum = 0;  // accumulator for prefix sum

	if((position + thread_number) > size)
	{  // if all prefix sums calculated
		R = 0;  // end of array reached
	} else
	{  // still prefix sums to be calculated
		for(int i = 0; i < thread_number; i++)
		{
			sum += threads[i];
		}
		printf("%d\n", sum);
	}
	
	return R;
}

// slides 9
int main(int argc, char* argv[])
{
	char* filename = argv[1];
	int size = atoi(argv[2]);  // size of input vector
	int thread_count = atoi(argv[3]);  // number of threads

	if(size < 2)
		exit(EXIT_FAILURE);
	int* input= malloc(size * sizeof(int));
	read_input_vector(filename, size, input);
	
	// end of starter code
	
	pthread_t threads[thread_count];  // array of threads, stack faster than heap
	int position = 0;  // position in array accumulator
	int keep_going;  // whether or not to do another loop of carlBestBrawler
		
	if(thread_count >= size)
	{  // if there are enough threads to go through entire vector without needing to reuse threads
		for(int i = 0; i < size; i++)
		{
			// i is the thread number
			keep_going = pthread_create(&threads[i], NULL, carlBestBrawler, threads, i, 0, size);
			// might need to implement a pthread_join(&threads[i]) here to make sure they're running in order, but this is slow
		}	
	} else
	{  // we need to reuse threads, need a barrier
		while(keep_going)
		{
			for(int i = 0; i < thread_count; i++)
			{
				keep_going = pthread_create(&threads[i], NULL, carlBestBrawler, threads, i, position, size);
				// might need to implement a pthread_join(&threads[i]) here to make sure they're running in order, but this is slow
				if(!keep_going)
				{
					break;  // reached end of array
				}
			}
			position += thread_count;  // next loop
		}
	}
	
	free(input);

	return 0;
}
