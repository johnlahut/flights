#define _XOPEN_SOURCE 500

#include "header.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "stdlib.h"
#include "stdio.h"
#include "fcntl.h"
#include "ftw.h"

#define CMD_LEN    48
#define MAX_CMDS   10
#define MAX_INPUT 256


void init_cmd_arr(char**);
void get_cmds(int, char**, char**);
void free_cmds(char**);
void exec_prog(char*, char**, int, char*);
static int delete(const char *name, const struct stat *status, int type);
char* getLine(FILE* stream);


int main(int argc, char const *argv[])
{
    char *cmds[MAX_CMDS];
    char input[MAX_INPUT];
    char output[CMD_LEN];
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
            int cmd_cnt = 0;
            int cursor = 0;
            int redirect = 0;
            int j = 0;
            for (int i=0; i<strlen(input); i++) {
                if (input[i] == ' ' && redirect == 0) {
                    cmds[cmd_cnt][cursor] = '\0';
                    cursor = 0;
                    cmd_cnt++;
                }
                else if(input[i] == '>')
                {
                    redirect = 1;
                }
                else if(redirect == 0) {
                    cmds[cmd_cnt][cursor++] = input[i];
                }
                else if(redirect == 1 && input[i] != ' ' && input[i] != '\n') {
                    output[j] = input[i];
                    j++;
                }
                
                
            }
            
            if(redirect == 0) {

                // one left over
                cmds[cmd_cnt][cursor] = '\0';
                cmd_cnt++;
            
                // exit cleanly
                if (strcmp(cmds[0], "quit") == 0)
                {
                    //asks user if they want to quit.
                    char choice;
                    printf("Do you really want to quit? All created files will be lost. Y/N\n");
                    scanf("%c", &choice);
                    if(choice == 'Y' || choice == 'y')
                    {
                        //calls delete, which deletes any created files in the current directory
                        if(ftw(".", delete, 1) == -1)
                        {
                            perror("FTW()");
                        }
                        exit(EXIT_SUCCESS);
                    }
                    getchar();

                }

                // run one of create indexer fileconverter or srchindx programs 
                else if (strcmp(cmds[0], "create") == 0 || strcmp(cmds[0], "fileconverter") == 0 || 
                        strcmp(cmds[0], "indexer") == 0 || strcmp(cmds[0], "srchindx") == 0)
                        {
                            exec_prog(cmds[0], cmds, cmd_cnt, "");
                        }


                // unknown command
                else
                {
                    printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);
                }


                free_cmds(cmds);
                cmd_cnt = 0;
            }
            else {
                
                // one left over
                cmds[cmd_cnt][cursor] = '\0';
                cmd_cnt++;
                //only runs search index for redirection
                if(strcmp(cmds[0], "srchindx") == 0)
                {
                    exec_prog(cmds[0], cmds, cmd_cnt, output);
                }
                else
                {
                    printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);
                }
                
                free_cmds(cmds);
                cmd_cnt = 0;
            }

        }
        
    }
    else if(argc == 2) {
        
        //tries to open script file
        FILE* fp;
        if((fp = fopen(argv[1], "r")) == NULL)
        {
            perror("Failure to open file.");
        }
        
        else {
    
            //file is passed as an argument.
            while(1)
            {
            // initialize array that will hold commands
            init_cmd_arr(cmds);
            
            strcpy(input, getLine(fp));
            
            // parse the input commands, append to command array, count number of commands
            // redirect is if the program is redirecting, j is to index output file char array
            int cmd_cnt = 0;
            int cursor = 0;
            int redirect = 0;
            int j = 0;
            for (int i=0; i<strlen(input); i++) {
                if (input[i] == ' ' && redirect == 0) {
                    cmds[cmd_cnt][cursor] = '\0';
                    cursor = 0;
                    cmd_cnt++;
                }
                else if(input[i] == '>')
                {
                    //redirect set to 1 if it is a redirect
                    redirect = 1;
                }
                else if(redirect == 0) {
                    cmds[cmd_cnt][cursor++] = input[i];
                }
                else if(redirect == 1 && input[i] != ' ' && input[i] != '\n') {
                    output[j] = input[i];
                    j++;
                }
                
            }
            
            //null terminates the end of the output array
            output[j-1] = '\0';
            
            if(redirect == 0) {

                // one left over
                cmds[cmd_cnt][cursor] = '\0';
                cmd_cnt++;
            
                // exit cleanly, all files are kept
                if (strcmp(cmds[0], "quit") == 0)
                {
                    exit(EXIT_SUCCESS);
                }


                // run one of create indexer fileconverter or srchindx programs 
                else if (strcmp(cmds[0], "create") == 0 || strcmp(cmds[0], "fileconverter") == 0 || 
                        strcmp(cmds[0], "indexer") == 0 || strcmp(cmds[0], "srchindx") == 0)
                        {
                            exec_prog(cmds[0], cmds, cmd_cnt, "");
                        }


                // unknown command
                else
                {
                    printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);
                }


                free_cmds(cmds);
                cmd_cnt = 0;
            }
            else {
                
                // one left over
                cmds[cmd_cnt][cursor] = '\0';
                cmd_cnt++;
                
                
                if(strcmp(cmds[0], "srchindx") == 0)
                {
                    exec_prog(cmds[0], cmds, cmd_cnt, output);
                }
                else
                {
                    printf("Unknown command '%s'. Valid commands are create fileconverter indexer srchindx quit\n", cmds[0]);
                }
                
                free_cmds(cmds);
                cmd_cnt = 0;
                
                }

            }
        }
        
        fclose(fp);
    }
    
    return 0;
}

