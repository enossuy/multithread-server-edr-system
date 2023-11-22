int initialisationServeur(short int *port,int connexions);
int boucleServeur(int ecoute,void* (*traitement)(void*));

