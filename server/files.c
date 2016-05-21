
#include "./include/files_.h"

/* ---------------------------------------------------------------------- */
/* SOME CODES LEFT AS COMMENT DEFINES SOME OTHER OPTIONS CAN BE ADDED TO  */
/* ------------------------- PRINT ----------------------------------------*/


void print_file(void * file)
{
  struct peer *p;
  struct name *n;
  struct file *f = (struct file *) file;
  int i =1;
  if (file == NULL)
    printf("empty allocated_files\n");
  else
    {
    
      printf("\t key:%s | length:%s |\n", f->key, f->length);
     
      DL_FOREACH(f->dl_peers,p) 
	{
	  printf("\t\t listening port:%s | piece size:%s | is seed?:%d | ", p->port, p->pieceSize, p->isSeed);
	  DL_FOREACH(p->dl_names, n)
	    printf("name:%s \n ", n->name);
	  printf("\t   PEERS: \n");
	  printf("\t\t peer %d: ", i); 
	  printf("ip:%s | port:%d | fd sock:%d\n ", p->session->ipAddr, p->session->portNum, p->session->connectfd);
		 
	  i++;	 	 
	}
     
    }
    
}


void print_allocated_files(void)
{
 struct file **f = (struct file **) allocated_files;
 int i;
 
 if (f == NULL)
   {
     printf("empty allocated_files\n");
     return ;
   }
 printf("files not already registered :\n");
 for(i=0; i < size_files; i++)
   {
     print_file((void *) (f[i]));
   }
}

void print_files(void)
{
  struct file * f;
  int i = 1;
  if (ht_files == NULL){
    printf("set of files is empty\n");
    return;
  }
  printf("DATA BASE:\n");
  for (f=ht_files; f != NULL; f=(struct file *)(f->hh.next))
    {
      if (strcmp(f->key, "") == 0) continue ;
     
      printf("\tfile %d:", i);
      print_file((void *) f); 
      i++;
     
    }
}



/* --------------------------MALLOC & FREE FOR EACH STRUCTURE --------------------------------- */
/*                            MALLOC                                  */

struct file * base_malloc(void)
{
  struct file * f = file_malloc();
  struct name * n =name_malloc();
  f->dl_peers = peer_malloc();
  f->dl_peers->dl_names = NULL;
  DL_APPEND(f->dl_peers->dl_names, n);
  return f;
}
struct file * file_malloc(void)
{
  struct file * f = (struct file *) malloc(sizeof(struct file));
  f->length = f->key =  NULL; 
  return f;
}

struct name * name_malloc(void)
{
  struct name * n = (struct name *)malloc(sizeof(struct name));
  n->name = NULL;
  n->next = n->prev = NULL;
  return n;
}

struct peer* peer_malloc(void)
{
  struct peer * p = (struct peer *)malloc(sizeof(struct peer));
  p->session = session_malloc();
  p->pieceSize = p->port = NULL;
  p->next =p->prev = NULL;
  p->isSeed = 0;
  return p;
}

struct session* session_malloc(void)
{
  struct session* s = (struct session *)malloc(sizeof(struct session));
  memset(s->ipAddr, '\0', 10);
  s->portNum = s->connectfd = -1;
  return s;
}

/*                              FREE                              */

void file_tab_free(void * f_tab, int size)
{
  if ( NULL == f_tab)
    return ;
  if ( size == 0)
    {
      free(f_tab);
      return ;
    } 
  int i = 0;
  struct file ** f = (struct file **)f_tab;
  for ( i = 0; i < size ; i++)
    file_free(f[i]);
  free(f_tab);
}


void file_free(struct file *f)
{
if (NULL == f)
  return ;
free(f->key);
free(f->length);
peer_free(f->dl_peers);
free(f);
}

void peer_free(struct peer *p)
{
  if ( NULL == p )
    return;
 free(p->port);
 free(p->pieceSize);
 name_free(p->dl_names);
 session_free(p->session);
 free(p);
}

