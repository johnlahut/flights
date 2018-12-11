CC = gcc -std=gnu99
CFLAGS = -o
CDEBUG = -g -o
all: fileconverter naiveinterface indexer create srchindx
fileconverter: fileconverter.c
	$(CC) fileconverter.c _fileconverter.c utils.c array.c $(CFLAGS) fileconverter
naiveinterface: interfaceFunctions.c
	$(CC) interfaceFunctions.c input.c $(CFLAGS) naiveinterface
indexer: indexer.c
	$(CC) indexer.c _indexer.c $(CFLAGS) indexer
create: create.c
	$(CC) create.c $(CFLAGS)create
srchindx: srchindx.c
	$(CC) srchindx.c hash.c list.c array.c utils.c _fileconverter.c $(CFLAGS) srchindx
clean:
	rm -f *.o core