
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

int macAddress[6];

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
        case 'i':
                interface = optarg;//les mettre en pointeur en parametre
                ports = optarg;
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

    while ((val_opt = getopt_long(argc, argv, "i:p:0",long_options, &option_index)) != -1) // si suivi de ":" ==> besoin d argument
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


void parseOptions(int argc,char **argv)
{
  int index;
  int c;

  /* Definition des options */

  static char *options="i:dsbu:";
  static const struct option longOptions[]={
    {"interface",1,0,'i'},
    {"port",required_argument, 0,'p'},
    {0,0,0,0}
    } ;

  /* Analyse lexicale */

  while((c=getopt_long(argc,argv,options,longOptions,&index))>0)
    switch(c){
      case 'i': ifname=optarg; break;
      case 'p': mode=MODE_DISPLAY; break;
      case 'u':{
        int x[ETHER_ADDR_LEN];
 int i,nb=sscanf(optarg,"%x:%x:%x:%x:%x:%x",x+0,x+1,x+2,x+3,x+4,x+5);
 if(nb!=ETHER_ADDR_LEN) usage(argv[0]);
 for(i=0;i<ETHER_ADDR_LEN;i++) macAddress[i]=x[i];
 unicast=1;
        break; }
      default: usage(argv[0]); break;
      }
  if(optind<argc) usage(argv[0]);
}
