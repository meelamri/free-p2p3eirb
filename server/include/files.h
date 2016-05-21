#ifndef FILES_H
#define FILES_H
#include "uthash.h"
#include "utlist.h"


typedef int bool;

struct name {
  char * name;
  struct name * next;
  struct name * prev;
};

struct session{
  int connectfd;
  /* char cliName[3]; */
  int portNum;
  char ipAddr[10];
};


typedef struct peer {
  struct session * session;
  char * port;
  struct name * dl_names;
  char * pieceSize;
  bool isSeed;
  struct peer * prev;
  struct peer * next;
} peer;


typedef struct file {
  char * length;
  char * key; 
  UT_hash_handle hh;
  struct peer * dl_peers;
} file;


/* FONTIONS MACROS */
#define name_set_name(n, n_name)  ((n)->name = (n_name))
#define name_set_pieceSize(n, n_size) ((n)->pieceSize = (n_size))
#define name_get_name(n) ((n)->name)

#define file_set_length(f, f_size)  ((f)->length = (f_size))
#define file_set_key(f, f_key) ((f)->key = f_key)

#define peer_set_port(p, p_port) ((p)->port = p_port)
#define peer_set_pieceSize(p, p_size) ((p)->pieceSize = p_size)
#define peer_set_isSeed(p, p_seed) ((p)->isSeed = p_seed)
#define peer_set_next(p, p_next) ((p)->next = p_next) 
#define peer_get_next(p) ((p)->next)

#define session_set_ipAddr(s, s_ip) (strcpy((s)->ipAddr, s_ip))
#define session_set_connectfd(s, s_fd) ((s)->connectfd = s_fd)
#define session_set_portNum(s, s_port) ((s)->portNum = s_port)

/* PRINT */
void print_file(void *file);
void print_allocated_files(void);
void print_files(void);
/* FREE ET MALLOC */
void session_free(struct session *);
void peer_free(struct peer *);
void name_free(struct name *);
void file_free(struct file *);
void file_tab_free(void * f_tab, int size);
struct session * session_malloc(void);
struct peer * peer_malloc(void);
struct file * file_malloc(void);
struct name * name_malloc(void);
struct file * base_malloc(void);
/* ADD FILE */
void add_file_name(struct name *dl_names, struct name *new);
int check_if_add_valid_ip(struct file *f, struct peer *tmp);
int check_if_add_valid(struct file *f, struct file* found);
int file_add(struct file * newfile);
int file_add_tab(int *files_not_added);
int test_valid_allocated_files(void);
/* GET RESPONSE */
void response_getfile(char *key);
void response_look_tab(void);
/* FILL ALLOCATED_FILES */
/* AUX */
int strcmpname(struct file *f, char *key);
int strcmpkey(struct file *f, char *key);
int strcmpsize(struct file *f, char * size_and_operator);
int cmp(char * size, char * sizecriterion, char *operator);
void add_ip_and_port(void);
int get_port(void);



/* LOOK  */
int file_look_filename_ht(char * filename);
int file_look_filesize_ht(char *size, char *operator);
int file_look_filename_tab(char *name);

/* GETFILE */
int file_getfile(char * key);
int file_search_tab(char * key, int (*cmp)(struct file *, char *));

/* UPDATE */
struct file * file_find(char * key);
void delete_files(char * ip);



#endif
