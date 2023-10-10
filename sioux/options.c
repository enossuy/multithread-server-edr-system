
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

#define MAX_CONNEXIONS 5



void err_syntax(void)
{
    printf("Syntaxe incorrecte : -p <port> ou --port <port>\n");
}

int parsing(int valopt, char *optarg)
{
    int port_num = -1;

    switch (valopt)
    {
        case 0:
        case 'p':

            if (sscanf(optarg, "%d", &port_num) == 1)
                printf("Port : %d \n", port_num);
            else
                err_syntax();
            break;

        case '?':
            err_syntax();
            break;
    }

    return port_num;
}

int analyseArguments(int argc,char *argv[]){
    static struct option long_options[] = {
            {"port",     required_argument, 0,  'p' },
            {0,     0, 0,  0 }
            };

    int option_index = 0;
    int val_opt;
    int port_num;
    int i = 0;

    while ((val_opt = getopt_long(argc, argv, "p:0",long_options, &option_index)) != -1) // si suivi de ":" ==> besoin d argument
        {
            port_num = parsing(val_opt,optarg);
            i = 1;
        }

        if(i == 0){
            printf("Port non spécifié\n");
            exit(-1);
        }

    return port_num;
}




