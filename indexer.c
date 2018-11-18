#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define TABLE_SIZE 15
#define ARRAY_SIZE  1
#define MAX_FILE_LEN (MAX_AIRLINE_CODE + 4)

// to hold (Filename, Count) pairs
struct file_tuple {
    char *filename;
    int hits;
} typedef FileTuple;

// to hold a list of (Filename, Count) pairs
// dynamic
struct string_array {
    FileTuple *data;
    int capacity;
    int size;
} typedef StringArr;

// hash table keys, key is airport, value is string array of filenames
struct airport_key {
    StringArr filenames;
    char airport[MAX_AIRLINE_CODE];
} typedef Key;

// hash table
struct airport_table {
    Key *table;
    int size;
    int capacity;
} typedef AirportTable;

void add_airport(char*, char*, AirportTable*, bool, int);
void sort_airports();
void sort_hits();
void copy_table(AirportTable*, AirportTable*);
void print_table(AirportTable* table);
void expand_table(AirportTable*, AirportTable*);
bool is_full(AirportTable*);
void add_file(char*, StringArr*, bool, int);

AirportTable table;

/*
@purpose: 		initializes a string array
@args:	  		arr: array to be initialized
@assumptions: 	arr has been declared 
*/
void init_str_arr(StringArr* arr){
    arr->capacity = ARRAY_SIZE;
    arr->size = 0;
    arr->data = (FileTuple*) malloc(ARRAY_SIZE * sizeof(FileTuple));
}

void init_table() {
    table.capacity = TABLE_SIZE;
    table.size = 0;
    table.table = malloc(sizeof(Key) * table.capacity);

    for (int i=0; i<table.capacity; i++) {
        strcpy(table.table[i].airport, "-");
        init_str_arr(&table.table[i].filenames);
    }
}

/*
@purpose: 		adds a file to the array, increments hit counter if found
@args:	  		char* filename: filename to add/search 
                StringArr *arr: array to add file tuple to
                bool rehash: if we are rehashing, need to copy hits
                int hits: hits to copy only if rehashing
@assumptions: 	filename does not exceed max file length, arr has been initialized
*/
void add_file(char* filename, StringArr *arr, bool rehash, int hits) {

    // grow array if needed
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity*2;
        arr->data = realloc(arr->data, sizeof(FileTuple) * arr->capacity);
    }

    // look in array to see if file is already there
    // if found, increment counter, return
    for (int i=0; i<arr->size; i++) {
        if (strcmp(arr->data[i].filename, filename) == 0) {

            if (!rehash) arr->data[i].hits++;
            else arr->data[i].hits = hits;
            return;
        }
    }

    // not found, add to array, set hits to 1
    arr->data[arr->size].filename = (char*) malloc(MAX_FILE_LEN);
    if (!rehash) arr->data[arr->size].hits = 1;
    else arr->data[arr->size].hits = hits;
    strcpy(arr->data[arr->size++].filename, filename);
    // arr->size++;
}

