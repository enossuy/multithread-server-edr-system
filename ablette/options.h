#ifndef __OPTIONS_H_
#define __OPTIONS_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_PORT 20
int parseArguments(int argc, char *argv[], char *interface, int ports[]);

#endif