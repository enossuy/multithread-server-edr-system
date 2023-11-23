
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
#include <getopt.h>
#include "./options.h"


int main(int argc,char *argv[]){


    char *interface;

    short int port = getPort(argc,argv,interface); //service = port, char *service ?



    printf("Ports = %d");

    return 0;
}
