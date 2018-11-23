#include "header.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "stdio.h"

#define CMD_LEN  48
#define MAX_CMDS 10


void init_cmd_arr(char**);
void input_to_cmds(char*, char**);
char* getLine(FILE* stream);

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
    // char *argv[*cmd_cnt+1];
    int count = 0;

    argv[0] = malloc(2);
    strcpy(argv[0], "");

    for (int i=1; i<cmd_cnt; i++) {
        argv[i] = malloc(strlen(cmds[i]));
        strcpy(argv[i], cmds[i]);
        count++;
    }
    argv[cmd_cnt] = NULL;
}

int main(int argc, char const *argv[])
{
    char *cmds[4];
    char *msg = ">>naiveinterface";
    char input[256];
    bool valid;

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

            // for(int i=0; i<cmd_cnt; i++){
            //     printf("%s ", cmds[i]);
            // } printf("\n");
            
            // exit cleanly
            if (strcmp(cmds[0], "quit") == 0) {
                exit(EXIT_SUCCESS);
            }

            // create program execution
            else if (strcmp(cmds[0], "create") == 0) {
                printf("[create program]\n");
            }

            // fileconverter execution
            else if (strcmp(cmds[0], "fileconverter") == 0) {

                // get commands and put them into an arg array
                // will be one larger - need to pass in "dummy" arg in place of program name
                char *argv[cmd_cnt+1];
                get_cmds(cmd_cnt, cmds, argv);

                // create new process for fileconverter to use
                int status;
                pid_t fileconverter = fork();

                // execute if child
                if (fileconverter == 0) execv("fileconverter", argv);

                // wait on child, print status when complete
                waitpid(fileconverter, &status, 0);
                // printf(">>naiveinterface: fileconverter processing complete.\n");
            }

            // indexer execution
            else if (strcmp(cmds[0], "indexer") == 0){
                // get commands and put them into an arg array
                // will be one larger - need to pass in "dummy" arg in place of program name
                char *argv[cmd_cnt+1];
                get_cmds(cmd_cnt, cmds, argv);

                // create new process for indexer to use
                int status;
                pid_t indexer = fork();

                // execute if child
                if (indexer == 0) execv("indexer", argv);

                // wait on child, print status when complete
                waitpid(indexer, &status, 0);
                // printf(">>naiveinterface: indexer processing complete.\n");
            }

            // srchindx execution
            else if (strcmp(cmds[0], "srchindx") == 0) {
                printf("[srchindx program]\n");
            }
            // unknown command
            else {
                valid = false;
                printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);
            }
            free_cmds(cmds);
            // for (int i=0; i<=cmd_cnt; i++) memset(cmds[i], 0, sizeof(CMD_LEN));
            cmd_cnt = 0;
            // printf("Normal end.\n");
        }
        
    }
    return 0;
}
