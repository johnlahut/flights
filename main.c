/*
Authors:
	John Lahut
	James Bohrer
	Jason Deacutis
Date: 9.30.2018
Filename: utils.c
Purpose: Main entry point of program.
Project: CSI402 Final Project
*/

#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#include "header.h"


int main(int argc, char* argv[]) {

	// data structs
	FlightArray flightArray;
	FlightHash flightHash;
	init_array(&flightArray);
	init_hash(&flightHash);
	
	// read test file into array
	convert("test.bin", &flightArray);

	// sort and output text files
	sort(&flightArray);
	// createFiles(&flightArray, "");
	
	printf("Capacity: %d Size: %d\n", flightArray.capacity, flightArray.size);

	// add values into hash table
	for (int i=0; i<flightArray.size; i++) {
		insert(flightArray.data[i], &flightHash);
	}

	// print data structures
	// hash_print(&flightHash);
	// printFlights(&flightArray);
	
	// tear down
	destroy(&flightArray);

	init_table();

	read_files("AA.txt");
	read_files("DL.txt");
	read_files("F.txt");
	read_files("OO.txt");

	
	write_file("invind.txt");

	printf("Normal end.\n");

}