
#include "./include/tracker.h"

#define true 0
#define false 1

#define MAXCONNECTIONS 5
#define MAXMESGSIZE 6000
#define MAXBUFFSIZE 6000
#define MAXNUMFILES 100

int serv_fd; 
char tmprecvBuff[MAXMESGSIZE];




int getconfig(){
  int port=0;  
  char tmp [10]; 
  FILE *config=fopen("tracker.config","r+"); 
  if(config !=NULL)
    fscanf(config, "%s %d", tmp, &port); 
  return port; 
}
//Strore info about connect file desciptor and ipNum
 struct session conRec[MAXCONNECTIONS];

/* ------------------ INITIALIZE DATA_BASE -------------------------- */
void init_db(void) 
{
  struct file * emptyRec ;
  emptyRec =  file_malloc();
  file_set_key(emptyRec, "");
  file_set_length(emptyRec, "");
  HASH_ADD_KEYPTR(hh, ht_files,emptyRec->key, strlen(emptyRec->key) , emptyRec); 
}

/* ----------------- DELETE DATA AFTER A CLIENTS CLOSE THE CONNECTION --- */

void delete_from_database(struct session *s)
{
  struct file *f;
  struct peer *p;
  int count;
  for (f=ht_files; f != NULL; f = f->hh.next) {
    DL_COUNT(f->dl_peers,p, count);
    if (count == 1 && (strcmp(f->dl_peers->session->ipAddr, s->ipAddr) == 0) && (f->dl_peers->session->portNum == s->portNum)){
      /* DL_DELETE(f->dl_peers->dl_names, n); */
      /* DL_DELETE(f->dl_peers, p); */
      HASH_DEL(ht_files, f);
      /* file_free(f); */
    }
    else {
      DL_FOREACH(f->dl_peers, p) {
	if ((strcmp(p->session->ipAddr, s->ipAddr) == 0) && (p->session->portNum ==  s->portNum)) {
	  /* DL_DELETE(f->dl_peers->dl_names, n); */
	  DL_DELETE(f->dl_peers, p);
	  /* peer_free(p); */
	}
      }
    }
  }

  if (delete_client(s->connectfd))
    printf("client deleted\n");
}


/* ------------  FUNCTIONS TO MANIPULATE CLIENT SESSION  -------------------*/

void print_num_active_clients(void){
  int count, i;
  count = 0;
  for(i=0;i<MAXCONNECTIONS;i++){
    if(conRec[i].connectfd >0)
      count++; 
  }
  printf("Total active clients = %d\n",count);
}

/* int check_if_exists(int fd) { */
/*   int i = 0; */
/*   for(i=0;i<MAXCONNECTIONS;i++){ */
/*     if(fd == conRec[i].connectfd) */
/*       return true;  */
/*   } */
/*   return false; */
/* } */

int check_session_exists(struct session *tmp){
  int i, ret;
  ret = false;
  for(i=0;i<MAXCONNECTIONS;i++){
    pthread_mutex_lock(&lockconRec);
    if(conRec[i].connectfd >0){
    
      if((strncmp(conRec[i].ipAddr, tmp->ipAddr,strlen(tmp->ipAddr))==0) && (conRec[i].connectfd == tmp->connectfd)) {
	ret = true;
	break; 
      }
    
    }
    pthread_mutex_unlock(&lockconRec);
  }
  pthread_mutex_unlock(&lockconRec);
  return ret;
}

int delete_client(int fd)
{
  int i = 0;
  for (i=0; i<MAXCONNECTIONS; i++)
    if (conRec[i].connectfd == fd)
      return 1 ;
  return 0;
}

void register_session(struct session * cli)
{
  int i;
  if (cli == NULL)
    return;
  for (i=0; i < MAXCONNECTIONS; i++){
    pthread_mutex_lock(&lockconRec);
    if (conRec[i].connectfd <= 0){
      conRec[i].connectfd = cli->connectfd;
      strncpy(conRec[i].ipAddr, cli->ipAddr, sizeof(cli->ipAddr));
      conRec[i].portNum = cli->portNum ;
     
      break;
    }
    pthread_mutex_unlock(&lockconRec);
  }
  pthread_mutex_unlock(&lockconRec);
}



/* ----------------------- SETUP SERVER CONNECTION ---------------------- */

int setup_server(struct sockaddr_in server, int port_num) {
  int sock_fd =  socket(AF_INET , SOCK_STREAM , 0);
  if (sock_fd < 0) {
      printf("Could not create socket\n");
      exit(1);
    }
  puts("Socket created");
     
  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( port_num );
    
  //Bind
  if( bind(sock_fd,(struct sockaddr *)&server , sizeof(server)) < 0)  {
    perror("bind failed. Error\n");
    exit(1);
  }
  puts("bind done");
  //Listen
  if (listen(sock_fd , MAXCONNECTIONS) < 0) {
    perror("listen failer. Error\n");
    exit(1);
  }
  return sock_fd;
}

