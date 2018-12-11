#include <string.h>
#include <stdlib.h>

#include "header.h"

/*
@purpose: 		inits a hash table, setting all keys to an empty struct
@args:	  		*ht: declared FlightHash struct
@author: John Lahut
*/
void init_hash(FlightHash* ht) {

	// for each slot in table, initialize the hashkey struct
	for (int i = 0; i < HASH_SIZE; i++) {
		ht->table[i] = (hashkey*)malloc(sizeof(hashkey));
		ht->table[i]->collisions = 0;
		ht->table[i]->data = NULL;
	}
}

/*
@purpose: 		given a string, return the hash value
@args:	  		hash_string: assumed to be a flight's origin code
				key: initalized int
@assumptions: 	key, hash_string are initalized
@author: John Lahut
*/
// void hash(char* hash_string, int* key) {
// 	for (int i = 0; i < strlen(hash_string); i++) {
// 		*key += hash_string[i];
// 	}
// 	*key = *key % HASH_SIZE;
// }


/*Hash Function*/
int  hashFunction(char *s,  int T) {

   /* The parameter s represents the symbol to be hashed and  */
   /* the parameter T represents the size of the hash table.  */
   /* The function returns the hash value for the symbol s.   */

   /* String s is assumed to be terminated with '\0'.         */
   /* It is also assumed that T is at least 2. The returned   */
   /* hash value is an integer in the range 0 to T-1.         */

   /* The function computes the hash value using arithmetic   */
   /* based on powers of the BASE value defined below.        */

   #define  BASE   127

   int h = 0;     /* Will hold the hash value at the end. */
   int temp;      /* Temporary.                           */

   /* The hash value is computed in the for loop below. */
   for (;  *s != 0;  s++) {
       temp = (BASE * h + *s);
       if (temp < 0) temp = -temp;
       h = temp % T;
   }

   /* The hash value computation is complete. So, */
   return h;

} /* End of hash function */


/*
@purpose: 		inserts a given flight to the hash table
@args:	  		f: valid flight object
				*ht: valid hashtable
@assumptions: 	f, ht have been initialized
@author: John Lahut, Jason Deacutis
*/
void insert(Flight f, FlightHash *ht) {

	// get key
	int key = 0;
	key = hashFunction(f.origin, HASH_SIZE);

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
@purpose: 		prints all values in the hash table, including empty
@args:	  		*ht: FlightHash hashtable
@assumptions: 	*ht has been initialized
@author: John Lahut, Jason Deacutis
*/
void hash_print(FlightHash *ht) {
	for (int i = 0; i < HASH_SIZE; i++) {

		if (ht->table[i]->data == NULL) {
			printf(">>hash:  [%d] EMPTY\n", i);
		}
		else {
			node* temp = ht->table[i]->data;

			int ctr = 0;
			while (temp != NULL) {
				Flight f = temp->f;
				if (!ctr) {
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

// @author: Jason Deacutis, John Lahut
bool retrieve(char* origin, FlightHash *ht, node* list) {
	// int key = 0;
	// hash(origin, &key);

	int key = hashFunction(origin, HASH_SIZE);
	node* value = ht->table[key]->data;

	if (value == NULL) {
		//printf(">>hash: Error: No values for given key: %s", origin);
		return false;
	}

	*list = *value;
	return true;
}