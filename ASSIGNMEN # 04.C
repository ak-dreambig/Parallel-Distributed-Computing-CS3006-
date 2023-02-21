#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_THREADS 5
bool choosing[NUM_THREADS];
int number[NUM_THREADS];

void lock(int thread_num) {
    choosing[thread_num] = true;

    int max_num = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (number[i] > max_num) {
            max_num = number[i];
        }
    }
    number[thread_num] = max_num + 1;

    choosing[thread_num] = false;

    for (int j = 0; j < NUM_THREADS; j++) {
        if (j != thread_num) {
            while (choosing[j]) {};
            while (number[j] != 0 && (number[j] < number[thread_num] ||
                                      (number[j] == number[thread_num] && j < thread_num))) {};
        }
    }
}

void unlock(int thread_num) {
    number[thread_num] = 0;
}

void *thread_func(void *thread_id) {
    int thread_num = *(int *)thread_id;
    printf("Thread %d is starting\n", thread_num);

    lock(thread_num);
    printf("Thread %d is in critical section\n", thread_num);
    unlock(thread_num);

    printf("Thread %d is ending\n", thread_num);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        int rc = pthread_create(&threads[i], NULL, thread_func, &thread_ids[i]);
        if (rc) {
            printf("Error: pthread_create() returned %d\n", rc);
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}