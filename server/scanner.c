#include "./include/scanner_.h"
#include <pthread.h>

#define false 1
#define true 0

/*---------------------------- MAIN FUNCTION: EXAMINE THE SOCKET--------- */
int scan_request(char *ret, char * message, int size,  struct session *client, int exists)
{
  /* YY_BUFFER_STATE buffer; */
  yy_scan_string(message);
  int ntoken, port;
  /* struct sockaddr_in client; */
  /* socklen_t len = sizeof(client); */
  char * ip;
  int files_not_added[MAX_FILE_SIZE];
  int err_file_add;
  struct file **tmp_allocated_files;
 
  response = ret;

  ntoken = yylex();
  if (exists == false && ntoken != ANNOUNCE) {
    RESPONSE_ERROR('you aren\'t connected to the Tracker.Please Use "announce" command before');
    return 1;
  }

  tmp_peer = peer_malloc();
  port = client->portNum;
  ip = client->ipAddr;

  session_set_ipAddr(tmp_peer->session, ip);
  session_set_portNum(tmp_peer->session, port);
  session_set_connectfd(tmp_peer->session, client->connectfd);
		      
  allocated_files = malloc(sizeof(struct file *) * MAX_FILE_SIZE);
  tmp_allocated_files = (struct file**) allocated_files;
  switch (ntoken)
    {
      int i;
      
    case ANNOUNCE:
      
      if(scan_announce()) {
	//peer_free(tmp_peer);
	//file_free_tab(allocated_files, size_files);
	return 1;
      }
      add_ip_and_port();
    
      /* peer_free(tmp_peer); */
      pthread_mutex_lock(&lock);
      err_file_add = file_add_tab(files_not_added);
      pthread_mutex_unlock(&lock);
    
      if (err_file_add) {
	printf("this files belong are not added\n");
	for (i=0; i < err_file_add; i++)
	  printf("\t%s ",tmp_allocated_files[files_not_added[i]]->dl_peers->dl_names->name); 
	printf("\n");
      }
      print_files();
      break;

    case GETFILE:
      //peer_free(tmp_peer);
      //file_free_tab(allocated_files, size_files);
    
      if (scan_getfile())
	return 1;
      break;
    
    case LOOK:
      // peer_free(tmp_peer);
      if(scan_look())
    	return 1;
 
      break;
      
    case UPDATE:
     
      /* if ( get_port()) */
      /*  printf("port doesn't exist"); */

      
 
      if(scan_update()) {
	// peer_free(tmp_peer);
	//file_free_tab(allocated_files, size_files);
    	
	return 1;
      }
      /* peer_find_port(ip); */
      /* peer_set_port(tmp_peer, peer_find_port(ip)); */
      /* add_ip_and_port(tmp_peer); */
      /* peer_free(tmp_peer); */
      
      print_allocated_files();
      pthread_mutex_lock(&lock);
      err_file_add = file_add_tab(files_not_added);
      pthread_mutex_unlock(&lock);
      
      print_files();
      break;
   
    default:
      sprintf(response, "Syntax error in line %d\n", yylineno);
    }

  free(allocated_files);
  return 0;
}

