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
#include <getopt.h>
#include "libserveur.h"


#define MAX_LIGNE 100


#define WEB_DIR  "./www" 
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK  200
#define CODE_NOTFOUND 404

int initialisationServeur(short int *port,int connexions){
struct addrinfo precisions,*resultat,*origine;
int statut;
int s;

/*RE- reconversion ???*/
char temp[20]; 
snprintf(temp, sizeof(temp), "%d", *port);//renvoie nb caracteres 
char *service = temp;

/* Construction de la structure adresse */
memset(&precisions,0,sizeof precisions);
precisions.ai_family=AF_UNSPEC;
precisions.ai_socktype=SOCK_STREAM;
precisions.ai_flags=AI_PASSIVE;
statut=getaddrinfo(NULL,service,&precisions,&origine);
if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
struct addrinfo *p;
for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
  if(p->ai_family==AF_INET6){ resultat=p; break; }

/* Creation d'une socket */
s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }

/* Options utiles */
int vrai=1;
if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
  perror("initialisationServeur.setsockopt (REUSEADDR)");
  exit(EXIT_FAILURE);
  }
if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
  perror("initialisationServeur.setsockopt (NODELAY)");
  exit(EXIT_FAILURE);
  }

/* Specification de l'adresse de la socket */
statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
if(statut<0){ freeaddrinfo(origine); shutdown(s,SHUT_RDWR); return -1; }

/* Liberation de la structure d'informations */
freeaddrinfo(origine);

/* Taille de la queue d'attente */
statut=listen(s,connexions);
if(statut<0){ shutdown(s,SHUT_RDWR); return -2; }

return s;
}

int boucleServeur(int ecoute,int (*traitement)(int)){
int dialogue;
while(1){

    /* Attente d'une connexion */
    if((dialogue=accept(ecoute,NULL,NULL))<0) return -1;
    

    printf("\033[93mClient connecté\033[0m\r\n");
    /* Passage de la socket de dialogue a la fonction de traitement */
    if(traitement(dialogue)<0){ shutdown(ecoute,SHUT_RDWR); return 0;}
    }
}

int gestionClient(int s){

FILE *dialogue=fdopen(s,"a+");
  char cmd[MAX_BUFFER];
  char page[MAX_BUFFER];
  char proto[MAX_BUFFER];
  char path[2048];
  char type[MAX_BUFFER];
if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }

char ligne[MAX_LIGNE];

while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
{
  fprintf(dialogue,"> %s",ligne);
  if(sscanf(ligne,"%s %s %s",cmd,page,proto)!=3) exit(-1);
  fprintf(dialogue,"> cmd :%s page :%s proto:%s\n",cmd, page,proto);

  /*On cherche si il ya "?""*/
  char *page_arg = strtok(page, "?"); 

    if (page_arg != NULL) {

        fprintf(dialogue,"page extraite : %s\n", page);
        
        char *argument = strtok(NULL, "?");
        if (argument != NULL) {
            fprintf(dialogue,"argument dans la requete: %s\n", argument);
        } else {
            fprintf(dialogue,"pas d'argument dans la requete\n");
        }
    }


  if(strcasecmp(cmd,"GET")==0){
  int code=CODE_OK;
  struct stat fstat;
  sprintf(path,"%s%s",WEB_DIR,page_arg);
  fprintf(dialogue,"> path : %s\n",path);

  if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode)){
    sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
    fprintf(dialogue,"page non trouvée\n");
    code=CODE_NOTFOUND;
    }
  else{
    fprintf(dialogue,"page trouvée\n");

    strcpy(type,"text/html");
    char *end=page+strlen(page);
    if(strcmp(end-4,".png")==0) strcpy(type,"image/png");
    if(strcmp(end-4,".jpg")==0) strcpy(type,"image/jpg");
    if(strcmp(end-4,".gif")==0) strcpy(type,"image/gif");
    fprintf(stdout,"HTTP/1.0 %d\r\n",code);
    fprintf(stdout,"Server: CWeb\r\n");
    fprintf(stdout,"Content-type: %s\r\n",type);
    fprintf(stdout,"Content-length: %ld\r\n",fstat.st_size);
    fprintf(stdout,"\r\n");
    fflush(stdout);    



    }
  }
}
  
/* Termine la connexion */
fclose(dialogue);
return 0;
}

void err_syntax(void)
{
    printf("Syntaxe incorecte : -p <port> ou --port <port>\n");
}

int parsing(int valopt, char *optarg)
{
    int port_num = -1;

    switch (valopt)
    {
        case 0:
        case 'p':

            if (sscanf(optarg, "%d", &port_num) == 1) 
                printf("Port : %d ", port_num);
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

    if(i == 0)
    {
      printf("Port non spécifié\n");
      exit(-1);
    }

    return port_num;
}


