/*
Authors:
	John Lahut
Date: 9.30.2018
Filename: fileconverter.c
Project: CSI402 Final Project
Purpose: Main entry point of program
*/
#include "header.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>


/*
@purpose: 		converts a binary file of flights to a flightarray
@args:	  		str: filename that contains the flight data
				flightArray: initalized flightarray
@assumptions: 	flightArray has been initialized
@author: John Lahut
*/
int main(int argc, char* argv[]) {

	DIR *dp;
	FILE *fp;
	FlightArray flightArray;
	
	if (argc < 2 || argc > 3) {
		printf(">>fileconverter: Supported usage is fileconverter infile outdirectory.\n");
	}

	// file + directory
	else if (argc == 3) {
		dp = opendir(argv[2]);
		if (dp == NULL) perror(">>fileconverter");
		// dir exists
		else {

			fp = fopen(argv[1], "r");
			if (fp == NULL) {
				perror(">>fileconverter");
			}

			else {
				init_array(&flightArray);
				convert(argv[1], &flightArray);
				sort(&flightArray);
				createFiles(&flightArray, argv[2]);
				destroy(&flightArray);
				fclose(fp);
			}
		closedir(dp);
		}
	}
	// file only
	else if (argc == 2) {
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			perror(">>fileconverter");
		}
		else {
			init_array(&flightArray);
			convert(argv[1], &flightArray);
			sort(&flightArray);
			createFiles(&flightArray, "");
			destroy(&flightArray);
			fclose(fp);
		}
	}
	return 0;
}