//reads a line from a file, used for the script file
char* getLine(FILE* stream) {

    char* line = malloc(CMD_LEN);
    fscanf(stream, "%[^\n]%*c", line);
    return line;
}

//mallocs the commands array
void init_cmd_arr(char** cmds) {
    for(int i=0; i<MAX_CMDS; i++) {
        cmds[i] = (char*) malloc(CMD_LEN);
    }
}

//frees the commands array
void free_cmds(char** cmds) {
    for (int i=0; i<MAX_CMDS; i++) {
        free(cmds[i]);
    }
}

//obtains the arguments
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

//executes the program if possible
void exec_prog(char* prog_name, char** cmds, int cmd_cnt, char* output) {
    //no file to output to
    if(strlen(output) == 0) {
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
    //output file is given
    else {
        // get commands and put them into an arg array
        // will be one larger - need to pass in "dummy" arg in place of program name
        char *argv[cmd_cnt+1];
        get_cmds(cmd_cnt, cmds, argv);
           
        //try to open output file
        FILE *out;
        if((out = fopen(output, "w")) == NULL) {
            perror("Failure to open file.");
        }
        
        else {
            //stores file descriptor of the file
            int fd;
            if((fd = fileno(out)) == -1)
            {
                perror("fileno failure.");
            }
            
            //saves std output for later
            int SAVE;
            if ((SAVE = dup(1)) == -1)
            {
                perror("Dup failure");
            }
            //redirects std output to the file
            if(dup2(fd,1) == -1) {
                fclose(out);
                perror("Dup2 failure.");
            }
            
            else {
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
                //close the file and restore output to the terminal
                fclose(out);
                dup2(SAVE, 1);
            }
        }
    }

}

//ftw function that will delete all but necessary files to run the program
static int delete(const char *name, const struct stat *status, int type)
{
    //deletes all created directories
    if(type == FTW_D)
    {
        remove(name);
    }
    //deletes all files not required to run programs
    else if(type == FTW_F)
    {
        if(strcmp(name,"./interfaceFunctions.c") != 0 && strcmp(name,"./_indexer.c") != 0
            && strcmp(name,"./_fileconverter.c") != 0 && strcmp(name,"./array.c") != 0
            && strcmp(name,"./create.c") != 0 && strcmp(name,"./fileconverter.c") != 0
            && strcmp(name,"./hash.c") != 0 && strcmp(name,"./header.h") != 0
            && strcmp(name,"./indexer.c") != 0 && strcmp(name,"./list.c") != 0 
            && strcmp(name,"./main.c") != 0 && strcmp(name,"./makefile") != 0
            && strcmp(name,"./main") != 0 && strcmp(name,"./utils.c") != 0
            && strcmp(name,"./fileconverter") != 0 && strcmp(name,"./indexer") != 0
            && strcmp(name,"./create") != 0 && strcmp(name,"./naiveinterface") != 0
            && strcmp(name,"./array") != 0 && strcmp(name,"./hash") != 0
            && strcmp(name,"./list") != 0 && strcmp(name,"./utils") != 0
            && strcmp(name,"./srchindx.c") != 0 && strcmp(name,"./srchindx") != 0)
            {
                remove(name);
            }
    }
    //unlinks and deletes soft links
    else if(type == FTW_SL)
    {
        unlink(name);
        remove(name);
    }
    return 0;
}