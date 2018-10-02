#include <stdbool.h>
#include <stdio.h>

#define MAX_FLIGHT_CODE 6
#define MAX_AIRLINE_CODE 4
#define DATE_SIZE 17
#define FLIGHT_SIZE 31



/*****  Structure Definitions  *****/
struct flight {
	char f_code[MAX_FLIGHT_CODE];
	char origin[MAX_AIRLINE_CODE];
	char dest[MAX_AIRLINE_CODE];
	char timestamp[DATE_SIZE];
};

/*****  Typing Definitions  *****/
typedef struct flight Flight;
typedef struct flight* FlightPtr;


/*****  fileconverter.c Prototypes  *****/
void generate_file(const char[], int);
void convert(const char*, const char*, Flight[]);

/*****  indexer.c Prototypes  *****/

/*****  srchindx.c Prototypes  *****/

/*****  create.c Prototypes  *****/

/*****  utils.c Prototypes  *****/
void print_flight(Flight);
int random_range(int, int);
char* strToBinStr(char*);
char* binStrToStr(char*);