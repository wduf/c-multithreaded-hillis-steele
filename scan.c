// includes:

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// globals:

int* input_g;  // global input array
int* sums_g;  // global sum array
int pos_g;  // global current position in array

// functions:

void* threadFunction(void* par)
{
	int tn = *((long*) par);
	int sum = 0;

	for(int i = 0; i < (pos_g + tn); i++)
	{  // calculate prefix sum
		sum += input_g[i];
	}
	sums_g[pos_g + tn - 1] = sum;  // tn starts at 1

        return 0;
}

void printSums(int loops)
{  // print out prefix sums for (loops) sums
	for(int i = 0; i < loops; i++)
	{  // print out all values
		printf("%d\n", sums_g[i]);
	}
}

/*
        input = input array
        size = size of input array
        nt = number of threads
*/
void production(int* input, int size, int nt)
{
        pthread_t threads[size];  // place to store threads
	int tn[size];  // place to store thread numbers (to pass into pthread_create)
	int sums[size];
	int end = 0;  // if end of input reached
	int curr_threads = 0;  // counter for current threads running
	input_g = input;  // make input global
	sums_g = sums;  // make sums global
	pos_g = 0;
	
	if(nt >= size)
	{  // if we don't need to reuse threads
		for(int i = 0; i < size; i++)
		{  // create (nt) threads
			tn[i] = (i + 1);
			pthread_create(&threads[i], NULL, threadFunction, (void*) &tn[i]);  // (i + 1) is the thread number (in order of creation)
			curr_threads++;
		}
		for(int i = 0; i < curr_threads; i++)
		{  // don't stop until all threads are done executing
			pthread_join(threads[i], NULL);
			curr_threads--;
		}
	} else
	{  // if we need to reuse threads
		while(!end)
		{
			for(int i = 0; i < nt; i++)
			{  // create (nt) threads
				tn[i] = (i + 1);
				if((pos_g + tn[i]) > size)
				{
					end = 1;  // reached end of array
					break;
				}
				pthread_create(&threads[i], NULL, threadFunction, (void*) &tn[i]);  // (i + 1) is the thread number (in order of creation)
				curr_threads++;
			}
			for(int i = 0; i < curr_threads; i++)
			{  // don't stop until all threads are done executing
				pthread_join(threads[i], NULL);
				curr_threads--;
			}
			pos_g += nt;  // start 1 after the last thread, reuse threads
		}
	}
	printSums(size);
}

int main()
{
        int input[32] = {793, 283, 735, 755, 637, 266, 826, 175, 107, 651, 466, 949, 754, 60, 52, 948, 3, 263, 592, 511, 316, 784, 45, 139, 398, 545, 976, 394, 38, 403, 689, 182};
        int size = 32;
        int nt = 4;

        production(input, size, nt);

        return 0;
}
