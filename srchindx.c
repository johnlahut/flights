#include "header.h"

void print_list(node* list) {
	do {
		Flight flight = list->f;
		printf("%s %s %s %s\n", flight.f_code, flight.origin, flight.dest, flight.timestamp);

		if (list->next == NULL) break;
		list = list->next;
	} while (true);
}

struct BST_Node {
	struct node* airport;
	struct BST_Node* less;
	struct BST_Node* greater;
} typedef BST_Node;

void init_bst(BST_Node* bst) {
	bst->airport = NULL;
	bst->less = NULL;
	bst->greater = NULL;
}
BST_Node* new_bst_node() {
	BST_Node* n = malloc(sizeof(BST_Node));
	init_bst(n);
	return n;
}
void bst_insert(Flight f, BST_Node* bst) {
	node* airport = bst->airport;

	if (airport == NULL) {
		airport = new(f);
		bst->airport = airport;
	}
	else {
		int cmp = strcmp(f.dest, airport->f.dest);
		if (cmp < 0) {// less
			if (bst->less == NULL) bst->less = new_bst_node();
			bst_insert(f, bst->less);
		}
		else if (cmp > 0) {// greater
			if (bst->greater == NULL) bst->greater = new_bst_node();
			bst_insert(f, bst->greater);
		}
		else {// ==
			int c = 0;
			push(f, airport, &c);
		}
	}
}
bool bst_search(char* dest, BST_Node* bst, node* list) {
	if (bst == NULL) return false;
	node* airport = bst->airport;
	if (airport == NULL) return false;
	int cmp = strcmp(dest, airport->f.dest);
	if (cmp < 0) {// less
		return bst_search(dest, bst->less, list);
	}
	else if (cmp > 0) {// greater
		return bst_search(dest, bst->greater, list);
	}
	else {// ==
		*list = *airport;
		return true;
	}
}

void FlightsInAirlineFile(char* file, char* airport, int c, bool origin, FlightHash* ht, BST_Node* bst) {
	FILE *fp = fopen(file, "r");
	//printf("Searching %s with %d %s flights\n", file, c, airport);

	if (fp == NULL) {
		perror("FlightsInAirlineFile error");
		return;
	}

	char line[35] = "";
	int i = 0;

	while (!feof(fp)) {
		line[i] = fgetc(fp);

		if ((line[i++] == '\n' || feof(fp)) && i != 1) {
			line[i - 1] = '\0';
			//printf("%s", line);
			Flight f = flightFromStr(line);
			if (origin) {
				insert(f, ht);
			}
			else {
				bst_insert(f, bst);
			}

			i = 0;
		}
	}

	fclose(fp);
}

/*void FlightsInAirlineFile(char* file, char* airport, int c, bool origin) {
	FILE *fp = fopen(file, "r");
	printf("Searching %s for %d %s flights\n", file, c, airport);

	if (fp == NULL) {
		perror("FlightsInAirlineFile error");
		return;
	}

	char buf[16] = "", line[64] = "";
	int i = 0, j = 0, k = 0, l = 0, j_1 = origin ? 1 : 2;
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

				if (!strcmp(buf, airport) && j == j_1) {
					k++;
					printLine = true;
					searchState = 2;
				}
				else if (j == 2) searchState = 2;

				j++;
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
}*/

void search_flights(char* airport, char* indexFile, char* path, bool origin) {
	//printf("Searching for flights with %s airport: %s\n", origin ? "origin" : "destination", airport);

	FILE *fp = fopen(indexFile, "r");
	//printf("search_flights: opening %s\n", indexFile);

	if (fp == NULL) {
		printf("search_flights: opening %s\n", indexFile);
		perror("search_flights error");
		return;
	}

	FlightHash ht;
	BST_Node bst;
	if (origin) init_hash(&ht);
	else init_bst(&bst);

	char buf[16] = "", file[16] = "";
	int i = 0;

	bool srchForList = false, beginAirportSearch = false, searchForAirport = false;
	int srchState = 0, pathLen = strlen(path);
	bool pathFound;

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
				int cmp = strcmp(buf, airport);
				if (!cmp) {
					srchState = 4;
					//printf("Airport %s found\n", airport);
				}
				else if (cmp > 0) {
					break; // Early bailout, airport not found in alphabetical order, will never be found
				}
				else srchState = 0;
				i = 0;
			}
			else i++;
		}
		else if (srchState == 4) {// find airline file beginning
			if (buf[0] != ' ' && buf[0] != '\\' && buf[0] != '\t' && buf[0] != '\n' && buf[0] != '\r') {
				srchState = 5;
				pathFound = pathLen == 0;
				i++;
			}
		}
		else if (srchState == 5) {// parse airline file name
			if (buf[i] == ' ') {
				if (pathFound) {
					buf[i] = '\0';
					strcpy(file, buf);
					srchState = 6;
				}
				else srchState = 0;
				i = 0;
			}
			else if (!pathFound && (buf[i] == '/' || buf[i] == '\\')) {// check path
				buf[i] = '\0';
				if (!strcmp(buf, path)) {// path found
					pathFound = true;
					buf[i] = '/';
					i++;
				}
				else {
					srchState = 0;
					i = 0;
				}
			}
			else if (buf[i] == '\n') {
				srchState = -1;
				break;
			}
			else i++;
		}
		else if (srchState == 6) {// parse flight count and search file
			if (buf[i] == ' ') {
				buf[i] = '\0';

				FlightsInAirlineFile(file, airport, atoi(buf), origin, &ht, &bst);

				srchState = 5;
				i = 0;
			}
			else i++;
		}
	}

	if (srchState != -1) {
		//printf("Airport %s not found\n", airport);
	}

	if (origin) {
		node list;
		if (retrieve(airport, &ht, &list)) {
			print_list(&list);
			/*do {
				Flight flight = list.f;
				printf("%s %s %s %s\n", flight.f_code, flight.origin, flight.dest, flight.timestamp);

				if (list.next == NULL) break;
				list = *list.next;
			} while (true);*/
		}
	}
	else {
		node list;
		if (bst_search(airport, &bst, &list)) {
			print_list(&list);
		}
	}

	fclose(fp);
}


int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf(">>srchindx: invalid arguments, usage is: srchindx <flags> <search terms> <index file> <path>\n");
		return 0;
	}

	char* flag = argv[1];
	bool origin = false;

	if (!strcmp(flag, "-o")) origin = true;
	else if (!strcmp(flag, "-d")) origin = false;

	char* indexFile = "invind.txt";
	if (argc >= 4) indexFile = argv[3];
	char* path = "";
	if (argc >= 5) path = argv[4];

	char* terms = argv[2];
	char airport[4] = "";
	int j = 0, len = strlen(terms);
	for (int i = 0; i < len; i++) {
		char c = terms[i];
		bool delim = c == ';';
		if (!delim) airport[j++] = c;
		if (delim || i == len - 1) {
			j = 0;
			//printf("%s\n", airport);
			search_flights(airport, indexFile, path, origin);
		}
	}
}