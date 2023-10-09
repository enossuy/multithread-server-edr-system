int initialisationServeur(short int *port,int connexions);
int boucleServeur(int ecoute,int (*traitement)(int));
int gestionClient(int s);
int analyseArguments(int argc,char *argv[]);
void err_syntax(void);
int parsing(int valopt, char *optarg);
