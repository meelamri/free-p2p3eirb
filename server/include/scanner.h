#ifndef LABO_H
#define LABO_H
#include "files.h"
#include <pthread.h>

int scan_leech(int is_seed);
int scan_request(char * ret, char * message, int size, struct session* client, int exists);
int scan_seed();
int scan_announce();
int scan_look();
int scan_update();
int scan_getfile();



#endif
