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

#include "header.h"

int random_range(int lower, int upper) {
	return (rand() % (upper - lower + 1)) + lower;
}

void print_flight(Flight flight) {
	printf("Flight Code: %s | Origin Airport: %s | Destination Airport: %s | Departure Date: %s\n",
		flight.f_code, flight.origin, flight.dest, flight.timestamp);
}

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
			//num = num | (1 << i) set
			//num = num & ~(1 << i) clear
		}

		str[i] = c;
	}

	str[strLen] = '\0';
	return str;
}