void name_free(struct name *n)
{
  if(n == NULL)
    return ;
  free(n->name);
  free(n);
}

void session_free(struct session *s)
{
  if( NULL == s)
    return ;
  free(s->ipAddr);
  free(s);
}



/* ---------------------- ADD FILE -------------------- */
int test_valid_allocated_files(void)
{
  int i, j;
   struct file ** tmp_allocated_files = (struct file **)allocated_files;
   for (i = 0; i < size_files-1; i++){
     for (j = i+1; j < size_files; j++){
       
       if (strcmp(tmp_allocated_files[i]->key, tmp_allocated_files[j]->key) ==0 && (strcmp(tmp_allocated_files[i]->length, tmp_allocated_files[j]->length) != 0)){
	 sprintf(response, "files duplicated\n");
	 return 1;
       }
     }
   }
   return 0;
}


int file_add_tab(int *files_not_added)
{
  int i, j;
   struct file ** tmp_allocated_files = (struct file **)allocated_files;
   if (files_not_added == NULL)
    return 0;
   /* if(test_valid_allocated_files()) */
   /*   return 1; */
   j = 0;
   for (i = 0; i < size_files; i++)
     if (file_add((struct file *)tmp_allocated_files[i])) {
       files_not_added[j] = i;
       j++;
     }
   return j;
}

int file_add(struct file * newfile)
{
  struct file * found;    
  struct peer * tmp;
  if ( newfile == NULL)
    return 0;
  HASH_FIND_STR(ht_files, newfile->key, found);
  
  if (found == NULL) {
    tmp = newfile->dl_peers;
    newfile->dl_peers = NULL;
    DL_APPEND(newfile->dl_peers, tmp);
    HASH_ADD_KEYPTR(hh, ht_files, newfile->key,  strlen(newfile->key), newfile);//free out
    return 0;
  }
  return check_if_add_valid(newfile,found);
 }

//adding file whose key exists already in the data base
int check_if_add_valid(struct file *f, struct file* found)
{
  struct peer *tmp;
  //files with same key have same length
  if (strcmp(found->length, f->length) != 0) {
    sprintf(response, "file exists already with a different length.Please keep the size of the file or change the key\n");	
    return 1;
  }

  DL_FOREACH(found->dl_peers, tmp) {
    //Client has already this file
    /* or teste pour connectfd */
    if ((strcmp(tmp->session->ipAddr, f->dl_peers->session->ipAddr) == 0) && (tmp->session->portNum == f->dl_peers->session->portNum)) {
      //the client can only modify the name of file
      if (check_if_add_valid_ip(f, tmp)){
	file_free(f);
	return 1; 
      }
      file_free(f);
      return 0;
    }
  }
  //Other client has the same key file
  tmp = f->dl_peers;
   DL_APPEND(found->dl_peers, tmp);
  free(f->key);
  free(f->length);
  free(f);
 
 return 0;
}


//File exists already.it's only permitted to modify add other name of file
int check_if_add_valid_ip(struct file *f, struct peer *tmp)
{
  if (strcmp(tmp->port, f->dl_peers->port)) {
    sprintf(response, "you have already this file.Please, keep the same port number\n");
    return 1;
  }
  if (tmp->isSeed != f->dl_peers->isSeed) {
    sprintf(response, "you have already this file.Please keep the same statut of seed.use update to update this statut \n");
    return 1;
  } 
  if (strcmp(tmp->pieceSize, f->dl_peers->pieceSize)) {
    sprintf(response, "you have already this file.please, keep the same piece size");
    return 1;
  }
  /* add_file_name(f->dl_peers->dl_names, tmp->dl_names) */
  return 0;
}

//add "one" name to a list of names
void add_file_name(struct name *dl_names, struct name *new)
{

  struct name* el; 
  DL_FOREACH(dl_names, el) {
    if (strcmp(el->name, new->name) == 0)
      return;
  }
  el = name_malloc();
  sprintf(el->name, "%s", new->name);
  DL_APPEND(dl_names,el);
  return ;
}



