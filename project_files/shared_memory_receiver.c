// shared_memory_receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "shared_data.h"
// shared_memory_receiver.c
#define SHARED_MEMORY_KEY 1234

// shared_memory_receiver.c
int main() {
    int shmid;
    shared_data_t *shared_data;

    // Get the shared memory segment created by the first Raspberry Pi
    shmid = shmget(SHARED_MEMORY_KEY, sizeof(shared_data_t), 0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to our address space
    shared_data = (shared_data_t *)shmat(shmid, NULL, 0);
    if (shared_data == (shared_data_t *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Print the received shared data
    printf("Received message: %s\n", shared_data->message);

    // Detach the shared memory segment
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}