/* ------------------------- ANNOUNCE --------------------------------- */
int scan_announce()
{
  int one_key_word[3] = {0};
  int vtoken = yylex();
  size_files = 0;
	
  while(vtoken) {
    switch (vtoken) {
    case LISTEN:
      if ( one_key_word[0] == TWO_TIMES ) {
	RESPONSE_ERROR('listen duplicated');
	return 1; 
      }
      (one_key_word[0])++ ;
      if (yylex() != INTEGER) { 
	RESPONSE_ERROR('INTEGER');
	return 1;
      }
      peer_set_port(tmp_peer, strdup(yytext)); 
      
      break;
      
    case SEED:
      if (one_key_word[1] == TWO_TIMES ) {
	RESPONSE_ERROR('seed duplicated');
	return 1;
      }
      (one_key_word[1])++;

      if (scan_seed()) {
	return 1;
      }
      break ;
      
    case LEECH:
      if (one_key_word[2] == TWO_TIMES) {
	RESPONSE_ERROR('leech duplicated');
	return 1;
      }
      (one_key_word[2])++;
      
      if(scan_leech(0))
	{ 
	  return 1;
	}
      break;
      
    default:
      sprintf(response, "Syntax error in line %d\n", yylineno);
      return 1;
    }
    vtoken = yylex();
  }
  if (one_key_word[0] != TWO_TIMES) {
    RESPONSE_ERROR('don\'t forget listen');
    return 1;
  }
  sprintf(response, "ok\n");
  
  return 0;
}
/* ----------------------- SEED ----------------------------------------- */
int scan_seed()
{
  struct file * new_file;
  struct file **tmp_allocated_files = (struct file **) allocated_files;
 
  if (yylex() != OPEN_HOOK )
    {
      RESPONSE_ERROR('[');
      return 1;
    }
  int vtoken = yylex();
    
  while (vtoken != CLOSED_HOOK)
    {

      tmp_allocated_files[size_files] = base_malloc();
      new_file = tmp_allocated_files[size_files];
      size_files++;
      
      if_not_identifier(vtoken)
      {
      	RESPONSE_ERROR(']');
      	return 1;
      }
      name_set_name(new_file->dl_peers->dl_names, strdup(yytext));
      
      vtoken = yylex();
      if (vtoken != INTEGER)
	{
	  RESPONSE_ERROR('INTEGER');
	  return 1;
	}
      file_set_length(new_file, strdup(yytext));	  

      vtoken = yylex();
      if (vtoken != INTEGER)
	{
	  RESPONSE_ERROR('INTEGER');
	  return 1;
	}
      peer_set_pieceSize(new_file->dl_peers, strdup(yytext));
      
      vtoken = yylex();
      if_not_key(vtoken) {
	RESPONSE_ERROR('key');
	return 1;
      }
      file_set_key(new_file, strdup(yytext));
      
      vtoken = yylex();
      if (!vtoken) {
	RESPONSE_ERROR(']');
	return 1;
      }
      peer_set_isSeed(new_file->dl_peers, 1);
    }
    
  return 0;
}
/* --------- LEECH:USED TO PARSE THE FIELD LEECH IN A ANNOUNCE REQUEST AND TO SCAN FIELDS OF THE UPDATE COMMAND, DEPENDS THE PAREMTER IS_SEED WHICH COMBINE THREE THINGS IN ONE  :) -------------------------------- */
int scan_leech(int is_seed)
{
  
  int vtoken = yylex();
  struct file *found, *new_file;
  struct file **tmp_allocated_files;
  char * key, exists = 0;
  int seed_count = 0, leech_count = 0;
  struct peer * seed_files[100];
  struct peer * leech_files[100];
  struct peer *el; 
  tmp_allocated_files = (struct file **) allocated_files;
  if ( vtoken != OPEN_HOOK){
    RESPONSE_ERROR("[");
    return 1;
  }
  vtoken = yylex();
  while (vtoken != CLOSED_HOOK) {
    if_not_key(vtoken) {
      RESPONSE_ERROR(']');
      return 1;
    }
    key = yytext;
    found = file_find(key);
    
    if ( found == NULL) {
      RESPONSE_ERROR("file  doesn't exist");
      return 1;
    }
    
    DL_FOREACH(found->dl_peers, el) {
     
      if ( (strcmp(el->session->ipAddr, tmp_peer->session->ipAddr) == 0) && (el->session->portNum == tmp_peer->session->portNum) ){
	
	if (is_seed == 1 && el->isSeed == 0) {
	  seed_files[seed_count] = el;
	  seed_count++;
	}
	else if (is_seed == 0 && el->isSeed == 1){
	  leech_files[leech_count] = el;
	  leech_count++;
	}
	exists = 1;
	break;
      }
    }
      
    if (exists == 0) {
	
      new_file = base_malloc();
      file_set_key(new_file, strdup(key));
      file_set_length(new_file, strdup(found->length));
      sprintf(new_file->dl_peers->session->ipAddr, "%s", tmp_peer->session->ipAddr);
    
      new_file->dl_peers->session->portNum = tmp_peer->session->portNum;
      new_file->dl_peers->session->connectfd = tmp_peer->session->connectfd;
      new_file->dl_peers->isSeed = is_seed;
      peer_set_pieceSize(new_file->dl_peers, strdup(found->dl_peers->pieceSize));
      /* sprintf(new_file->dl_peers->port, "%s", tmp_peer->port); */
      name_set_name(new_file->dl_peers->dl_names, strdup(found->dl_peers->dl_names->name));
      tmp_allocated_files[size_files] = new_file;
      size_files++;
    }
    exists = 0;    
    vtoken = yylex();
   
    if (!vtoken) {
      RESPONSE_ERROR(']');
      return 1;
    }
  }
  
  int i;
  for (i=0; i < seed_count; i++)
    seed_files[i]->isSeed = 1;
  for (i=0; i<leech_count; i++)
    leech_files[i]->isSeed = 0;//tester s'il est le seul
  return 0;
}


/* ---------------- UPDATE --------------------------------------- */

/* traiter scan_leech */
int scan_update()
{
  int ntoken = yylex();
  size_files = 0;
 
  switch (ntoken)
    {
    case SEED:
      if (scan_leech(1))
	return 1;
      
      break;
    case LEECH:
      if (scan_leech(0))
	return 1;
      break;

    default:
      sprintf(response, "Syntax error in line %d\n", yylineno);
    }

  sprintf(response, "ok\n");

  return 0;
}

/* ----------------------- LOOK ------------------------------------- */

