CC = gcc
CFLAGS = -g
OBJECTS = main.o list.o fileconverter.o utils.o array.o hash.o indexer.o create.o
OUT_FILES = AA.txt DL.txt F.txt OO.txt

test: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS)
%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o core
cleanoutput: # clean up .txt files
	rm -f $(OUT_FILES)
superclean: # WARNING: REMOVES ALL .txt FILES
	rm -f *.o *.txt core a.out