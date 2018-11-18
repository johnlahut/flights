#include "header.h"

void search_flights(char* airport) {
	printf("Searching for flights with airport: %s\n", airport);

	char* master = "invind.txt";

	/*char* filename = malloc(strlen(airport) + 5);
	sprintf(filename, "%s.txt", airport);*/

	FILE *fp = fopen(master, "r");
	printf("search_flights: opening %s\n", master);

	if (fp == NULL) {
		perror("search_flights error");
		return;
	}

	char buf[16] = "";
	char file[16] = "";
	int i = 0;

	bool srchForList = false, beginAirportSearch = false, searchForAirport = false;
	int srchState = 0;

	while (!feof(fp)) {
		buf[i] = fgetc(fp);

		if (srchState == 0) {// find <list> beginning
			if (buf[0] == '<')
				srchState = 1;
		}
		if (srchState == 1) {// parse <list>
			i++;
			if (i == 6) {
				if (!strcmp(buf, "<list>")) {
					srchState = 2;
				}
				else srchState = 0;
				i = 0;
			}
		}
		else if (srchState == 2) {// find airport beginning
			if (buf[0] != ' ') {
				srchState = 3;
				i++;
			}
		}
		else if (srchState == 3) {// parse airport
			if (buf[i] == '\n') {
				buf[i] = '\0';
				if (!strcmp(buf, airport)) {
					srchState = 4;
					printf("Airport found\n");
				}
				else srchState = 0;
				i = 0;
			}
			else i++;
		}
		else if (srchState == 4) {// find airline file beginning
			if (buf[0] != ' ' && buf[0] != '\\' && buf[0] != '\t' && buf[0] != '\n' && buf[0] != '\r') {
				srchState = 5;
				i++;
			}
		}
		else if (srchState == 5) {// parse airline file name
			if (buf[i] == ' ') {
				buf[i] = '\0';
				strcpy(file, buf);
				srchState = 6;
				i = 0;
			}
			else if (buf[i] == '\n') {
				//srchState = 0;
				//i = 0;
				srchState = -1;
				break;
			}
			else i++;
		}
		else if (srchState == 6) {// parse flight count and search file
			if (buf[i] == ' ') {
				buf[i] = '\0';

				FlightsInAirlineFile(file, airport, atoi(buf));

				srchState = 5;
				i = 0;
			}
			else i++;
		}
	}

	if (srchState != -1) {
		printf("Unknown airport");
	}

	fclose(fp);
}

void FlightsInAirlineFile(char* file, char* airport, int c) {
	FILE *fp = fopen(file, "r");
	printf("Searching %s for %d %s flights\n", file, c, airport);

	if (fp == NULL) {
		perror("FlightsInAirlineFile error");
		return;
	}

	char buf[16] = "", line[64] = "";
	int i = 0, j = 0, k = 0, l = 0;
	int searchState = 0;
	bool printLine = false;

	while (!feof(fp)) {
		line[l++] = buf[i] = fgetc(fp);

		if (searchState == 0) {// find airport section of flight
			if (buf[i] == ' ') {
				i = 0;
				j++;
				searchState = 1;
			}
			else i++;
		}
		else if (searchState == 1) {// parse airport
			if (buf[i] != ' ') i++;
			else {
				buf[i] = '\0';
				j++;

				if (!strcmp(buf, airport)) {
					k++;
					printLine = true;
					searchState = 2;
				}
				else if (j == 3) searchState = 2;

				i = 0;
			}
		}
		else if (searchState == 2) {// find next line
			if (buf[i] == '\n' || feof(fp)) {
				if (printLine) {
					printLine = false;
					line[l] = '\0';
					printf("%s", line);
					if (k == c) break;
				}
				
				j = 0;
				l = 0;
				searchState = 0;
			}
		}
	}

	fclose(fp);
}