int scan_look()
{
  char * operator;
  int one_key_word[MAX_CRITERION] = {0};
  int search_from_ht = 1; 
  char * name_file;
  char * size;
  int tmp = 0, ntoken, loop = 0;
  char * size_and_operator[2];
  if (yylex() != OPEN_HOOK)
    {
      RESPONSE_ERROR('[');
      return 1;
    }
  
  ntoken = yylex();
  if (ntoken == CLOSED_HOOK)
    {
      if (yylex())
	{
	  RESPONSE_ERROR(empty);
	  return 1;
	}
      sprintf(response, "list []\n");
      return 0;
    }

  if (yylex() != OPERATOR)
    {
      RESPONSE_ERROR("operator");
      return 1;
    }
  operator = strdup(yytext);
  
  while (ntoken)
    { 
      switch (ntoken)
	{
	  
	case FILE_NAME:
	  
	  if (one_key_word[0] == TWO_TIMES){ 
	    RESPONSE_ERROR('criterion name of file is duplicated');
	    return 1;
	  }
	  one_key_word[0]++;
	  if (tmp){
	    if ( yylex() != OPERATOR){
	      RESPONSE_ERROR(empty);
	      return 1;
	    }
	    operator = strdup(yytext);
	  }
	  tmp = 1;
	  
	  if ( strcmp(operator, "=") )  {
	    RESPONSE_ERROR("operator");
	    return 1;
	  }
	  if (yylex() != QUOTE) {
	    RESPONSE_ERROR("quote");
	    return 1;
	  }
	  ntoken = yylex();
	  if_not_identifier (ntoken) {
	    RESPONSE_ERROR('name of file');
	    return 1;
	  }
	  name_file = strdup(yytext);
	  if(yylex() != QUOTE) {
	    RESPONSE_ERROR("quote");
	    return 1;
	  }
	  if(search_from_ht == 1) {
	    size_files = 0;
	    if (file_look_filename_ht(name_file))
	      sprintf(response, "list []\n");
    	  }
	  else {
	    if (file_search_tab(name_file, strcmpname))
	      sprintf(response, "list []\n");	    
	  }
	  
	  free(name_file);
	  search_from_ht = 0;
	  break;
     
 	case FILE_SIZE:
	  if (one_key_word[1] == TWO_TIMES) {
	    RESPONSE_ERROR("criterion size of file is duplicated");
	    return 1;
	  }
	  one_key_word[1]++;
	  if (tmp) {
	    if ( yylex() != OPERATOR){
	      RESPONSE_ERROR(empty);
	      return 1;
	    }
    	    operator = strdup(yytext);
	  }
	  tmp = 1;

	  if (yylex() != QUOTE) {
	    RESPONSE_ERROR(quote);
	    return 1;
	  }
	  if (yylex() != INTEGER) {
	    RESPONSE_ERROR('size of file');
	    return 1;
	  }
	  size = strdup(yytext);
	  if(yylex() != QUOTE) {
	    RESPONSE_ERROR(quote);
	    return 1;
	  } 
	  if (search_from_ht == 1) 
	    file_look_filesize_ht(size, operator);
	  
	  else {
	    size_and_operator[0] = size;
	    size_and_operator[1] = operator;
	    file_search_tab((char *)size_and_operator, strcmpsize);
	  }
	  search_from_ht = 0;
	  free(size);
	  free(operator);
	  break;

	default:
	  RESPONSE_ERROR(']');
	  return 1;
	}

      ntoken = yylex();  
      if (ntoken == CLOSED_HOOK) {
	loop = 1;
	ntoken = 0;
	if (yylex()) {
	  RESPONSE_ERROR(empty);
	  return 1;
	}
      }
    }
  if (loop == 0) {
    RESPONSE_ERROR('close up your command');
    return 1;
  }
    
  if (size_files == 0) {
    sprintf(response, "list []\n");
  }
  else	    
    response_look_tab();
  return 0;      
}


/* ---------------------    GETFILE   ------------------------------ */
int scan_getfile()

{ 
  char *key_ = malloc (sizeof(char) * 1024);
  int ntoken = yylex();
  if_not_key(ntoken) {
    RESPONSE_ERROR('KEY');
    return 1;
  }
  key_ = strdup(yytext);
  if (yylex()) {
    RESPONSE_ERROR(empty);
    return 1;
  }
  file_getfile(key_);
  response_getfile(key_);

  free(key_); 
  return 0;    
}


/* -------------- ACCEPT SOCKET FROM AN IPv6------------------- */

/* char *s = NULL; */
/* switch(res->sa_family) { */
/*     case AF_INET: { */
/*         struct sockaddr_in *addr_in = (struct sockaddr_in *)res; */
/*         s = malloc(INET_ADDRSTRLEN); */
/*         inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN); */
/*         break; */
/*     } */
/*     case AF_INET6: { */
/*         struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res; */
/*         s = malloc(INET6_ADDRSTRLEN); */
/*         inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN); */
/*         break; */
/*     } */
/*     default: */
/*         break; */
/* } */
/* printf("IP address: %s\n", s); */
/* free(s); */



