#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int sharedData = 0; // Shared data accessed by multiple threads

pthread_barrier_t barrier; // Barrier to synchronize threads
pthread_mutex_t mutex; // Mutex to protect sharedData

void* threadFunction(void* arg) {
    int threadID = *(int*)arg;

    // Perform some computation
    // ...

    // Acquire the lock to access sharedData
    pthread_mutex_lock(&mutex);
    sharedData += threadID; // Modify sharedData
    pthread_mutex_unlock(&mutex);

    // Memory barrier to ensure all previous operations are completed before proceeding
    __sync_synchronize(); // This is a GCC built-in function for a full memory barrier

    // Wait for all threads to reach this point before printing sharedData
    pthread_barrier_wait(&barrier);

    // All threads are synchronized at this point, print sharedData
    printf("Thread %d: Shared Data = %d\n", threadID, sharedData);

    return NULL;
}

int main() {
    pthread_t threads[3];
    int threadIDs[3] = {1, 2, 3};

    // Initialize barrier and mutex
    pthread_barrier_init(&barrier, NULL, 4); // 4 threads (3 worker threads + main thread)
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < 3; ++i) {
        pthread_create(&threads[i], NULL, threadFunction, &threadIDs[i]);
    }

    // Main thread also participates in the computation
    threadFunction(&threadIDs[3]);

    // Join threads
    for (int i = 0; i < 3; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Destroy barrier and mutex
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    return 0;
}
