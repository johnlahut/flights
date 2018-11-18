/*
Authors:
	John Lahut
	James Bohrer
	Jason Deacutis
Date: 9.30.2018
Filename: fileconverter.c
Purpose: Given a binary file of flight data, create .txt files for each airline, and           
store them in appropriate directories. Can generate the binary files for testing purposes.
Implements list data structure to store flights while being read from file
Project: CSI402 Final Project
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "header.h" 

// -- private functions --
void add_zero_buffer(char*, int);
Flight flightFromStr(char*);


/*
@purpose: 		converts a binary file of flights to a flightarray
@args:	  		str: filename that contains the flight data
				flightArray: initalized flightarray
@assumptions: 	flightArray has been initialized

@todo: 			- create directories
				- error checking
*/
void convert(char *str, FlightArray *flightArray) {

	// convert binary file into raw string
	char* raw_flights = convertBinaryStringFile(str);

	// represents current flight in "buffer"
	char buff[FLIGHT_SIZE];
	int buff_c = 0;

	Flight f;

	// loop through string, adding chars to buffer
	// if \n is encountered, convert the string to a flight and add to array
	// will be one left over at end of reading
	for (int i = 0; i < strlen(raw_flights); i++) {
		if (raw_flights[i] == '\n' || raw_flights[i] == '\r') {
			if (buff_c > 0) {
				buff[buff_c] = '\0';
				f = flightFromStr(buff);
				add(f, flightArray);
				memset(buff, 0, sizeof(buff));
				buff_c = 0;
			}
		}
		else {
			buff[buff_c] = raw_flights[i];
			buff_c++;
		}
	}

	// will be one flight left in buffer
	f = flightFromStr(buff);
	add(f, flightArray);
}

// this is just a proof of concept function for testing
// files will need to be created by the create program which hasn't been written yet
void createFiles(FlightArray *flightArray, char* dir) {
	
	char code[MAX_FLIGHT_CODE];
	char *filename;
	Flight f;

	for (int i=0; i<flightArray->size; i++) {
		f = get(i, flightArray);
		getFlightCode(f, code);
		filename = strcat(code, OUTPUT_FEXT);

		FILE *fp = fopen(filename, "a+");
		fprintf(fp, "%s %s %s %s\n",  f.f_code, f.origin, f.dest, f.timestamp);
		fclose(fp);
	}
}

/*
@purpose: 		given a properly formatted string representation of a flight, return a Flight to the caller
@args:	  		char* str: string repr. of a Flight
@return:  		returns a Flight with data populated to the caller
@assumptions: 	str is a valid repr. of a Flight

@todo: 			maybe error checking? flight string should be valid when entering this function
*/
Flight flightFromStr(char* str) {
    Flight f;

    int cursor = 0;
	int f_code_c = 0;
	int origin_c = 0;
	int dest_c = 0;
	int timestamp_c = 0;


    // case-switch statement not working properly on my machine for some stupid reason - jlahut
    for (int i=0; i<strlen(str); i++) {
        if (cursor ==  0) {
            f.f_code[f_code_c] = str[i];
            f_code_c++;
        }
        else if (cursor == 1 ) {
            f.origin[origin_c] = str[i];
            origin_c++;
        }
        else if (cursor == 2 ) {
            f.dest[dest_c] = str[i];
            dest_c++;
        }
        else {
            f.timestamp[timestamp_c] = str[i];
            timestamp_c++;
        }

        if (str[i] == ' ') cursor++;

    }

    // add terminating string characters to each string
    // timestamp is not -1 because loop will exit at last character in string
    f.f_code[f_code_c-1] = '\0';
    f.origin[origin_c-1] = '\0';
    f.dest[dest_c-1] = '\0';
    f.timestamp[timestamp_c] = '\0';
    return f;

}

/*
@purpose: 		convert a binary file repr. flights to a raw string
@args:	  		char* filename: valid .bin file repr. flights
@return:  		raw string data from converted binary file
@assumptions: 	filename is a valid binary file
*/
char* convertBinaryStringFile(const char* filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error reading %s\n", filename);
		exit(EXIT_FAILURE);
	}

	char *str = 0;

	fseek(fp, 0, SEEK_END);

	int length = ftell(fp);
	str = malloc(length);

	fseek(fp, 0, SEEK_SET);

	if (str) fread(str, 1, length, fp);
	str[length] = '\0';

	fclose(fp);
	return binStrToStr(str);

	// printf("%s\n", str);
	// printf("%s\n", binStrToStr(str));
	// convertStr(binStrToStr(str), arr, count);
}

