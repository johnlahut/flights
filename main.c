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
	#define num_flights 2

	Flight flights[num_flights];

	generate_file("output.txt", num_flights);
	//convert("output.bin", "", flights);

	for (int i = 0; i < num_flights; i++) {
		print_flight(flights[i]);
	}

	//printf("\n\nStrToBin: %s\n", strToBinStr("Test"));
	//printf("BinToStr: %s\n", binStrToStr(strToBin("Test")));
}