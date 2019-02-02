#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>

void checkError(int status)
{
    if (status < 0) {
        printf("socket error: [%s]\n",strerror(errno));
        exit(-1);
    }
}

int main(int argc,char* argv[])
{
    int sid = socket(PF_INET,SOCK_STREAM,0);

    struct sockaddr_in srv;
    struct hostent *server = gethostbyname("localhost");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(3100);

    memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);

    int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
    checkError(status);

    char mesg_recv[1024];
    char input[1024];

    int len = recv(sid, &mesg_recv, sizeof(mesg_recv), 0);
    checkError(len);
    printf("%s", mesg_recv);

    unsigned int done = 0;

    while (!done) {
        memset(mesg_recv, 0, sizeof(mesg_recv));
        memset(input, 0, sizeof(input));

        scanf("%s", input);

        int sent = send(sid, input, sizeof(input), 0);
        checkError(sent);

        int len = recv(sid, mesg_recv, sizeof(mesg_recv), 0);
        checkError(len);

        if (strcmp(input, "Bye") == 0) {
            printf("%s", mesg_recv);
            done = 1;
        }
        else{
            printf("%s", mesg_recv);
        }
    }
    close(sid);

    return 0;
}
