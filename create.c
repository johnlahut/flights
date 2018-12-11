/*
Authors:
	James Bohrer
Date: 11.3.2018
Filename: create.c
Purpose: Creates files, directories, and links.
Project: CSI402 Final Project
*/

/*TODO: 
some extra error checking should be done, mainly with user input.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void make_file(char**);
void make_dir(char**);
void make_hard_link(char**, char**);
void make_soft_link(char**, char**);

// @author: James Bohrer
int main(int argc, char* argv[])
{
    if(argc < 3 || argc > 4)
    {
       printf(">>create: Supported usage is -f [filename] |  -d [dirpath] |  -h [oldname] [linkname] | -s [oldname] [linkname].\n");
    }
   
   //-f for file
    else if(strcmp(argv[1], "-f") == 0)
    {
        make_file(&argv[2]);
    }
    
    //-d for directory
    else if(strcmp(argv[1], "-d") == 0)
    {
        make_dir(&argv[2]);
    }
    
    //-h for hard link
    else if(strcmp(argv[1], "-h") == 0)
    {
        make_hard_link(&argv[2], &argv[3]);
    }
    
    //-s for soft link
    else if(strcmp(argv[1], "-s") == 0)
    {
        make_soft_link(&argv[2], &argv[3]);
    }
    
    //any other flag or argument in argv[1] causes an error
    else
    {
        printf(">>create: Unknown command: %s. Supported usage is -f [filename] |  -d [dirpath] |  -h [oldname] [linkname] | -s [oldname] [linkname].\n", argv[1]);
    }
    
    return 0;
}

//creates a new file with specified pathname and 0640 permissions.
// @author: James Bohrer
void make_file(char **path)
{
    FILE *fp;
    if((fp = fopen(*path, "w+")) == NULL)
    {
        perror(">>create:");
    }
    else {
        chmod(*path, 0640);
        fclose(fp);
    }
}

//creates a directory at given pathname with 0750 permissions.
// @author: James Bohrer
void make_dir(char **path)
{
    if(mkdir(*path, 0750) == -1)
    {
        perror(">>create:");
    }
}

//creates a hard link new to old.
// @author: James Bohrer
void make_hard_link(char **old, char **new)
{
    if(link(*old, *new) == -1)
    {
        perror(">>create:");
    }
}

//creates a soft link new to old.
// @author: James Bohrer
void make_soft_link(char **old, char **new)
{
    if(symlink(*old, *new) == -1)
    {
        perror(">>create");
    }
}