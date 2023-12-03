#ifndef __HANDLER_H__
#define __HANDLER_H__

#include <sys/ioctl.h>
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
#include "../libs/IPC/libpartage.h"


#define MAX_SRC 100


struct source {
    char adresse[IFNAMSIZ];
    int packet_cpt;
};


int compare(const void *a, const void *b);

void sort_src(char *source_ip);

void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet);

#endif