#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pcap.h>
#include "./options.h"
#include "../libs/Reseau/libserveur.h"


    
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    printf("Packet captured with length: %d\n", pkthdr->len);

}

int main(int argc, char *argv[]) {
    char interface[20] = ""; 
    int ports[MAX_PORT];

    int size_port = parseArguments(argc, argv, interface, ports);

    /*Affichage, test*/
    printf("interface : %s\n", interface);
    printf("Ports :\n"); for (int i = 0; i < size_port; i++) printf("%d\n", ports[i]);

    char errbuff[PCAP_ERRBUF_SIZE]; // pour y mettre buffers d erreurs

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
    char filter_exp[] = "dst host 192.168.17.114 and (port 80 or port 443)";

    if (pcap_compile(handler, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
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

