#include "handler.h"

extern int id_shm;

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

    /*=== Construction de l'expression pour le filtre ===*/// De base : "dst host 192.168.17.114 and (port 80 or port 443)"
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
    printf("expression : %s\n", expression);
    
    //compilation

    if (pcap_compile(handler, &fp, expression, 0, PCAP_NETMASK_UNKNOWN) == -1) {
       perror("Erreur compilation filtre\n");
        pcap_close(handler);
        exit(-1);
    }

    //filtre
    if (pcap_setfilter(handler, &fp) == -1) {
        perror("Erreur set filtre\n");
        pcap_close(handler);
        exit(-1);
    }

    printf("Capture sur l'interface %s en cours\n", interface);

    //boucle de capture

    if (pcap_loop(handler, -1, packet_handler, NULL) != 0) {
        perror("Erreur capture paquets\n");
        pcap_close(handler);
        exit(-1);
    }

    free_shm(id_shm);
    pcap_close(handler);
    return 0;
}

