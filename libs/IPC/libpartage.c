#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOP 10
#define IP_LEN 16
char read_p(char tab[10][16]) {
    // ftok to generate unique key
    key_t key = ftok("/tmp/top", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, TOP * 16, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // shmat to attach to shared memory
    char (*mem)[16] = shmat(shmid, NULL, 0);
    if ((void*)mem == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOP; i++) {
        // Check for null termination before printing
        if (mem[i][0] != '\0') {
            printf("%s\n", mem[i]);
        }
    }

    for (int i = 0; i < TOP; i++) {
        strncpy(tab[i], mem[i], IP_LEN - 1);
        tab[i][IP_LEN - 1] = '\0';  // Ensure null-terminated string
    }

    for (int i = 0; i < TOP; i++) {
        // Check for null termination before printing
        if (tab[i][0] != '\0') {
            printf("%s\n", tab[i]);
        }
    }


    // detach from shared memory
    if (shmdt(mem) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

void free_shm(int id)
{
    // destroy the shared memory
    shmctl(id, IPC_RMID, NULL);
}

int write_p(char tab[10][16]) {
    // ftok to generate unique key
    key_t key = ftok("/tmp/top", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, TOP * 16, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // shmat to attach to shared memory
    char (*mem)[16] = shmat(shmid, NULL, 0);
    if ((void*)mem == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOP; i++) {
        strncpy(mem[i], tab[i], IP_LEN - 1);
        mem[i][IP_LEN - 1] = '\0';  // Ensure null-terminated string
    }

    // detach from shared memory
    if (shmdt(mem) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