/* --------------------------- GET RESPONSE ---------------------- */
void response_look_tab(void)
{
  struct file **f = (struct file **)allocated_files;
  int i;
  char *src ;
  char *tmp=malloc(1024); 
  memset(response, '\0', strlen(response));
  if ( size_files == 0)
    return;
  
  for (i=0; i<size_files; i++)
    {
      if(tmp_peer->session->connectfd != f[i]->dl_peers->session->connectfd) {
        if (i == 0)
             sprintf(tmp,"%s %s %s %s",f[i]->dl_peers->dl_names->name, f[i]->length, f[i]->dl_peers->pieceSize, f[i]->key);
        else
          sprintf(tmp," %s %s %s %s", f[i]->dl_peers->dl_names->name, f[i]->length, f[i]->dl_peers->pieceSize, f[i]->key);
      }
      strcat(response, tmp); 
    }
  src = malloc (sizeof(char) * 1024);
  sprintf(src, "list [%s]\n", response);
  sprintf(response, "%s", src);
  free(src);
}

void response_getfile(char *key)
{
  int i =0; 
  struct peer *p;
  struct file * f = (struct file *) allocated_files;
  char *src = malloc(sizeof(char) * 1024);
  sprintf(response, "peers %s [", key);
  DL_FOREACH(f->dl_peers, p)
    {
      if (tmp_peer->session->connectfd != p->session->connectfd) {
	if(i==0){
      sprintf(src,"%s:%s", p->session->ipAddr, p->port); 
      strcat(response, src);
	}
	if(i>0){
       sprintf(src," %s:%s", p->session->ipAddr, p->port); 
       strcat(response, src); 
	}
      }
    }
  strcat(response, "] \n");
  free (src);
}


/* --------------------- FILLING  ALLOCATED_FILES ----------------- */

/*              AUXILIARY SMALL FUNCTIONS                    */

int strcmpname(struct file *f, char * name)
{
  if( strcmp(f->dl_peers->dl_names->name, name))
    return 0;
  return 1;
}

int strcmpkey(struct file *f, char *key)
{
  if(strcmp(f->key, key))
    return 0;
  return 1;
}

int strcmpsize(struct file *f, char * size_and_operator)
{
  char ** p = (char **) size_and_operator;
  return cmp(f->length, p[0], p[1]);
}

int cmp(char * sizefile, char * sizecriterion, char * op)
{
  char operator = op[0];	/*  case: <= */
  switch (operator)
    {
    case '=':
      if(atoi(sizefile) - atoi(sizecriterion) == 0)
	 return 1;
      break;
    case '>':
      return atoi(sizefile) - atoi(sizecriterion);
    case '<':
      return atoi(sizecriterion) - atoi(sizefile);
    }
     return 0;
}

void add_ip_and_port(void)
{
  int i;
  struct file ** f = (struct file **) allocated_files;
  for (i = 0; i < size_files; i++)
    {
      peer_set_port( f[i]->dl_peers, strdup(tmp_peer->port));
      session_set_ipAddr(f[i]->dl_peers->session, tmp_peer->session->ipAddr);
      session_set_portNum(f[i]->dl_peers->session, tmp_peer->session->portNum);
      session_set_connectfd(f[i]->dl_peers->session, tmp_peer->session->connectfd);

    }
}

int get_port(void)
{
  struct file *f;
  struct peer *p;
  for (f=ht_files; f != NULL; f=(struct file *)(f->hh.next))
    {
      DL_FOREACH(f->dl_peers, p){
	if (strcmp(p->session->ipAddr, tmp_peer->session->ipAddr) == 0 && (p->session->portNum == tmp_peer->session->portNum)){
	  if (p->port != NULL){
	    tmp_peer->port = p->port;
	    return 0;
	  } }}
    }
  return 1;
}


