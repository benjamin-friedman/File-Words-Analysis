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
#include <ctype.h>
#include "hash_table.h"

// gets rid of uppercase and any non alphabetic characters
char* modifyWord(char* unmodifiedWord);

int main(int argc, char* argv[]) {

	HASH_TABLE hTable = ht_create(10000);

	FILE* fpIn, *fpOut, *fpHashTable;
	if (fopen_s(&fpIn, "bible.txt", "r") != 0) {
		printf("Error. Failed to open input file.\n");
		exit(1);
	}
	if (fopen_s(&fpOut, "output.txt", "w") != 0) {
		printf("Error. Failed to open output file.\n");
		exit(2);
	}
	if (fopen_s(&fpHashTable, "output_hash_table.txt", "w") != 0) {
		printf("Error. Failed to open hash table output file.\n");
		exit(3);
	}

	// get all the words
	char word[100];
	int data;
	unsigned index;
	unsigned totalWords = 0;
	unsigned uniqueWords = 0;
	while (fscanf_s(fpIn, "%s", word, 100) != EOF) {
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




char* modifyWord(char* unmodifiedWord) {
	unsigned unmodifiedWordLength = strlen(unmodifiedWord);
	char modifiedWord[100] = { '\0' };
	unsigned index = 0;
	char* apostrophe = "'";
	Boolean needsToBeModified = FALSE;

	// edge case for standalone punctuation (? : ; etc.)
	if (unmodifiedWordLength == 1 && !isalpha(unmodifiedWord[0])) {
		unmodifiedWord[0] = '\0';
		return unmodifiedWord;
	}

	// check if word needs to be modified. Alphabetic characters and apostrophes are ok.
	for (unsigned i = 0; i < unmodifiedWordLength; i++) {
		if (!isalpha(unmodifiedWord[i] && unmodifiedWord[i] != apostrophe[0])) {
			needsToBeModified = TRUE;
			break;
		}
	}
	// doesn't need to be modified.
	if (!needsToBeModified)
		return unmodifiedWord;

	// needs to be modified. Continue with modification.
	// copy all alphbaetical characters/apostrophes from unmodified word into modified word
	for (unsigned i = 0; i < unmodifiedWordLength; i++) {
		// alphabetic letter
		if (isalpha(unmodifiedWord[i]))
			modifiedWord[index++] = tolower(unmodifiedWord[i]);
		// apostrophes are ok
		else if (unmodifiedWord[i] == apostrophe[0])
			modifiedWord[index++] = unmodifiedWord[i];
	}


	for (unsigned i = 0; i < index; i++)
		unmodifiedWord[i] = modifiedWord[i];
	unmodifiedWord[index] = '\0';

	return unmodifiedWord;
}
