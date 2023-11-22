#include "../libs/Reseau/libserveur.h"
#include "./analyste_http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *gestionClientThread(void *arg)
{
    int *s = (int*)arg;   
    gestionClient(*s);
    free(s);
    return NULL;
}




