#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libpartage.h"
#define IP_LEN 16
int read_p(char tab[TOP][16]) {
    
    key_t key = ftok("/tmp", 'B');

    int shmid = shmget(key, TOP * 16, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    char (*mem)[16] = shmat(shmid, NULL, 0);
    if ((void*)mem == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    /*recuperation*/
    for (int i = 0; i < TOP; i++) {
        strncpy(tab[i], mem[i], IP_LEN - 1);
        tab[i][IP_LEN - 1] = '\0';
    }

    if (shmdt(mem) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

void free_shm(int id)
{
    // liberation
    shmctl(id, IPC_RMID, NULL);
}


int write_p(char tab[TOP][16]) {
    
    key_t key = ftok("/tmp", 'B');

    int shmid = shmget(key, TOP * 16, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    char (*mem)[16] = shmat(shmid, NULL, 0);
    if ((void*)mem == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    /*copie dans la memoire*/
    for (int i = 0; i < TOP; i++) { 
        strncpy(mem[i], tab[i], IP_LEN - 1);
        mem[i][IP_LEN - 1] = '\0';
    }

    if (shmdt(mem) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }

    return shmid;
}