/* ---------------- HANDLER LINKED WITH THREAD THAT TREATS NEW CONNECTION ------ */
void *connection_handler(void * session)
{
  //buffer to send and receive the file list
  char sendMsg[MAXMESGSIZE]; 
 
  char recvBuff[MAXMESGSIZE];

  int recvBytes;
  
  int  exists, notValidCmd;
  FILE *log_fd;
  struct session tmpSession = *(struct session *) session;

  
 
  printf("fd thread %d\n", tmpSession.connectfd);
  


 
  //Get the message from the client
  while(1) {
    
    memset(recvBuff, 0, sizeof(recvBuff)); 
    recvBytes=recv(tmpSession.connectfd, recvBuff, MAXMESGSIZE-1,0); 
   
    if (recvBytes>=0) {
    
      /* Information about received message */
      printf("Received %d bytes from thread %d\n", recvBytes, tmpSession.connectfd);
      printf("%s\n\n",recvBuff); 
      //check if client exists    
      exists = check_session_exists(&tmpSession);
      
      notValidCmd = scan_request(sendMsg, recvBuff, recvBytes, &tmpSession,exists);
  
     
      //client connection to the server for the first time
      if (exists == false && notValidCmd == 0){
	//Register the client to activeConnectionsList
	printf("client is registred\n");
	register_session(&tmpSession);	 
	
      }
      if (notValidCmd == 1){
	log_fd = fopen("error_log.txt", "w");
	fprintf(log_fd,"from client connected to listen fd:%d, ip:%s, port:%d\n\t request:%s\n",tmpSession.connectfd, tmpSession.ipAddr, tmpSession.portNum, sendMsg);
	
	write(tmpSession.connectfd, sendMsg, strlen(sendMsg)); 
	continue;
      }
      log_fd = fopen("log.txt", "w");
      fprintf(log_fd,"from client connected to listen fd:%d, ip:%s, port:%d\n\t request:%s\n",tmpSession.connectfd, tmpSession.ipAddr, tmpSession.portNum, sendMsg);
      
      /* Send the message back to client */
      write(tmpSession.connectfd, sendMsg, strlen(sendMsg));
      printf("%s\n", sendMsg);
      //clear the message buffer
     
    
      fclose(log_fd);
    }
    else if(recvBytes==-1)

	   break;
 
  } 
  delete_from_database(&tmpSession );
 return NULL; 
 }
/* ---------------------CATCH SIGNALS BEFORE CLOSING CONNECTION ----- */
 void signalHandler() 
 { 
   close(serv_fd); 
   exit(0); 
 } 


/* -------------- TRACKER: MULTI-THREAD P2P --------------------- */
int main(int argc , char *argv[])
{
  //variables declaration:
  int  serv_fd, cli_fd , sockLen, i;
  struct sockaddr_in servSock , cliSock;
  pthread_t *thread_id =malloc(sizeof(pthread_t)); 
  int port_num = getconfig(); 
  struct session cliInfo;
 //initialize data base:
   pthread_mutex_init(&lock,NULL);
  init_db(); 
  

  //Interrupt signal 
   signal (SIGINT, signalHandler); 
   signal (SIGQUIT, signalHandler); 
   signal (SIGSEGV, signalHandler);   

  sockLen= sizeof(struct sockaddr_in);
  memset(&servSock, '\0', sockLen);
  memset(&cliSock, '\0', sockLen);
  

  for (i = 0; i < MAXCONNECTIONS; i++) {
    conRec[i].connectfd = -1;
    memset(conRec[i].ipAddr, '\0', sizeof(conRec[i].ipAddr));
    conRec[i].portNum = -1;
  }

  //Create socket:
  serv_fd = setup_server(servSock, port_num);
  printf("listen fd = %d\n", serv_fd);


 
  //Accept and incoming connection
  printf("----------------------------------\n");
  printf("Waiting for incoming connections...\n");
 
  do{
    cli_fd = accept(serv_fd, (struct sockaddr *)&cliSock, (socklen_t*)&sockLen);
    if (cli_fd < 0) {
    perror("accept connection with tracker failed");
    /* return 1; */
    }
 
    //full in fields new session
    cliInfo.connectfd = cli_fd;
    strcpy(cliInfo.ipAddr, inet_ntoa(cliSock.sin_addr));
    cliInfo.portNum = ntohs(cliSock.sin_port);
    printf("thread num %d\n", cliInfo.connectfd);
    printf("Got connection from client: ip %s, port %d\n", cliInfo.ipAddr, cliInfo.portNum);
    memset (tmprecvBuff, 0, sizeof (tmprecvBuff));
     if( pthread_create( thread_id , NULL ,  connection_handler , (void*) &cliInfo) < 0) {
      perror("could not create thread");
      return 1;
    }
    
  printf("Handler assigned\n");
  }while(1);
  
  //Now join the thread , so that we dont terminate before the thread
  pthread_join( *thread_id , NULL);
    
  
  pthread_exit(NULL);      
  return EXIT_SUCCESS;
}

