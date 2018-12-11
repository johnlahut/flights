/*
Authors:
	John Lahut
Date: 9.30.2018
Filename: array.c
Purpose: Dynamically allocated array of Flight structures used for storing the 
         Flights while being read into memory from binary file.
Project: CSI402 Final Project
*/

#include <stdlib.h>
#include <string.h>

#include "header.h"

// private function prototypes
int flight_cmp(const void*, const void*);

/*
@purpose: 		initializes array, allocates memory
@args:	  		FlightArray *arr - inital list struct
@assumptions: 	arr has been instantiated
@author: John Lahut
*/
void init_array(FlightArray *arr){
    arr->capacity = INIT_ARRAY_SIZE;
    arr->size = 0;

    arr->data = malloc(sizeof(Flight) * arr->capacity);
}

/*
@purpose: 		Adds a Flight f to the array at the current position of the list
                Doubles the capacity if list is full
@args:	  		Flight f - flight to add to the list
                FlightArray arr - array to insert f into
@assumptions: 	f is a valid flight, init_array has been called on arr
@author: John Lahut
*/
void add(Flight f, FlightArray *arr) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, sizeof(Flight) * arr->capacity);
    }

    arr->data[arr->size++] = f;
}

/*
@purpose: 		returns the flight found at index
@args:	  		int index - position of list where we want to retrieve a flight
                FlightArray arr - array to retrieve from
@assumptions: 	index is within bounds of arr, arr is non-empty FlightArray
@return:        returns the flight found at the index
@author: John Lahut
*/
Flight get(int index, FlightArray *arr) {
    return arr->data[index];
}

/*
@purpose: 		compares two flight timestamps
@args:	  		void* a - "left" side of comparison functions
                void* b - "left" side of comparison functions
@assumptions: 	a, b are actually Flight* types
@return:        -1 if b is "less than" a, 1 if vise versa, 0 if equal
@author: John Lahut
*/
int flight_cmp(const void *a, const void *b) {
    Flight *left = (Flight*)a;
    Flight *right = (Flight*)b;

    return(strcmp(left->timestamp, right->timestamp));

}

/*
@purpose: 		sorts the given fight data in decending timestamp order
@args:	  		FlightArray arr - array of Flights
@assumptions: 	arr is non-empty FlightArray
@author: John Lahut
*/
void sort(FlightArray *arr) {
    qsort(arr->data, arr->size, sizeof(Flight), flight_cmp);
}

/*
@purpose: 		frees the dynamically allocated memory used by arr
@args:	  		FlightArray arr - array of Flights
@assumptions: 	arr has been instantiated (init_array() has been called)
@author: John Lahut
*/
void destroy(FlightArray *arr) {
    arr->size = 0;
    arr->capacity = INIT_ARRAY_SIZE;
    free(arr->data);
}