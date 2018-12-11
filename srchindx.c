
void FlightsInAirlineFile(char* file, char* airport, int c, bool origin, FlightHash* ht, BST_Node* bst) {
	FILE *fp = fopen(file, "r");
void search_flights(char* airport, char* indexFile, char* path, bool origin) {
	//printf("Searching for flights with %s airport: %s\n", origin ? "origin" : "destination", airport);

	// Add path to beginning of index file name
	int pathLen = strlen(path);
	char* indexFull = malloc(sizeof(char)*(pathLen + strlen(indexFile) + 2));
	strcpy(indexFull, path);
	if (pathLen > 0) strcat(indexFull, "/");
	strcat(indexFull, indexFile);

	FILE *fp = fopen(indexFull, "r");
	FlightHash ht;
	BST_Node bst;
	if (origin) init_hash(&ht);// init hashtable
	else init_bst_node(&bst);// int BST
			search_flights(airport, indexFile, path, origin);// search for term