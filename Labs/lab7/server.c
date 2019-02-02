#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <dirent.h>
#include <time.h>

void checkError(int status)
{
    if (status < 0) {
        printf("socket error: [%s]\n",strerror(errno));
        exit(-1);
    }
}


int main(int argc,char* argv[]) {
    int sid = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3100);
    addr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(sid, (struct sockaddr *) &addr, sizeof(addr));
    checkError(status);

    status = listen(sid, 10);
    checkError(status);

    int total = 0;
    int mesg_recv = 0;
    char mesg[1024];
    char input[1024];
    char copy[1024];

    while (1) {
        struct sockaddr_in client;
        socklen_t clientSize;

        int chatSocket = accept(sid, (struct sockaddr *) &client, &clientSize);
        checkError(chatSocket);

        printf("We accepted a socket: %d\n",chatSocket);

        pid_t child = fork();
        if (child == 0) {

            snprintf(mesg, sizeof(mesg), "%d\n", total);

            int status = send(chatSocket, mesg, sizeof(mesg), 0);
            checkError(status);

            unsigned int done = 0;

            while (!done) {

                int len = recv(chatSocket, input, sizeof(input), 0);
                checkError(len);

                printf("We received a message: %s\n",input);

                if (strlen(input) > 30) {
                    snprintf(mesg, sizeof(mesg), "%s %d\n", "Error LongLine ", total);

                    int status = send(chatSocket, mesg, sizeof(mesg), 0);
                    checkError(status);
                }
                else if (atoi(input) != 0 || input == "0") {
                    sscanf(input, "%d", &mesg_recv);
                    total += mesg_recv;
                    snprintf(mesg, sizeof(mesg), "%d\n", total);

                    int status = send(chatSocket, mesg, sizeof(mesg), 0);
                    checkError(status);
                }
                else if (strcmp(input, "exit") != 0){
                    snprintf(mesg, sizeof(mesg), "%s %d\n", "Error NaN ", total);

                    int status = send(chatSocket, mesg, sizeof(mesg), 0);
                    checkError(status);
                } else {
                    snprintf(mesg, sizeof(mesg), "%s %d\n", "Bye ", total);

                    int len = send(chatSocket, mesg, sizeof(mesg), 0);
                    checkError(len);

                    done = 1;
                }

            }
            close(chatSocket);
            return -1;

        } else if (child > 0) {
            close(chatSocket);
            int status = 0;
            pid_t deadChild;
            do {
                deadChild = waitpid(0, &status, WNOHANG);
                checkError(deadChild);
                if (deadChild > 0)
                    printf("Reaped %d\n", deadChild);
            } while (deadChild > 0);
        }
        close(sid);
        return 0;
    }
}

