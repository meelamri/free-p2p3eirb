#ifndef LABO_H
#define LABO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>  //strlen
#include <unistd.h>  //write
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h> //inet_addr
#include <pthread.h>  //for threading , link with lpthread
#include <signal.h>
#include "files.h"
#include "scanner.h"


struct file * ht_files = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lockconRec = PTHREAD_MUTEX_INITIALIZER;
 
void *connection_handler(void *session);
int check_if_exists(int fd);
int setup_server(struct sockaddr_in server, int port_num);
void init_db(void);
void print_num_active_clients(void);
int check_session_exists(struct session *tmp);
void register_session(struct session * cli);
int delete_client(int fd);
int scan_request(char * ret, char * message, int size, struct session* client, int exists);

void signalHandler();


void delete_from_database(struct session *);
void generate_master_file_list(char * buffer);




#endif
