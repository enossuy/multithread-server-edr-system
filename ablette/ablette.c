#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <getopt.h>
#include <pcap.h>
#include "options.h"
#include <time.h>
#include <stdbool.h>
#include "../libs/Reseau/libserveur.h"

#define MAX_SRC 100
 
struct source {
    char adresse[IFNAMSIZ];
    int packet_cpt;
};

struct source sources[MAX_SRC];
int cpt_source = 0;

int compare(const void *a, const void *b) {
  
    struct source *sourceA = (struct source *)a;
    struct source *sourceB = (struct source *)b;
  
    return (sourceB->packet_cpt - sourceA->packet_cpt); // si on inverse ca fait decroissant
}

void sort_src(char *source_ip)
{
    bool already_in = false;
    int index;
    for(int i = 0 ; i < cpt_source; i++)
    {
        if(strcmp(sources[i].adresse,source_ip) == 0){
            already_in = true;
            index = i; 
            break;   
        }
    }

    if(!already_in){ 
        strcpy(sources[cpt_source].adresse,source_ip);
        sources[cpt_source].packet_cpt = 1;
        cpt_source++;
    }else{
        sources[index].packet_cpt++;
    }

    qsort(sources, cpt_source, sizeof(struct source), compare);
}

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    printf("Paquet capturé. Taille : %d octets", pkthdr->len);
    struct ether_header *eth_header;
    struct ip *ip_header;

    eth_header = (struct ether_header *)packet;

    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) { // protocole IP
        ip_header = (struct ip *)(packet + sizeof(struct ether_header));
        char source_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
        printf("| Date : %s ",ctime((const time_t*)&pkthdr->ts.tv_sec));
        printf("| IP source : %s\n", source_ip);
        sort_src(source_ip);
    }
    for(int i = 0 ; i<cpt_source; i++)
    {
        if(i<5)
            printf("num : %d ip: %s cpt :%d \n",i+1,sources[i].adresse,sources[i].packet_cpt);
    }
    printf("================================\n\n");


    
    
}

int main(int argc, char *argv[]) {
    char interface[20] = ""; 
    int ports[MAX_PORT];

    int size_port = parseArguments(argc, argv, interface, ports);

    int fd;
    struct ifreq ifr;

    /* ===== Recuperation adresse IPv4 de l'interface ===== */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET; //IPv4
    strncpy(ifr.ifr_name, interface, IFNAMSIZ-1); // interface
    ioctl(fd, SIOCGIFADDR, &ifr);// recuperation IP 
    close(fd);
    char ad_IP[IFNAMSIZ]; 
    strncpy(ad_IP,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr),IFNAMSIZ-1);//conversion binaire vers str
    printf("%s\n", ad_IP);


    /*Affichage, test parsing + IP*/
    printf("interface : %s, adresse IP : %s\n", interface,ad_IP);
    printf("Ports :\n"); for (int i = 0; i < size_port; i++) printf("%d\n", ports[i]);

    char errbuff[PCAP_ERRBUF_SIZE]; // pour y mettre erreurs

    /*
    - 1 ==> toutes les trames sont capturées pas que celles pour le pc
    - -1 ==> pour capturer en continu
    BUFSIZ=4096, taille
    */

    pcap_t *handler = pcap_open_live(interface,BUFSIZ,1,-1,errbuff);
    if(handler == NULL){ 
        perror("Erreur ouverture interface");
        exit(-1);
    }

    struct bpf_program fp;
   

    /*=== Construction de l'expression pour le filtre ===*///"dst host 192.168.17.114 and (port 80 or port 443)"
    char expression[100] = "dst host ";
    strcat(expression,ad_IP);
    char text[15];

    char expression2[100] = " and (";
    for (int i = 0; i < size_port; i++) {
        if(i+1 == size_port) {
            sprintf(text, "port %d)", ports[i]);
            strcat(expression2,text);
        }
        else {
        sprintf(text, "port %d or ", ports[i]);
        strcat(expression2,text);
        }
    }

    strcat(expression,expression2);
    printf("expression : %se\n", expression);
    //char eexpression[] = "dst host 192.168.0.16 and (port 80 or port 443)";

    if (pcap_compile(handler, &fp, expression, 0, PCAP_NETMASK_UNKNOWN) == -1) {
       perror("Erreur compilation filtre\n");
        pcap_close(handler);
        exit(-1);
    }

    if (pcap_setfilter(handler, &fp) == -1) {
        perror("Erreur set filtre\n");
        pcap_close(handler);
        exit(-1);
    }

    printf("Capture sur l'interface %s en cours\n", interface);

    if (pcap_loop(handler, -1, packet_handler, NULL) != 0) {
        perror("Erreur capture paquets\n");
        pcap_close(handler);
        exit(-1);
    }

    pcap_close(handler);
    return 0;
}

