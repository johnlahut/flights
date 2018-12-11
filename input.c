#include <stdio.h>
#include <stdlib.h>
#include "header.h"

//reads a line from a file, used for the script file
char* getLine(FILE* stream) {

    char* line = malloc(CMD_LEN);
    fscanf(stream, "%[^\n]%*c", line);
    return line;
}