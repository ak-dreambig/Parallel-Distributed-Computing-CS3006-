#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5 // Number of processes
#define M 10 // Number of requests each process makes

int want[N] = {0}; // Set of requests for each process
int reply[N] = {0}; // Set of replies received by each process
int done = 0; // Number of processes that have completed

void* process(void* id_ptr) {
    int id = *(int*) id_ptr;
    int i;

    for (i = 0; i < M; i++) {
        // Make request for critical section
        want[id] = 1;

        // Send request to other processes
        int j;
        for (j = 0; j < N; j++) {
            if (j != id) {
                reply[j] = 0;
                // Send request message to process j
                printf("Process %d sends request to process %d\n", id, j);
            }
        }

        // Wait until all replies have been received
        int count = 0;
        while (count < N - 1) {
            int j;
            for (j = 0; j < N; j++) {
                if (j != id && want[j] && !reply[j]) {
                    // Process j has not yet replied to request
                    break;
                }
            }
            if (j == N) {
                // All processes have replied to request
                break;
            }
            // Wait for 1 millisecond
            usleep(1000);
        }

        if (count == N - 1) {
            // All replies have been received
            // Enter critical section
            printf("Process %d is in critical section\n", id);
            sleep(1);
            printf("Process %d is leaving critical section\n", id);
            // Clear request for critical section
            want[id] = 0;
            // Send reply messages to all waiting processes
            int j;
            for (j = 0; j < N; j++) {
                if (j != id && reply[j]) {
                    reply[j] = 0;
                    // Send reply message to process j
                    printf("Process %d sends reply to process %d\n", id, j);
                }
            }
        } else {
            // One or more processes have not yet replied to request
            // Wait for 1 millisecond and try again
            usleep(1000);
        }
    }

    // Notify other processes that current process has completed
    done++;
    if (done == N) {
        // Reset reply array
        int j;
        for (j = 0; j < N; j++) {
            reply[j] = 0;
        }
        done = 0;
    }

    return NULL;
}

int main() {
    int i;
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

    // Join threads
    for (i = 0; i < N; i++) {
        pthread_join(thread[i], NULL);
    }

    return 0;
