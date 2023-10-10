#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>


#define WEB_DIR  "./www"
#define PAGE_NOTFOUND "error.html"
#define MAX_BUFFER 1024

#define CODE_OK  200
#define CODE_NOTFOUND 404


#define MAX_LIGNE 100

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

            FILE *vote_csv=fopen("./www/vote-reponse.csv","a+");
            fprintf(dialogue,"argument dans la requete: %s\n", argument);
            fprintf(vote_csv,"%s;",argument);
            fclose(vote_csv);

        } else {
            fprintf(dialogue,"pas d'argument dans la requete\n");
        }
    }


  if(strcasecmp(cmd,"GET")==0){
  int code=CODE_OK;
  struct stat fstat;
  sprintf(path,"%s%s",WEB_DIR,page_arg);


  if(strcmp("./www/",path) == 0)
      strcpy(path,"./www/index.html");


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
