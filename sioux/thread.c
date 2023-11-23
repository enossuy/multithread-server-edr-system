#include "../libs/Reseau/libserveur.h"
#include "./analyste_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "thread.h"

int gestionClientThread(int s)
{
    pthread_t t;
    int *socket = (int*) malloc(sizeof(int));
    *socket = s;
    pthread_create(&t, NULL, threadClient, (void*) socket);//free dedans
    pthread_detach(t);
    return 0;
}




void *threadClient(void* arg){
    gestionClient(*(int*)arg);
    free(arg);
}
