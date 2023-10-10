#include "../libs/Reseau/libserveur.h"
#include "./options.h"
#include "./analyste_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_CONNEXIONS 5



int main(int argc,char *argv[]){

        int s;

        /* Lecture des arguments de la commande */
        short int port = analyseArguments(argc,argv); //service = port, char *service ?

        /* Initialisation du serveur */
        s=initialisationServeur(&port,MAX_CONNEXIONS);
        if(s<0){
                fprintf(stderr,"Port non utilisable\n");
                exit(EXIT_FAILURE);
        }
        else
                printf("Port libre !\n");
   
/* Lancement de la boucle d'ecoute */
        boucleServeur(s,gestionClient);



return 0;
} 




