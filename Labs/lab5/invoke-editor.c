#define    _POSIX_C_SOURCE  1

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    // change 'vi' to your favorite editor
    char    *cmd = "vi";
    char    *filename = "a.txt";

    if (argc >= 2)
        filename = argv[1];

    // TODO
    pid_t value = fork();
    int status;

    if (value > 0){
        FILE *file;
        char line[1000];

        file = fopen(filename, "r");

        while(!feof(file)){
            fgets(line, 1000, file);
            if (line[0] == '#'){
                puts(line);
            }
        }

        fclose(file);
    }
    else{
        printf("pid=%d exited=%d exitstatus=%d\n",getpid(), WIFEXITED(status), WEXITSTATUS(status));
    }

    execl(filename, cmd);
    return 0;
}
