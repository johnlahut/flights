#include "header.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    
    if (argc < 2 || argc > 3) 
        printf(">>indexer: supported usage is indexer [inverted-index file name] directory.\n");

    else {
        DIR* dp;
        struct dirent* dir;

        char dir_name[128];
        char file_name[128];

        bool valid = true;
        

        dp = opendir(argv[1]);

        // first arg is not directory, and the user passed in a second arg
        if(dp == NULL && argc == 3) {
            dp = opendir(argv[2]);

            // second arg is not a directory either
            if (dp == NULL) {
                valid = false;
                perror(">>indexer");
            }

            // second arg is a valid dir -> first is a filename
            else {
                strcpy(file_name, argv[1]);
                strcpy(dir_name, argv[2]);
            }
        }

        // first arg is not a directory and no second arg
        else if (dp == NULL || argc != 2) {
            valid = false;
            perror(">>indexer");
        }

        // only directory
        else if (argc == 2) {
            strcpy(dir_name, argv[1]);
            strcpy(file_name, "invind.txt");
        }

        // valid args
        if (valid) {
            FILE* fp;

            // switch to passed in dir, initialize data struct
            chdir(dir_name);
            init_table();
            
            // read all files in directory
            while((dir = readdir(dp)) != NULL) {

                // ignore curent dir, parent dir, and current file name
                // this would be caught further down, but this suppresses output
                if (!strcmp(dir->d_name, ".")) continue;
                if (!strcmp(dir->d_name, "..")) continue;
                if (!strcmp(dir->d_name, file_name)) continue;

                // open file, check to see if regular file or null
                fp = fopen(dir->d_name, "r");
                if (fp == NULL || dir->d_type != DT_REG) {

                    // cant open file, generate error msg
                    if (fp == NULL) {
                        char *err_msg = malloc(sizeof(dir->d_type) + 10);
                        strcpy(err_msg, ">>indexer ");
                        strcat(err_msg, dir->d_name);
                        perror(err_msg);
                    }

                    // not a regular file
                    else {
                        fclose(fp);
                        printf(">>indexer: skipping non-file [%s]\n", dir->d_name);
                    }
                }

                // good to go, read the file
                else {
                    read_files(dir->d_name);
                    fclose(fp);
                }
            }
            closedir(dp);
        }
        write_file(file_name);
        destroy_table();

    }
    return 0;
}
