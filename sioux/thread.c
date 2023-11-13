#include "../libs/Reseau/libserveur.h"
#include "./analyste_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <pthread.h>


#define MAX_THREAD 1000

int boucleServeur(int ecoute){
    int dialogue;
    pthread_t tid_threads;

    while(1){
    	 
        /* Attente d'une connexion */
        if((dialogue=accept(ecoute,NULL,NULL))<0) return -1;//mettre free, variable differente voir page 35 cours thread

        printf("\033[93mClient connecté\033[0m\r\n");
        /* Passage de la socket de dialogue a la fonction de traitement */
        pthread_create(&tid_threads,NULL,gestionClient,&dialogue);
        pthread_detach(tid_threads);
        if(dialogue<0){ shutdown(ecoute,SHUT_RDWR); return 0;}

    }
}



