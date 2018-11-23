#include "header.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "stdio.h"

#define CMD_LEN    48
#define MAX_CMDS   10
#define MAX_INPUT 256


void init_cmd_arr(char**);
void get_cmds(int, char**, char**);
void free_cmds(char**);
void exec_prog(char*, char**, int);
char* getLine(FILE* stream);

int main(int argc, char const *argv[])
{
    char *cmds[MAX_CMDS];
    char input[MAX_INPUT];
    // bool valid;

    // no argument passed in, normal run time
    if (argc == 1) {

        // enter loop, executing commands until quit is given
        while (1) {
            printf(">>naiveinterface: ");

            // getchar reads in the new line, prevents "infinite" loop 
            scanf("%[^\n]", input);
            getchar();

            // initialize array that will hold commands
            init_cmd_arr(cmds);
            
            // parse the input commands, append to command array, count number of commands
            int cmd_cnt, cursor = 0;
            for (int i=0; i<strlen(input); i++) {
                if (input[i] == ' ') {
                    cmds[cmd_cnt][cursor] = '\0';
                    cursor = 0;
                    cmd_cnt++;
                }
                else cmds[cmd_cnt][cursor++] = input[i];
            }

            // one left over
            cmds[cmd_cnt][cursor] = '\0';
            cmd_cnt++;
            
            // exit cleanly
            if (strcmp(cmds[0], "quit") == 0)
                exit(EXIT_SUCCESS);

            // run one of create indexer fileconverter or srchindx programs 
            else if (strcmp(cmds[0], "create") == 0 || strcmp(cmds[0], "fileconverter") == 0 || 
                     strcmp(cmds[0], "indexer") == 0 || strcmp(cmds[0], "srchindx") == 0)
                        exec_prog(cmds[0], cmds, cmd_cnt);

            // unknown command
            else
                printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);

            free_cmds(cmds);
            cmd_cnt = 0;
        }
        
    }
    return 0;
}

char* getLine(FILE* stream) {

    char* line = malloc(CMD_LEN);
    fscanf(stream, "%[^\n]s", line);
    return line;
}

void init_cmd_arr(char** cmds) {
    for(int i=0; i<MAX_CMDS; i++) {
        cmds[i] = (char*) malloc(CMD_LEN);
    }
}

void free_cmds(char** cmds) {
    for (int i=0; i<MAX_CMDS; i++) {
        free(cmds[i]);
    }
}

void get_cmds(int cmd_cnt, char** cmds, char** argv) {
    int count = 0;

    // first argument is dummy arg - exec functions do not pass in prog name
    argv[0] = malloc(2);
    strcpy(argv[0], "");

    // copy each command from cmds array into argv, count the commands
    for (int i=1; i<cmd_cnt; i++) {
        argv[i] = malloc(strlen(cmds[i]));
        strcpy(argv[i], cmds[i]);
        count++;
    }

    // last arg always should be NULL
    argv[cmd_cnt] = NULL;
}

void exec_prog(char* prog_name, char** cmds, int cmd_cnt) {
    // get commands and put them into an arg array
    // will be one larger - need to pass in "dummy" arg in place of program name
    char *argv[cmd_cnt+1];
    get_cmds(cmd_cnt, cmds, argv);

    // create new process for indexer to use
    int status;
    pid_t prog = fork();

    // execute if child
    if (prog == 0) {
        execv(prog_name, argv);

        // should never get here, child process is 'consumed' by execv
        printf(">>naiveinterface: %s failed to execute.\n", prog_name);
    }

    // wait on child
    waitpid(prog, &status, 0);
}