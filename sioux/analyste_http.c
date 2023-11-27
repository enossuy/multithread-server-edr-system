#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include "../libs/IPC/libpartage.h"

#define WEB_DIR  "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024
#define MAX_BUFFER_PATH 2000
#define CODE_OK  200
#define CODE_NOTFOUND 404


#define MAX_LIGNE 1024

int gestionClient(int s){



FILE *dialogue=fdopen(s,"a+");
  char cmd[MAX_BUFFER];
  char page[MAX_BUFFER];
  char proto[MAX_BUFFER];
  char path[MAX_BUFFER_PATH];
  char complete_path[MAX_BUFFER_PATH];
  char type[MAX_BUFFER];
if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }

char ligne[MAX_LIGNE];

while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL)
{
  fprintf(dialogue,"> %s",ligne);
  if(sscanf(ligne,"%s %s %s",cmd,page,proto)!=3) 
  {
    printf("\033[91mCommande non reconnue, arrêt.\033[0m\r\n");
    exit(-1);
  }
  
  fprintf(dialogue,"> cmd :%s page :%s proto:%s\n",cmd, page,proto);

  /*On cherche si il ya "?""*/
  char *page_arg = strtok(page, "?");
  page_arg = strtok(NULL, "?");

  if(strcmp("/",page) == 0)
  {
    strcpy(path,"./www/index.html");
  }
  else if(strcmp("/ablette.html",page) == 0)
  {
    char result[10][16];
    int id = read_p(result);
    strcpy(path,"./www/ablette.html");

    /*Création/Ecriture CSV*/
    FILE *ablette=fopen("./www/ablette.html","w+");
    for(int i = 0 ; i < 10 ; i++)
      fprintf(ablette,"%s",result[i]);
    fclose(ablette);
  }
  else
  {
    sprintf(path,"%s%s",WEB_DIR,page);
    if (page_arg != NULL) {

      /*Mise en place du chemin pour le fichier CSV*/
      char page_to_extract[MAX_BUFFER];
      strcpy(page_to_extract,page);
      char *csv = strtok(page_to_extract,".");
      size_t sizeCSV = strlen(WEB_DIR) + 1 +  strlen(page_to_extract) + 1;
      char *csv_path = (char *) malloc( sizeCSV );

      strcpy(csv_path,WEB_DIR);
      strcat(csv_path,page_to_extract);
      strcat(csv_path,".csv");

      /*Création/Ecriture CSV*/
      FILE *vote_csv=fopen(csv_path,"a+");
      fprintf(vote_csv,"%s;",page_arg);
      fclose(vote_csv);
    }
    else
    {
      fprintf(dialogue,"Pas d'argument \n");
    }
  }

  if(strcasecmp(cmd,"GET")==0){
  int code=CODE_OK;
  struct stat fstat;

  fprintf(dialogue,"> path : %s\n",path);
  if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode)){
    sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
    fprintf(dialogue,"page non trouvée\n");
    code=CODE_NOTFOUND;

    //ici aussi open error.html?
    }
  else{
    fprintf(dialogue,"page trouvée\n");

    strcpy(type,"text/html");
    char *end=page+strlen(page);
    if(strcmp(end-4,".png")==0) strcpy(type,"image/png");
    if(strcmp(end-4,".jpg")==0) strcpy(type,"image/jpg");
    if(strcmp(end-4,".gif")==0) strcpy(type,"image/gif");
    fprintf(stdout,"\n\033[92mEn-tête\033[0m\r\n");
    fprintf(stdout,"HTTP/1.0 CODE : %d\r\n",code);
    fprintf(stdout,"Server: CWeb\r\n");
    fprintf(stdout,"Content-type: %s\r\n",type);
    fprintf(stdout,"Content-length: %ld\r\n",fstat.st_size);
    fprintf(stdout,"\r\n");
    fflush(stdout);
    int fd=open(path,O_RDONLY);
    if(fd>=0){
    int bytes;
    while((bytes=read(fd,ligne,MAX_BUFFER))>0) write(1,ligne,bytes);
    send(s, HTML_RESPONSE,strlen(HTML_RESPONSE),0);
    close(fd);



    /*int pid = fork();
    if(pid  == 0) //fils
      execlp("open", "open", path, NULL); // fork ?*/
    }
    }
  }
}

/* Termine la connexion */
fclose(dialogue);
return 0;
}

