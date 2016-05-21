#ifndef SCANNER__H
#define SCANNER__H


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "lex.yy.h"
#include "files.h"
#include "scanner_.h"


#define MAX_FILE_SIZE 100
#define ERROR -1
#define TWO_TIMES 1
#define EMPTY_FILE "none"
#define MAX_CRITERION 2
#define FIRST_CRITERION FILE_NAME


#define scan(token) do {if(scan_## token ()) err = 1;}while(0)	\

#define RESPONSE_ERROR(type_token) \
  sprintf(response, "Syntax error in line %d, Expected " #type_token " but found %s\n", yylineno, yytext)

#define if_not_identifier(token)					\
  if((token) != IDENTIFIER && (token) != FILE_NAME && (token) != FILE_SIZE && (token) != ANNOUNCE && (token) != LOOK && (token) != GETFILE && (token) != UPDATE && (token) != LISTEN && (token) != SEED && (token) != LEECH) \


#define if_not_key(token) \
  if((token) != KEY && (token) != IDENTIFIER && (token) != INTEGER && (token) != FILE_NAME && (token) != FILE_SIZE && (token) != ANNOUNCE && (token) != LOOK && (token) != GETFILE && (token) != UPDATE && (token) != LISTEN && (token) != SEED && (token) != LEECH) \

/* --------- functions used by flex ---------------- */
extern int yylex();
extern int yylineno;
extern char* yytext;
extern  int yy_buffer_state;
typedef struct yy_buffer_state* YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string ( char *);
extern struct file * ht_files;
extern pthread_mutex_t  lock;

char *key_word[] = { NULL, "announce", "look", "getfile", "update", "listen", "seed", "leech"};
void * allocated_files;
int size_files;
struct peer * tmp_peer;
char * response;
int scan_leech(int is_seed);
int scan_request(char * ret, char * message, int size, struct session *, int exists);
int scan_seed();
int scan_announce();
int scan_look();
int scan_update();
int scan_getfile();


#endif
