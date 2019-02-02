#ifndef __COMMAND_H
#define __COMMAND_H


#define CC_LS	0
#define CC_LLS  1
#define CC_GET	2
#define CC_PUT  3
#define CC_EXIT 4
#define CC_SIZE 5
#define CC_LSIZE 6
#define CC_MGET 7


#define PL_TXT  0
#define PL_FILE 1

typedef struct CommantTag {
	int  code;
	char arg[256];
} Command;

typedef struct PayloadTag {
	int code;
	int length;
} Payload;

char* makeFileList(char* path);
int getFileSize(char* fName);
void sendFileOverSocket(char* fName,int chatSocket);
void receiveFileOverSocket(int sid,char* fname,char* ext,int fSize);

#endif