/*                              LOOK                           */

int file_look_filename_ht(char * filename)
{
  struct file *f;
  struct peer *p;
  struct name *n;
  struct file ** tmp_allocated_files = (struct file **) allocated_files;
  
  for (f=ht_files; f != NULL; f=(struct file *) (f->hh.next))
    {
      DL_FOREACH(f->dl_peers, p)
	{
	  DL_FOREACH(p->dl_names, n)
	    if (strcmp(n->name, filename) == 0)
	      {
		tmp_allocated_files[size_files] = f;
		size_files++;
	      }
	}
    }
  if (size_files == 0)
    return 1;

  return 0;
}

int file_look_filesize_ht(char *size, char *operator)
{
  struct file *f;
  struct file **tmp_allocated_files = (struct file **) allocated_files;

  size_files = 0;
  for (f=ht_files; f != NULL; f=(struct file *)(f->hh.next))
    {
      if (strcmp(f->length, "") && cmp(f->length,size, operator) > 0){
	tmp_allocated_files[size_files] = f;
	size_files++;
      }  
    }
  /* if (size_files == 0) */
  /*   return 1; */
  return 0;
}

/* rewrite thi function */
 int file_look_filename_tab(char *filename)
{
  int i;
  int pos[size_files];
  struct file ** files = (struct file **)allocated_files;
  struct file * tmp_allocated_files[100];
  int size = size_files;
  struct name * el_n;
  struct peer * el_p;
  size_files = 0;
  for(i = 0; i < size; i++)
    DL_FOREACH(files[i]->dl_peers, el_p)
      DL_FOREACH(el_p->dl_names,el_n)
      if (strcmp(el_n->name,filename) == 0)
      {
	pos[size_files] = i;
	size_files++;
      }
 
  if ( size_files == 0)
    return 1;

  
 for(i = 0; i < size_files; i++)
    tmp_allocated_files[i] = files[pos[i]];
  
 for(i = 0; i < size_files; i++)
   files[i] = tmp_allocated_files[i]; 

  return 0;
}

/*                   GETFILE                      */
int file_getfile(char * key)
{
  struct file *f;
  HASH_FIND_STR(ht_files, key, f);
  if( f == NULL)
    return 1;
  allocated_files = (void *)f;
  return 0;
}

int file_search_tab(char * key, int (*cmpkey)(struct file *, char *))
{
  int i;
  int pos[size_files];
  struct file ** files = (struct file **)allocated_files;
  struct file * tmp_allocated_files[100];
  int size_init = size_files;
  size_files = 0;
  for(i = 0; i < size_init; i++)
    if (cmpkey(files[i], key) > 0)
      {
	pos[size_files] = i;
	size_files++;
      }
  
  if ( size_files == 0)
    return 1;

  for(i = 0; i < size_files; i++)
    tmp_allocated_files[i] = files[pos[i]];

  for(i = 0; i < size_files; i++)
    files[i] = tmp_allocated_files[i];

  for ( i = size_files; i < size_init; i++)
    files[i] = NULL;
  
  return 0;
}

/*                         UPDATE                        */
void delete_files(char * ip)
{

}

struct file * file_find(char * key)
{
  struct file *f;
  HASH_FIND_STR(ht_files, key, f); //f: output pointer
  return f;
}

/* int peer_cmp(struct peer *a, struct peer * b) */
/* { */
/*   return strcmp(a->ip, a->ip) && strcmp(a->port, b->port) ; */
/* } */

/* void file_delete(struct file * f) */
/* { */
/*   HASH_DEL(ht_files, f); */
/*   free(f); */

/* } */

/* void delete_all() */
/* { */
/*   struct file *current_file, *tmp; */
/*   HASH_ITER(hh, ht_files, current_file, tmp) { */
/*     HASH_DEL(ht_files, current_file); */
/*     free(current_file); */
/*   } */
/* } */



