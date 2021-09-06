CC = gcc
CFLAGS = -g -Wall --std=c99
OBJ = main.o hash_table.o linked_list.o
EXE = driver

$(EXE): $(OBJ)
	$(CC) $(CLFAGS) -o $(EXE) $(OBJ)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
hash_table.o: hash_table.c
	$(CC) $(CFLAGS) -c hash_table.c -o hash_table.o
linked_list.o: linked_list.c
	$(CC) $(CFLAGS) -c linked_list.c -o linked_list.o
  
clean:
  	rm $(EXE) $(OBJ)
