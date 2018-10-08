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

#include "header.h"


int main(int argc, char* argv[]) {
	//const int num_flights = 2;// insisted on constant but wouldn't compile
	#define SIZE 20

	Flight flights[SIZE];
	int *num_flights;

	// generate_file("output.txt", num_flights);
	// convert("output.bin", "", flights);
	// convert("test.bin", flights);

	convert("output.bin", flights, SIZE);

	for (int i=0; i<10; i++) {
		print_flight(flights[i]);
	}


	// convertBinaryStringFile("test.bin", flights, num_flights);

	// for (int i = 0; i < num_flights; i++) {
	// 	print_flight(flights[i]);
	// }

	//printf("\n\nStrToBin: %s\n", strToBinStr("Test"));
	//printf("BinToStr: %s\n", binStrToStr(strToBin("Test")));
}