#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <errno.h>
#include <assert.h>

#define ERROR_MSG(x)  fprintf(stderr, "%s\n", (x))

#define BUF_SIZE    1024
#define MAX_TASKS   10

typedef struct program_tag {
    char**   args;      // array of pointers to arguments
    int      num_args;  // number of arguments
    int	     pid;	// process ID of this program
    int      fd_in;     // FD for stdin
    int      fd_out;    // FD for stdout
} Program;


/* start a program
 * The informaton about one or more programs is passed in.
 * parameters:
 *      progs:  array of pointers to Program.
 *      total:  the number of valid programs in progs.
 *      cur:    the one that needs to be started.
 *cela
 *  You need fork and then turn child into the program.
 *  Create pipes or use the pipes already created.
 *  Close pipes that are not used.
 */
void start_program(Program *progs, int total, int cur)
{
    // TODO
    prepare_pipes(progs, total);

    int count = cur - 1;

    if (count == 0) {
        int proc1 = fork();

        if (proc1 == 0) {
            dup2(progs[count].fd_in, 0);
            close(progs[count].fd_in);
            close(progs[count].fd_out);
            close(0);
        } else {
            execvp(progs[count].args[0], progs[count].args);
            wait_on_program(progs[count]);
        }
    }
    if (count == total - 1) {
        int proc2 = fork();

        if (proc2 == 0) {
            close(0);
            dup2(progs[count].fd_out, 1);
            close(progs[count].fd_in);
            close(progs[count].fd_out);
            close(1);
        } else {
            execvp(progs[count].args[0], progs[count].args);
            wait_on_program(progs[count]);
        }

    }
    if (count < total && count > 0){
        int proc3 = fork();

        if (proc3 == 0) {
            close(0);
            close(1);
            dup2(progs[count].fd_out, progs[count + 1].fd_in);
            close(progs[count].fd_in);
            close(progs[count].fd_out);
            close(progs[count + 1].fd_out);
            close(progs[count + 1].fd_in);
        } else {
            execvp(progs[count].args[0], progs[count].args);
            wait_on_program(progs[count]);
        }

    }
}

/* Wait on a program.
 */
int wait_on_program(Program *prog)
{
    // TODO
    int status;
    int rv = waitpid(prog->pid,&status,0);
    return rv;
}


/* create pipes to be used for communication
 * between processes.
 * you can create all pipes here.
 * Or you can create pipes when they are needed in start_program().
 * If you decided to create pipes in start_program(),
 * Leave this function empty.
 */
void prepare_pipes(Program *prog, int num_programs)
{
    // TODO
    int fd[2];

    for(int i = 0; i < num_programs; i ++){
        int pi = pipe(fd);

        if (pi != 0) {
            perror(strerror(errno));
            exit(1);
        }

        prog[i].fd_out = fd[1];
        prog[i + 1].fd_in = fd[0];
    }
}

/* clean up for all programs.
 */
void cleanup_programs(Program *prog, int num_programs)
{
    // TODO
    for(int i = 0; i < num_programs; i++){
        free(prog[i].args);
    }
}


/*********************************************************/
/* Do not change code below                              */
/*********************************************************/
void init_program(Program *prog, int na)
{
    // allocate memory for array of arguments
    prog->args = malloc(na * sizeof(char *));
    assert(prog->args != NULL);
    prog->num_args = 0;

    prog->pid = prog->fd_in = prog->fd_out =  -1;
}

int main(int argc, char **argv)
{
    Program progs[MAX_TASKS];
    int     num_programs;

    if (argc <= 1) {
        ERROR_MSG("Specify at least one program to run. Multiple programs are separated by --");
        exit(-1);
    }

    fprintf(stderr, "Parent started. pid=%d\n", getpid());

    // Prepare programs and their arguments
    num_programs = 0;
    int     cur_arg = 1;
    while (cur_arg < argc) {

        init_program(&progs[num_programs], argc);

        int     ia;
        for (ia = 0; cur_arg < argc; cur_arg ++) {
            if (!strcmp(argv[cur_arg], "--")) {
                if (num_programs == MAX_TASKS -  1) {
                    ERROR_MSG("Too many programs.");
                    exit(-1);
                }
                if (cur_arg + 1 == argc) {
                    ERROR_MSG("The last program is empty.");
                    exit(-1);
                }
                cur_arg ++;
                break;
            }
            progs[num_programs].args[ia++] = argv[cur_arg];
        }
        if (ia == 0) {
            ERROR_MSG("Empty program.");
            exit(-1);
        }
        progs[num_programs].args[ia] = NULL;
        progs[num_programs].num_args = ia;
        num_programs ++;
    }

    // Prepare pipes
    prepare_pipes(progs, num_programs);

    // spawn children
    for (int i = 0; i < num_programs; i ++) {
        start_program(progs, num_programs, i);
    }

    // wait for children
    for (int i = 0; i < num_programs; i ++) {
        fprintf(stderr, "Waiting for child %2d (%5d)...\n", i, progs[i].pid);
        wait_on_program(&progs[i]);
    }

    // cleanup
    cleanup_programs(progs, num_programs);

    fprintf(stderr, "Everything is good.\n");
    return 0;
}