// algorithm from: http://www.cse.yorku.ca/~oz/hash.html
// weird parenthesis needed to silence GCC warning
unsigned long airport_hash(char *str){
    unsigned long hash = 5831;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

/*
@purpose: 		prints the hash table
@assumptions: 	table has been initialized
*/
void print_table(AirportTable* table) {
    for(int i=0; i<table->capacity; i++) {
        printf("[%2d] Airport: %5s\n", i, table->table[i].airport);
        for (int j=0; j<table->table[i].filenames.size; j++) {
            printf("\t%-6s %2d\n", table->table[i].filenames.data[j].filename, table->table[i].filenames.data[j].hits);
        }
    }
    float f = ((float)table->size/table->capacity)*100.0;
    printf("Occupancy: %.1f%-6c Size: %d Capacity: %d\n", f, '%', table->size, table->capacity);
}

// TODO: Better collision handling
/*
@purpose: 		given a key and airport, find a new key for the airport (was a collision)
@args:	  		long *key: key to validate
                char *airport: airport to add to table
@assumptions: 	key is within range of 0 < key < TABLE_SIZE
                function will only be called if there is a collision
*/
void validate_key(unsigned long *key, char *airport, AirportTable *table) {
    
    // set temp key to +1 current key
    unsigned long temp = *key+1;

    // loop through all entries, trying to find an empty spot
    while (temp != *key) {

        // loop back around if at end
        if (temp >= table->capacity)
            temp = 0;
        
        // set key to next empty spot
        if(strcmp(table->table[temp].airport, "-") == 0 || strcmp(table->table[temp].airport, airport) == 0) {
            // printf("\tnew key for %s is %lu, old was %lu.\n", airport, temp, *key);
            *key = temp;
            break;
        }
        temp++;
    }
}

/*
@purpose: 		expands the given old array into the new one
@args:	  		AirportTable* old, new - old is the full table, new will have double the capacity
@assumptions: 	old is full, new has been declared; after this function the global variable table will have double the capacity 
*/
void expand_table(AirportTable* old, AirportTable* new) {

    // double capacity, reset all other fields (cannot use realloc because we need to rehash)
    new->capacity = old->capacity * 2;
    new->size = 0;
    new->table = malloc(sizeof(Key) * new->capacity);

    // reinit all arrays, set to blank value
    for (int i=0; i<new->capacity; i++){
        init_str_arr(&new->table[i].filenames);
        strcpy(new->table[i].airport, "-");
        
    }

    // rehash all values in old table
    for (int i=0; i<old->size; i++){
        for(int j=0; j<old->table[i].filenames.size; j++){
            add_airport(old->table[i].airport, old->table[i].filenames.data[j].filename, new, true, old->table[i].filenames.data[j].hits);
            printf("%s %s %d\n", old->table[i].airport, old->table[i].filenames.data[j].filename, old->table[i].filenames.data[j].hits);
        }
    }

    // old gets new
    table = *new;
}


/*
@purpose: 		inserts an airport, filename into the hash table
@args:	  		char *airport: airport to add to hash table 
                char *filename: filename airport was found in
                bool rehash: if we are rehashing, need to copy over hits
                int hits: hits to copy over if we are rehasing
@assumptions:   airport, filename do not exceed length limits
*/
void add_airport(char* airport, char* filename, AirportTable* table, bool rehash, int hits) {

    // expands table if needed, rehashes all values in old table. table will be new table after this block
    if (is_full(table)) {
        printf(">>indexer: Expanding table...\n");
        AirportTable new;
        expand_table(table, &new);
    }

    // get key
    unsigned long key = airport_hash(airport) % table->capacity;

    // collision, not empty, not current airport
    if (strcmp(table->table[key].airport, airport) != 0 && strcmp(table->table[key].airport, "-") != 0) {
        // printf(">>indexer err: Attempting to insert: %s but %s already is here!\n", airport, table->table[key].airport);
        
        // get new key
        validate_key(&key, airport, table);
    }

    // no collisions (chaining), fresh entry
    if (strcmp(table->table[key].airport, "-") == 0)
        table->size++;

    // actually add new airport into hashtable
    strcpy(table->table[key].airport, airport);
    add_file(filename, &table->table[key].filenames, rehash, hits);
};


/*
@purpose: 		read a given file generated by fileconverter
@args:	  		char* filename: valid .txt file containing all flight data for a given airline
@assumptions: 	filename is a valid .txt file containing valid data from fileconverter
*/
void read_files(char* filename) {
    FILE *fp = fopen(filename, "r");
    Flight f;

    printf(">>indexer: indexing %s\n", filename);

    if (fp == NULL) {
        perror(">>indexer err");
        return;
    }

    // loop through file
    while(!feof(fp)) {

        // read all data into temp flight struct; the '%[^\n]' thing just grabs the rest of the line
        // not splitting on a space (timestamp contains a space that we want to read in)
        fscanf(fp, "%s %s %s %[^\n]", f.f_code, f.origin, f.dest, f.timestamp);

        // need to check again because condition isn't evaluated until after it 'fails'
        if (!feof(fp)) {
            // print_flight(f);
            add_airport(f.origin, filename, &table, false, -1); add_airport(f.dest, filename, &table, false, -1);
        }
    }
    fclose(fp);
}


/*
@purpose: 		writes the inverted index file
@args:	  		char* filename: filename to write to
@assumptions: 	all airline files have been parsed, table has been initalized and populated
*/
void write_file(char* filename) {
    // print_table(&table);
    printf(">>indexer: writing output...\n");

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror(">>indexer err");
        return;
    }

    // sort first on hits for each file list, and then sort hash table alphabetically
    // WARNING: this destroys the integrity of the table. only sort when it is the LAST thing to do
    
    sort_hits();
    sort_airports();

    // print_table(&table);

    // for each item in the table, check to see if there is something there
    for (int i=0; i<table.capacity; i++) {
        // something is here, output the airport
        if(table.table[i].filenames.size != 0) {
            fprintf(fp, "<list> %s\n\t", table.table[i].airport);

            // for each (airline) file that contains the airport, output the counts and airline file
            for (int j=0; j<table.table[i].filenames.size; j++) {
                fprintf(fp, "%s %d ", table.table[i].filenames.data[j].filename,
                                      table.table[i].filenames.data[j].hits);
            }
            fprintf(fp, "\n</list>\n");
        }
    }
    fclose(fp);

}

/*
@purpose: 		compares two airports
@args:	  		void *a, *b: void pointers to be cast into Key values
@return:  		strcmp values based on airports
@assumptions: 	a, b are valid pointers to Key structs
*/
int airport_cmp(const void *a, const void *b) {
    Key *left = (Key*)a;
    Key *right = (Key*)b;

    return(strcmp(left->airport, right->airport));
}

/*
@purpose: 		compares two elements in the file tuple array
@args:	  		void *a, *b: void pointers to be cast into FileTuple values
@return:  		-1 if left is greater or equal than right, 1 otherwise
@assumptions: 	a, b are valid pointers to FileTuple structs
*/
int hit_cmp(const void *a, const void *b) {
    FileTuple *left = (FileTuple*)a;
    FileTuple *right = (FileTuple*)b;
    return (left->hits >= right->hits) ? -1 : 1;
}

/*
@purpose: 		sorts the hash table based on airports
                WARNING: this is destroys the integrity of the hash table as keys will no 
                         longer map to the correct value. sorting should only be done as the last
                         thing in this data structure
@assumptions: 	table has been initialized and populated
*/
void sort_airports() {
    qsort(table.table, table.capacity, sizeof(Key), airport_cmp);
}

/*
@purpose: 		for a given hash entry, sort the list of files by hits
@assumptions: 	table has been initialized and populated
*/
void sort_hits() {
    
    for (int i=0; i<table.capacity; i++) {
        qsort(table.table[i].filenames.data, table.table[i].filenames.size, sizeof(FileTuple), hit_cmp);
    }

}

/*
@purpose: 		checks to see if the hash table is full 
@args:	  		AirportTable* pointer to the table to check
@return:  		true if full, false otherwise
@assumptions: 	table has been initialized
*/
bool is_full(AirportTable* table){
    return (table->size >= table->capacity); 
}