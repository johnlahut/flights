/*
Authors:
	John Lahut
	James Bohrer
	Jason Deacutis
Date: 9.30.2018
Filename: utils.c
Purpose: General purpose utility functions that may be used across programs.
Project: CSI402 Final Project
*/

#include <stdlib.h>
#include <string.h>
#include "header.h"

#define CHAR_BIT 8

extern node* head;

/*
@purpose: 		returns a random range to the caller bound by lower, upper
@args:	  		int lower: lower bound
				int upper: upper bound
@return:  		int: random number
@assumptions: 	N/A
@author: John Lahut
*/
int random_range(int lower, int upper) {
	return (rand() % (upper - lower + 1)) + lower;
}

/*
@purpose: 		prints a flight	
@args:	  		flight: valid Flight struct
@assumptions: 	flight has been instantiated and populated
@author: John Lahut	
*/
void print_flight(Flight flight) {
	printf("Flight Code: %s | Origin Airport: %s | Destination Airport: %s | Departure Date: %s\n",
		flight.f_code, flight.origin, flight.dest, flight.timestamp);
}

/*
@purpose: 		converts a string to a binary string
@author: Jason Deacutis
*/
char* strToBinStr(char *s) {
	int len = strlen(s), binLen = len * CHAR_BIT;
	char *bin = malloc(binLen + 1);
	if (bin == NULL) {
		printf("strToBin(%s) malloc fail\n", s);
		return NULL;
	}

	int k = 0;
	for (int i = 0; i <= len; i++) {
		char c = s[i];

		for (int j = CHAR_BIT - 1; j >= 0; j--) {
			bin[k++] = ((c >> j) & 1) == 1 ? '1' : '0';
		}
	}

	bin[binLen] = '\0';
	return bin;
}

/*
@purpose: 		converts a binary string to a string	
@author: Jason Deacutis
*/
char* binStrToStr(char *bin) {
	int len = strlen(bin), strLen = len / CHAR_BIT;

	char *str = malloc(strLen + 1);
	if (bin == NULL) {
		printf("binToStr(%s) malloc fail\n", bin);
		return NULL;
	}

	int k = 0;
	for (int i = 0; i <= strLen; i++) {
		char bitc = bin[i];
		int bit = bitc == '1' ? 1 : 0;

		char c = 0;

		for (int j = CHAR_BIT - 1; j >= 0; j--) {
			if (bin[k++] == '1')  c |= 1 << j;
			else c &= ~(1 << j);
		}

		str[i] = c;
	}

	str[strLen] = '\0';
	return str;
}

/*
@purpose: 		prints all flights in a given flight array
@args:	  		*s: valid flight array
@assumptions: 	*s: has been instantiated
@author: Jason Deacutis
*/
void printFlights(FlightArray *s) {
	for(int i=0; i<s->size; i++) {
		print_flight(get(i, s));
	}
}

/*
@purpose: 		returns a flight code for a given flight
@args:	  		f: a valid flight
				code: string to return to caller
@assumptions: 	code is at least size of 5, f is a valid flight
@author: John Lahut
*/
void getFlightCode(Flight f, char code[]) {
	int i = 0;

	while(f.f_code[i] >= 65) {
		code[i] = f.f_code[i];
		i++;
	}
	code[i] = '\0';
}

// @author: Jason Deacutis
void check_file(FILE* fp, char* prog) {
	if (fp == NULL) {
		perror(prog);
	}
}