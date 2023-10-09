#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd,struct sockaddr *address,socklen_t length);

int initialisationServeur(short int *port,int connexions){
{
int s;
struct sockaddr_in adresse;
socklen_t taille=sizeof adresse;
int statut;

/* Creation d'une socket */
s=socket(PF_INET,SOCK_STREAM,0);
if(s<0){
        perror("initialisationServeur.socket");
        exit(-1);
        }

/* Specification de l'adresse de la socket */
adresse.sin_family=AF_INET;
adresse.sin_addr.s_addr=INADDR_ANY;
adresse.sin_port=htons(*port);
statut=bind(s,(struct sockaddr *)&adresse,sizeof(adresse));
if(statut<0) return -1;

/* On recupere le numero du port utilise */
statut=getsockname(s,(struct sockaddr *)&adresse,&taille);
if(statut<0){
        perror("initialisationServeur.getsockname");
        exit(-1);
        }
*port=ntohs(adresse.sin_port);

/* Taille de la queue d'attente */
statut=listen(s,connexions);
if(statut<0) return -1;

return s;
}
