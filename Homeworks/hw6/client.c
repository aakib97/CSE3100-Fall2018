#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include "command.h"

void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

void doLSCommand(int sid);
void doLLSCommand(int sid);
void doSIZECommand(int sid);
void doLSIZECommand(int sid);
void doExitCommand(int sid);
void doGETCommand(int sid);
void doMGETCommand(int sid);
void doPUTCommand(int sid);



int main(int argc,char* argv[]) {
   // Create a socket
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in srv;
   struct hostent *server;

    srv.sin_family = AF_INET;

   if (argc != 0) {
       if (argc == 2) {
           server = gethostbyname(argv[0]);
           srv.sin_port = htons(*(uint16_t *) argv[1]);
       }
       if (argc == 1){
           server = gethostbyname(argv[0]);
           srv.sin_port = htons(8080);
       }
   }

   server = gethostbyname("localhost");
   srv.sin_port = htons(8080);

   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);
   int done = 0;
   do {
      char opcode[32];
      scanf("%s255s",opcode);
      if (strncmp(opcode,"lsize",5) == 0) {
          doLSIZECommand(sid);
      } else if (strncmp(opcode,"ls",2) == 0) {
         doLSCommand(sid);
      } else if (strncmp(opcode,"get",3)==0) {
         doGETCommand(sid);
      } else if (strncmp(opcode,"put",3)==0) {
         doPUTCommand(sid);
      } else if (strncmp(opcode,"lls",3) == 0) {
          doLLSCommand(sid);
      } else if (strncmp(opcode,"size",4) == 0) {
          doSIZECommand(sid);
      } else if (strncmp(opcode,"mget",4) == 0) {
          doMGETCommand(sid);
      } else if (strncmp(opcode,"exit",4) == 0) {
         doExitCommand(sid);
         done = 1;
      }
   } while(!done);

   return 0;
}

void doLSCommand(int sid) {
   Command c;
   Payload p;
   int status;

   c.code = htonl(CC_LS);
   c.arg[0] = 0;
   status = send(sid,&c,sizeof(c),0);checkError(status,__LINE__);
   status = recv(sid,&p,sizeof(p),0);checkError(status,__LINE__);
   p.code = ntohl(p.code);
   p.length = ntohl(p.length);
   int rec  = 0,rem = p.length;
   char* buf = malloc(sizeof(char)*p.length);
   while (rem != 0) {
      int nbrecv = recv(sid,buf+rec,rem,0);checkError(status,__LINE__);
      rec += nbrecv;
      rem -= nbrecv;
   }
   if(p.code == PL_TXT)
      printf("Got: [\n%s]\n",buf);
   else {
      printf("Unexpected payload: %d\n",p.code);
   }
   free(buf);
}

void doLLSCommand(int sid) {
    Payload p;
    char* msg = makeFileList(".");
    p.code = htonl(PL_TXT);
    p.length = htonl(strlen(msg)+1);
    int status = send(sid,&p,sizeof(Payload),0);checkError(status,__LINE__);
    int rem = strlen(msg)+1,sent = 0;
    while (rem != 0) {
        status = send(sid,msg+sent,rem,0);
        rem -= status;
        sent += status;
    }
    if(p.code == PL_TXT)
        printf("Got: [\n%s]\n",msg);
    else {
        printf("Unexpected payload: %d\n",p.code);
    }
    free(msg);
}

void doGETCommand(int sid) {
   Command c;
   Payload p;
   int status;

   c.code = htonl(CC_GET);
   printf("Give a filename:");
   char fName[256];
   scanf("%s255s",fName);
   strncpy(c.arg,fName,255);
   c.arg[255] = 0;
   status = send(sid,&c,sizeof(c),0);checkError(status,__LINE__);
   status = recv(sid,&p,sizeof(p),0);checkError(status,__LINE__);
   p.code = ntohl(p.code);
   p.length = ntohl(p.length);
   receiveFileOverSocket(sid,c.arg,".download",p.length);
   printf("transfer done\n");
}

void doMGETCommand(int sid) {
    Command c;
    Payload p;
    int status;

    c.code = htonl(CC_GET);

    char fName[256];
    printf("Give one or more filenames:");
    scanf("\t%[^'\n']255s",fName);

    char *ptr = strtok(fName, "\t");

    while (ptr) {
        strncpy(c.arg, ptr, 255);
        c.arg[255] = 0;

        status = send(sid, &c, sizeof(c), 0);
        checkError(status, __LINE__);
        status = recv(sid, &p, sizeof(p), 0);
        checkError(status, __LINE__);

        p.code = ntohl(p.code);
        p.length = ntohl(p.length);
        receiveFileOverSocket(sid, c.arg, ".download", p.length);
        printf("transfer done\n");
        ptr = strtok(NULL, "\t");
    }
    return;
}

void doSIZECommand(int sid) {
    Command c;
    int msg;
    int status;

    c.code = htonl(CC_SIZE);
    printf("Give a filename:");
    char fName[256];
    scanf("%s255s",fName);
    strncpy(c.arg,fName,255);
    c.arg[255] = 0;
    status = send(sid,&c,sizeof(c),0);checkError(status,__LINE__);
    status = recv(sid,&msg,sizeof(msg),0);checkError(status,__LINE__);
    printf("filesize: %d\n", msg);

}

void doLSIZECommand(int sid) {
    Command c;
    int status;

    c.code = htonl(CC_LSIZE);
    printf("Give a local filename:");
    char fName[256];
    scanf("%s255s",fName);
    strncpy(c.arg,fName,255);
    int fileSize = getFileSize(c.arg);
    status = send(sid,&fileSize,sizeof(fileSize),0);checkError(status,__LINE__);
    printf("Filesize [%d] sent\n",fileSize);
}

void doPUTCommand(int sid) {
   Command c;
   Payload p;
   int status;

   c.code = htonl(CC_PUT);
   printf("Give a local filename:");
   char fName[256];
   scanf("%s255s",fName);
   strncpy(c.arg,fName,255);
   c.arg[255] = 0;
   status = send(sid,&c,sizeof(c),0);checkError(status,__LINE__);
   int fs = getFileSize(c.arg);
   p.code   = ntohl(PL_FILE);
   p.length = ntohl(fs);
   status = send(sid,&p,sizeof(p),0);checkError(status,__LINE__);
   sendFileOverSocket(c.arg,sid);
   printf("transfer done\n");
}

void doExitCommand(int sid) {
   Command c;
   //Payload p;
   int status;

   c.code = htonl(CC_EXIT);
   c.arg[0] = 0;
   status = send(sid,&c,sizeof(c),0);checkError(status,__LINE__);
}
