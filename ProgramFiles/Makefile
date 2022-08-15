# Author: Benjamin G. Friedman
# Date: 08/11/2022
# File: Makefile
# Description: Makefile for the file words analysis program.


CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic #-Og -g -fsanitize=undefined
LDLIBS =
EXE1 = FileWordsAnalysis
OBJ1 = Main.o HashTable.o LinkedList.o
EXES = $(EXE1)


all: $(EXES)
.PHONY: all clean


$(EXE1): $(OBJ1)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm $(EXES) $(wildcard *.o)
