https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
/////////////////////////////////////////////////////////////////
//
//  Running multiple threads using the pthreads library
//  Example that can be used to test performance on
//      single and multiple processors/cores
//
//  Copyright (c) 2015
//  Borna Noureddin
//  British Columbia Institute of Technology
//
/////////////////////////////////////////////////////////////////


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>

#define NUM_THREADS     5   // change this as needed


// Function to subtract one timeval struct from another
//  NB: returns 1 if the difference is negative, otherwise 0
int TimeSubtract(struct timeval *result,
                 struct timeval *t2,
                 struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) -
                    (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return diff < 0;
}

// Worker thread
// Just wastes time in a long empty loop
void *WorkerThread(void *argument)
{
    int passedInValue;
    struct timeval tvBegin, tvEnd, tvDiff;
    
    gettimeofday(&tvBegin, NULL);    // check time at start of thread
    
    passedInValue = *((int *) argument);
    printf("Hello World! It's me, thread with argument %d!\n", passedInValue);
    
    // waste some time (can replace with actual work here!)
    volatile unsigned long long i;
    for (i = 0; i < 5000000000ULL; ++i);
    
    gettimeofday(&tvEnd, NULL);  // check time at end of thread
    TimeSubtract(&tvDiff, &tvEnd, &tvBegin);
    printf("Finished thread with value %d (%ld.%06ld ms).\n",
           passedInValue, tvDiff.tv_sec, tvDiff.tv_usec);
    
    return NULL;
}

// main function
int main(void)
{
    pthread_t threads[NUM_THREADS];
    int threadArgs[NUM_THREADS];
    int resultCode, index;
    struct timeval tvBegin, tvEnd, tvDiff;
    
    // Get time it takes to run a single thread by itself
    gettimeofday(&tvBegin, NULL);
    int val = 1000;
    WorkerThread((void *)&val);
    gettimeofday(&tvEnd, NULL);
    TimeSubtract(&tvDiff, &tvEnd, &tvBegin);
    printf("Single thread: %ld.%06ld ms\n", tvDiff.tv_sec, tvDiff.tv_usec);
    
    gettimeofday(&tvBegin, NULL);    // start time for all threads...
    
    // Spawn threads
    for (index = 0; index < NUM_THREADS; ++index)
    {
        threadArgs[index] = index;
        printf("In main: creating thread %d\n", index);
        resultCode = pthread_create(&threads[index], NULL, WorkerThread, (void *) &threadArgs[index]);
        assert(0 == resultCode);
    }
    
    // Wait for each thread to complete
    for (index = 0; index < NUM_THREADS; ++index) {
        // block until thread 'index' completes
        resultCode = pthread_join(threads[index], NULL);
        printf("In main: thread %d has completed\n", index);
        assert(0 == resultCode);
    }

    // Report time for all threads to complete
    gettimeofday(&tvEnd, NULL);  // end time after all threads done
    TimeSubtract(&tvDiff, &tvEnd, &tvBegin);
    printf("In main: All threads completed successfully\n");
    printf("TOTAL TIME = %ld.%06ld ms\n", tvDiff.tv_sec, tvDiff.tv_usec);

    exit(EXIT_SUCCESS);
}