/* OLD FUNCTIONS, PROBABLY WILL NOT BE USED */

void convertStr(char* str, Flight arr[], int count) {
	char data[FLIGHT_SIZE - 1];
	data[FLIGHT_SIZE - 2] = '\0';
	int counter = 0;

	int k = 0;
	for (int i = 0; i < count; i++) {
		for (int i = 0; i < FLIGHT_SIZE - 2; i++) data[i] = str[k++];
		k++;//skip \n

		Flight f;

		// counters for each field in the structure
		int cursor = 0;
		int f_code_c = 0;
		int origin_c = 0;
		int dest_c = 0;
		int timestamp_c = 0;

		// add null characters to terminate the string fields
		f.origin[3] = '\0';
		f.dest[3] = '\0';
		f.timestamp[16] = '\0';
		f.f_code[4] = '\0';

		// for each line of data, loop through each character
		// if there is a space, we know we are in a new field (except for timestamp)
		// cursor = 0 : flight code data
		// cursor = 1 : origin airport data
		// cursor = 2: destination airport data
		// cursor > 2: timestamp data, will contain spaces
		for (int i = 0; i < strlen(data); i++) {

			// increment cursor if space is encountered
			if (data[i] == ' ') {
				cursor = cursor + 1;

				// if we are in timestamp segment, append the space, else ignore
				if (cursor > 3) {
					f.timestamp[timestamp_c] = data[i];
					timestamp_c++;
				}
			}
			// for each segment, append the appropriate data
			else {
				switch (cursor) {
				case 0:
					f.f_code[f_code_c] = data[i];
					f_code_c++;
					break;
				case 1:
					f.origin[origin_c] = data[i];
					origin_c++;
					break;
				case 2:
					f.dest[dest_c] = data[i];
					dest_c++;
					break;
				default:
					f.timestamp[timestamp_c] = data[i];
					timestamp_c++;
					break;
				}
			}
		}
		arr[counter] = f;
		counter++;
	}
}

void generate_file(const char filename[], int count) {
	const char *sample_airports[10] = { "AAA", "BBB", "CCC", "DDD", "EEE", "FFF", "GGG", "HHH", "III", "JJJ" };
	const char *sample_airlines[10] = { "ZZ", "YY", "XX", "WW", "VV", "UU", "TT", "SS", "RR", "QQ" };
	FILE *fp;
	FILE *fp_bin;

	fp = fopen(filename, "w");
	fp_bin = fopen("output.bin", "w");

	if (fp == NULL) {
		printf("Failed to open %s", filename);
		exit(EXIT_FAILURE);
	}
	if (fp_bin == NULL) {
		printf("Failed to open %s", "output.bin");
		exit(EXIT_FAILURE);
	}

	//printf("Files Opened\n");

	for (int i = 0; i < count; i++) {
		const char *dest = sample_airports[random_range(0, 9)];
		const char *dep = sample_airports[random_range(0, 9)];
		const char *airline = sample_airlines[random_range(0, 9)];
		char flight_code[3], timestamp[DATE_SIZE], flight_data[FLIGHT_SIZE];

		sprintf(flight_code, "%d", random_range(0, 9));
		add_zero_buffer(flight_code, 2);

		char year[5], month[3], day[3], hour[3], min[3];
		sprintf(year, "%d", random_range(2000, 2018));
		sprintf(month, "%d", random_range(1, 12));
		add_zero_buffer(month, 2);
		sprintf(day, "%d", random_range(1, 31));
		add_zero_buffer(day, 2);
		sprintf(hour, "%d", random_range(1, 24));
		add_zero_buffer(hour, 2);
		sprintf(min, "%d", random_range(1, 59));
		add_zero_buffer(min, 2);

		sprintf(timestamp, "%s-%s-%s %s:%s", year, month, day, hour, min);

		sprintf(flight_data, "%s%s %s %s %s\n", airline, flight_code, dep, dest, timestamp);
		printf("Flight Data[%d]: %s", i, flight_data);
		fprintf(fp, "%s", flight_data);

		char *binStr = strToBinStr(flight_data);

		//fwrite((const void*)flight_data, sizeof(flight_data), 1, fp_bin);
		fwrite((const void*)binStr, strlen(binStr), 1, fp_bin);
	}

	fclose(fp);
	fclose(fp_bin);
}

void add_zero_buffer(char* str, int buff) {
	if (strlen(str) < buff) {
		char temp = str[0];
		str[0] = '0';
		str[1] = temp;
		str[2] = '\0';
	}
}