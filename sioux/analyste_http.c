#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#define PORT 8080

void err_syntax(void)
{
    printf("Syntaxe incorecte : -p <port> ou --port <port>\n");
}

int parsing(int valopt, char *optarg)
{
    int port_num;
    switch (valopt)
    {
        case 0:
        case 'p':
            port_num = atoi(optarg);// faire sscanf pour tester
            printf("good, port : %d\n",port_num);
            return port_num;
            break;

        case '?':
            err_syntax();
            break;
    }
}

int main(int argc, char * argv[])
{
    static struct option long_options[] = {
        {"port",     required_argument, 0,  'p' },
        {0,     0, 0,  0 }
        };

    int option_index = 0;
    int val_opt;
    int port_num;

    while ((val_opt = getopt_long(argc, argv, "p:0",long_options, &option_index)) != -1) // si suivi de ":" ==> besoin d argument
           port_num = parsing(val_opt,optarg);

	return 0;
}

