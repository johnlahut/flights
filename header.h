#include <stdbool.h>
#include <stdio.h>

#define MAX_FLIGHT_CODE 7
#define MAX_AIRLINE_CODE 4
#define DATE_SIZE 20
#define FLIGHT_SIZE MAX_FLIGHT_CODE + (2*MAX_AIRLINE_CODE) + DATE_SIZE

#define OUTPUT_FEXT ".txt"

#define INIT_ARRAY_SIZE 4
#define HASH_SIZE 50



/*****  Structure Definitions  *****/
struct flight {
	char f_code[MAX_FLIGHT_CODE];
	char origin[MAX_AIRLINE_CODE];
	char dest[MAX_AIRLINE_CODE];
	char timestamp[DATE_SIZE];
} typedef Flight;

struct node {
    struct flight f;
    struct node *next;
} typedef node;

// probably will not be used, old logic 
struct sentinel {
	node *head;
	node *currPos;
	int key;
} typedef *Sentinel;

// dynamic array for reading in binary file
struct flightArray {
	int size;
	int capacity;
	struct flight *data;
} typedef FlightArray;

// all table entries will be initalized to this struct which points to nodes
struct hashkey {
	int collisions;
	node* data;
} typedef hashkey;

// represents a hash table
struct hashtable {
	hashkey *table[HASH_SIZE];
} typedef FlightHash;

struct index {
	char file[20];
	int count;
};
struct airlineArray {
	int size;
	int capacity;
	struct index *index;
};

struct airlineHash;

/*****  Typing Definitions  *****/


/*****  fileconverter.c Prototypes  *****/

void generate_file(const char[], int);
void convert(char*, FlightArray*);
char* convertBinaryStringFile(const char*);
Flight flightFromStr(char*);
void createFiles(FlightArray*, char*);

/*****  list.c Prototypes  *****/
// void push(Flight, node*, int*);
// node* new(Flight);
// int numFlights();
// Sentinel newList();
// // void swap(node*, node*, Sentinel);
// void printReverse(Sentinel);

// /*****  hash.c Prototypes  *****/
// void insert(Flight, FlightHash*);
// void retrieve(char*, FlightHash*, node*);
// void hash(char*, int*);
// void init_hash(FlightHash*);
// void hash_print(FlightHash*);

/*****  indexer.c Prototypes  *****/
void read_files(char*);
void init_indexer();
void init_table();
void print_stack();
unsigned long airport_hash(char*);
void write_file(const char* filename);
/*****  srchindx.c Prototypes  *****/

// /*****  create.c Prototypes  *****/

/*****  utils.c Prototypes  *****/
void print_flight(Flight);
int random_range(int, int);
char* strToBinStr(char*);
char* binStrToStr(char*);
void printFlights(FlightArray*);
void getFlightCode(Flight, char[]);
void check_file(FILE*, char*);

/*****  array.c Prototypes  *****/
void init_array(FlightArray*);
void add(Flight , FlightArray*);
Flight get(int, FlightArray*);
void sort(FlightArray*);
void destroy(FlightArray*);