#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5 // Number of processes
#define K 2 // Number of resources each process needs
#define M 10 // Number of requests each process makes

int want[N][K] = {0}; // Set of requests for each process
int allowed[N] = {0}; // Set of processes allowed to enter critical section
int done = 0; // Number of processes that have completed

void* process(void* id_ptr) {
    int id = *(int*) id_ptr;
    int i, j;

    for (i = 0; i < M; i++) {
        // Make K resource requests
        for (j = 0; j < K; j++) {
            want[id][j] = 1;
        }

        // Wait until process is allowed to enter critical section
        while (!allowed[id]);

        // Enter critical section
        printf("Process %d is in critical section\n", id);
        sleep(1);
        printf("Process %d is leaving critical section\n", id);

        // Clear K resource requests
        for (j = 0; j < K; j++) {
            want[id][j] = 0;
        }

        // Notify other processes that current process has completed
        done++;
        if (done == N) {
            // Reset allowed array
            for (j = 0; j < N; j++) {
                allowed[j] = 0;
            }
            done = 0;
        }
    }
    return NULL;
}

int main() {
    int i, j;
    pthread_t thread[N];
    int id[N];

    // Initialize thread IDs
    for (i = 0; i < N; i++) {
        id[i] = i;
    }

    // Create threads
    for (i = 0; i < N; i++) {
        pthread_create(&thread[i], NULL, process, &id[i]);
    }

    // Run Maekawa algorithm
    while (1) {
        for (i = 0; i < N; i++) {
            int can_enter = 1;
            for (j = 0; j < N; j++) {
                if (i != j && want[j][0] && want[j][1]) {
                    can_enter = 0;
                    break;
                }
            }
            allowed[i] = can_enter;
        }
        sleep(1);
    }

    // Join threads
    for (i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
}
