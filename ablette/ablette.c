

#include <pcap.h>



/** Recherche du numero systeme d'une interface **/
int getInterfaceIndex(int sock,char *ifname){
  struct ifreq ifr;
  strcpy(ifr.ifr_name,ifname);
  if(ioctl(sock,SIOCGIFINDEX,&ifr)<0){
    perror("getInterfaceIndex.ioctl");
    exit(-1);
    }
  return ifr.ifr_ifindex;
  }
