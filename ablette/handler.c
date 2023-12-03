#include "handler.h"

struct source sources[MAX_SRC];
int cpt_source = 0;
char top[TOP][IFNAMSIZ];
int id_shm;

int compare(const void *a, const void *b) {
  
    struct source *sourceA = (struct source *)a;
    struct source *sourceB = (struct source *)b;
  
    return (sourceB->packet_cpt - sourceA->packet_cpt); // si on inverse ca fait decroissant
}

void sort_src(char *source_ip)
{
    bool already_in = false;
    int index;
    for(int i = 0 ; i < cpt_source; i++)// recherche de l adresse capturée dans le tableau
    {
        if(strcmp(sources[i].adresse,source_ip) == 0){
            already_in = true;
            index = i; 
            break;   
        }
    }

    if(!already_in){ 
        if(cpt_source < (MAX_SRC - 1)){
            strcpy(sources[cpt_source].adresse,source_ip);
            sources[cpt_source].packet_cpt = 1;
            cpt_source++;
        } 
        else{
            printf("\n\033[91mNouvelle source mais max sources atteint !\033[0m\n");
        }
    }
    else{
        
        sources[index].packet_cpt++;
    }

    qsort(sources, cpt_source, sizeof(struct source), compare);//tri
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
        printf("| IP source : \033[93m%s\033[0m\r\n", source_ip);
        sort_src(source_ip);
    }
    for(int i = 0 ; i<cpt_source; i++)
    {
        if(i<TOP)
            printf("num : %d ip: %s cpt :%d \n",i+1,sources[i].adresse,sources[i].packet_cpt);//test print
    }
    printf("================================\n\n");

    for(int i = 0; i < TOP ; i++) //copie du top des adresses du tableau pour mémoire partagée
        strcpy(top[i],sources[i].adresse);

    id_shm = write_p(top);
}

