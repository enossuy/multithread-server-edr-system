#include "../libs/Reseau/libserveur.h"
#include "./analyste_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>



void MultiThread(int nb_connexions, int socket){

    pthread_t tid_threads[nb_connexions];

    for(int i = 0; i < nb_connexions; i++)
    {
        int status = pthread_create(&tid_threads[i],NULL,boucleServeur,&socket);
        if(status != 0) 
        {
                perror("Erreur Thread");
        }
        else
        {
                printf("Thread numero %d\n",i+1);
        }
    }
/*sans le join le programme se termine direct*/
    for (int i = 0; i < nb_connexions; i++) {
        pthread_join(tid_threads[i], NULL) ; // mettre test status !=0
    }
} 





