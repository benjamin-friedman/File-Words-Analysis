/*
   Copyright (C) 2020 Benjamin G. Friedman. Code may be used or redistributed freely with credit given to the author.
   File Description:
	- name: main.c
	- description: the main program.
   Contact: bfriedman12@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"


int main(int argc, char* argv[]) {

	HASH_TABLE hTable = ht_create(10000);

	FILE* fpIn, *fpOut, *fpHashTable;
	if ((fpIn = fopen(argv[1], "r")) == NULL) {
		printf("Error. Failed to open input file.\n");
		exit(1);
	}
	if ((fpOut = fopen(argv[2], "w")) == NULL) {
		printf("Error. Failed to open output file.\n");
		exit(2);
	}
	if ((fpHashTable = fopen(argv[3], "w")) == NULL) {
		printf("Error. Failed to open hash table output file.\n");
		exit(3);
	}

	// get all the words
	char word[100];
	int data;
	unsigned index;
	unsigned totalWords = 0;
	unsigned uniqueWords = 0;
	while (fscanf(fpIn, "%s", word) != EOF) {
		modifyWord(word);
		if (strcmp(word, "")) {
			totalWords++;
			if (ht_keyExists(hTable, word, &index, &data)) {
				ht_updateElement(hTable, word, index, data + 1);
			}
			else {
				ht_insert(hTable, word, 1);
				uniqueWords++;
			}
		}
	}

	// generate the output
	NODE list = NULL;
	ht_createListLTG(hTable, &list);
	fprintf(fpOut, "%-20s%u\n", "Total words:", totalWords);
	fprintf(fpOut, "%-20s%u\n\n", "Unique words:", uniqueWords);
	list_printHeadToTail(list, fpOut);

	ht_print(hTable, fpHashTable);

	ht_destroy(&hTable);
	list_destroy(&list);
	fclose(fpIn);
	fclose(fpOut);
	fclose(fpHashTable);

	return 0;
}
