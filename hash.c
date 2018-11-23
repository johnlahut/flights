#include <string.h>
#include <stdlib.h>

#include "header.h"

/*
TODO: need to free() the hash table
*/

/*
@purpose: 		inits a hash table, setting all keys to an empty struct
@args:	  		*ht: declared FlightHash struct
*/
void init_hash(FlightHash* ht) {

    // for each slot in table, initialize the hashkey struct
    for(int i=0; i<HASH_SIZE; i++) {
        ht->table[i] = (hashkey*) malloc(sizeof(hashkey));
        ht->table[i]->collisions = 0;
        ht->table[i]->data = NULL;
    }
}

/*
@purpose: 		inserts a given flight to the hash table
@args:	  		f: valid flight object
                *ht: valid hashtable
@assumptions: 	f, ht have been initialized
*/
void insert(Flight f, FlightHash *ht) {

    // get key
    int key = 0;
    hash(f.origin, &key);

    // get current value @ key, & number of collisions
    node* value = ht->table[key]->data;
    int* collisions = &ht->table[key]->collisions;

    // nothing there, new node
    if (value == NULL) {
        value = new(f);
    } 
    
    // something here, add to list
    else {
        push(f, value, collisions);
        // printf(">>hash: collision hash key: %d | count: %d\n", key, *collisions);
    }

    // give values back
    ht->table[key]->data = value;
    ht->table[key]->collisions = *collisions;
}

/*
@purpose: 		given a string, return the hash value
@args:	  		hash_string: assumed to be a flight's origin code
                key: initalized int
@assumptions: 	key, hash_string are initalized
*/
void hash(char* hash_string, int* key) {
    for (int i=0; i<strlen(hash_string); i++) {
        *key += hash_string[i];
    }
    *key = *key % HASH_SIZE;
}

/*
@purpose: 		prints all values in the hash table, including empty
@args:	  		*ht: FlightHash hashtable
@assumptions: 	*ht has been initialized			
*/
void hash_print(FlightHash *ht) {
    for(int i=0; i<HASH_SIZE; i++) {
        
        if(ht->table[i]->data == NULL) {
            printf(">>hash:  [%d] EMPTY\n", i);
        }
        else {
            node* temp = ht->table[i]->data;

            int ctr = 0;
            while (temp != NULL) {
                Flight f = temp->f;
                if(!ctr) {
                    printf(">>hash:  [%d] ", i);
                    print_flight(f);
                }
                else {
                    printf(">>hash: *[%d] ", i);
                    print_flight(f);
                }
                
                temp = temp->next;
                ctr++;
            }
        }

    }
}

// TODO: 
// need to think about how to do this
// should i return a new linked list of all the flights that hash to the same origin airport
// the client will have to worry about freeing the new list
// new memory allocation is not needed for non-colliding keys, but probably should keep behavior consistent
void retrieve(char* origin, FlightHash *ht, node* list) {
    int key = 0;
    hash(origin, &key);
    node* value = ht->table[key]->data;

    if (value == NULL) {
        printf(">>hash: Error: No values for given key: %s", origin);
    }
    else {

    }

}