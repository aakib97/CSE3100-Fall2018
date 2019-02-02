#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

#define     BUF_LEN     1024


int main(int argc, char ** argv)
{
    // change 'vim' to another editor if you like
    char * cmd_editor = "vim";
    // char    * cmd_editor = "nano";
    // char    * cmd_editor = "emacs";
    char * cmd_make = "make";
    char * cmd_find_error = "./find-error.py";
    char * fn_make = "make.out";   // output of make
    int pFD[2];    // pipe FDs

    // Do not specify a line number initially
    int linenumber = -1;

    // buffers
    char fn_c[BUF_LEN];
    char make_target[BUF_LEN];
    char* opt_linenumber[BUF_LEN];

    if (argc >= 2) {
        int len = strlen(argv[1]);
        if (len > BUF_LEN - 10) {
            fprintf(stderr, "file name or make target is too long.\n");
        }
        if (len > 2 && argv[1][len-1] == 'c' && argv[1][len-2] == '.') {
            strcpy(fn_c, argv[1]);
            strcpy(make_target, argv[1]);
            make_target[len-2] = 0;
        }
        else {
            strcpy(make_target, argv[1]);
            strcpy(fn_c, argv[1]);
            strcat(fn_c, ".c");
        }
    }
    else {
        strcpy(fn_c, "bugs.c");
        strcpy(make_target, "bugs");
    }

    // If you'd like to prepare more before getting into the loop,
    // do it here.
    // TODO
    pid_t value = fork();
    int status = 0;
    pipe(pFD);


    int done = 0;

    do {
        // invoke editor
        // invoke make
        // invoke ./find-error.py to find first error/warning
        // read the linenumber that has error/warning
        // TODO
        linenumber = argc;

        if (linenumber > 0) {
            sprintf(opt_linenumber[0], "%d", linenumber);
            strcat("+", opt_linenumber[0]);
        } else {
            sprintf(opt_linenumber[0], "%d", 0);
            strcat("+", opt_linenumber[0]);
        }

        if (value == 0) {
            char *cmd_lines[] = {opt_linenumber[0], argv[0], NULL};
            execvp(cmd_editor, cmd_lines);
            perror("execvp failed:");
            return 1;
        } else {
            wait(&status);
        }

        

        
        // ask if the user wants to continue
        char answer;
        printf("Would you like to continue?(y/n)\n");
        answer = getchar();
        if (answer == EOF || (answer != '\n'  && tolower(answer) != 'y'))
            done = 1;
        else {
            // skip to the end of the line
            while (answer != EOF && answer != '\n')
                answer = getchar();
        }
    } while (! done);

    // If you'd like to clean up after the loop,
    // do it here.
    // TODO

    return 0;
}
