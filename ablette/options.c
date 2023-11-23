
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "options.h"

#define MAX_PORTS 20 // Nombre maximum de ports dans la liste

int parseArguments(int argc, char *argv[], char *interface, int ports[]) {
    
    int cpt = 0;

    int opt;
    while ((opt = getopt(argc, argv, "i:p:")) != -1) {
        switch (opt) {
            case 'i':
                strncpy(interface, optarg, sizeof(interface));
                break;
            case 'p':
                {
                    char *token = strtok(optarg, ",");
                    while (token != NULL) {
                        if (cpt < MAX_PORTS) {
                            ports[cpt] = atoi(token);
                            token = strtok(NULL, ",");
                            cpt++;
                        } else {
                            printf("Trop de ports spécifiés ( le MAX est %d)\n", MAX_PORTS);
                            exit(-1);
                        }
                    }
                }
                break;
        }
    }

    /*si rien, mettre ports et interface par defaut*/
    if (strlen(interface) == 0 || cpt == 0) {
        strncpy(interface, "eth0", sizeof(interface));
        cpt = 2;
        ports[0] = 80;
        ports[1] = 443;
    }

    return cpt;
}













