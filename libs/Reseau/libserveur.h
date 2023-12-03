
#ifndef __SERVEUR_H__
#define __SERVEUR_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <netdb.h>

int initialisationServeur(short int *port,int connexions);
int boucleServeur(int ecoute,int (*traitement)(int));

#endif