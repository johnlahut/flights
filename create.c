/*
Authors:
	John Lahut
	James Bohrer
	Jason Deacutis
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

//creates a new file with specified pathname and 0640 permissions.
void make_file(char **path)
{
    FILE *fp;
    if((fp = fopen(*path, "w+")) == NULL)
    {
        printf("error creating file.\n");
        exit(EXIT_FAILURE);
    }
    chmod(*path, 0640);
    fclose(fp);
}

//creates a directory at given pathname with 0750 permissions.
void make_dir(char **path)
{
    if(mkdir(*path, 0750) == -1)
    {
        printf("error creating directory.\n");
    }
}

//creates a hard link new to old.
void make_hard_link(char **old, char **new)
{
    if(link(*old, *new) == -1)
    {
        printf("error creating hard link.\n");
    }
}

//creates a soft link new to old.
void make_soft_link(char **old, char **new)
{
    if(symlink(*old, *new) == -1)
    {
        printf("error creating soft link.\n");
    }
}

// commented temporarily - need to split each program into its own main functions

int main(int argc, char* argv[])
{
    if(argc < 3 || argc > 4)
    {
        printf("Incorrect number of arguments.\n");
        exit(EXIT_FAILURE);
    }
    
    /* TODO: Might be better to put this in a switch/case statement, although not sure on
    how to impliment that, given that it takes character/interger input instead of strings.
    */
   
   //-f for file
    if(strcmp(argv[1], "-f") == 0)
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
        printf("Incorrect flag type.\n");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}