// shared_memory_creator.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "shared_data.h"
// shared_memory_creator.c
#define IP_ADDRESS "192.168.1.160"
#define SHARED_MEMORY_KEY 1234

// shared_memory_creator.c
int main() {
    int shmid;
    shared_data_t *shared_data;

    // Create the shared memory segment
    shmid = shmget(SHARED_MEMORY_KEY, sizeof(shared_data_t), IPC_CREAT | S_IRUSR | S_IWUSR);
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

    // Initialize the shared data
    strncpy(shared_data->message, "Hello from 192.168.0.127", MAX_LENGTH);

    // Connect to the second Raspberry Pi
    int sockfd;
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    if (inet_pton(AF_INET, IP_ADDRESS, &(server_addr.sin_addr)) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    // Send the shared data
    if (send(sockfd, shared_data, sizeof(shared_data_t), 0) == -1) {
        perror("send");
        exit(1);
    }

    // Detach the shared memory segment
    if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    printf("Shared memory segment created and data sent.\n");

    return 0;
}

