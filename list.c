/*
Authors:
	John Lahut
Date: 9.09.2018
Filename: list.c
Purpose: Create a singly linked list to store flight data.
         Used by hash.c
Project: CSI402 Final Project
*/

#include <stdlib.h>

#include "header.h"
/*
@purpose: 		add a Flight to the list
@args:	  		f is a valid Flight
@assumptions: 	f, c have been instantiated
                head points to the first element in a linked list, and is not null
@author: John Lahut
*/
void push(Flight f, node* head, int* c) {

    node *temp = head;
    (*c)++;
    while(temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = new(f);
}

/*
@purpose: 		return a new allocated node
@args:	  		f is a valid Flight
@author: John Lahut
*/
node* new(Flight f) {
    node* n = (node*)malloc(sizeof(node));
    if (n == NULL) {
        perror("linked list mem error");
        exit(EXIT_FAILURE);
    }
    n->f = f;
    n->next = NULL;
    return n;
}

/* OLD LOGIC, PROBABLY WILL NOT BE USED */

/*
@purpose: 		return number of flights
@args:	  		s is a valid sentinel
@assumptions: 	s has at least one element in it
@author: John Lahut
*/
int numFlights(Sentinel s) {
    return s->key;
}

/*
@purpose: 		returns a new sentinel to the caller
@author: John Lahut
*/
Sentinel newList() {
    Sentinel s = (Sentinel)malloc(sizeof(struct sentinel));
    s->currPos = NULL;
    s->head = NULL;
    s->key = 0;
    return s;
}