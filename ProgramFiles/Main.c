/*
  Author: Benjamin G. Friedman
  Date: 08/11/2022
  File: Main.c
  Description:
	  - The main function for the file words analysis program.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "HashTable.h"


#define HASH_TABLE_CAPACITY 10000


/**** Helper functions ****/
// validates the command line arguments - terminates program if anything is invalid
void validateCLA(int argc, const char* argv[]);

// Opens a file in the given mode and stores it in the file pointer pointed to by pfp.
// Terminates the program if it fails to open.
void openFileByNameAndMode(FILE** pfp, const char* fileName, const char* mode);




int main(int argc, const char* argv[]) {

	HASH_TABLE hHashTableWords;
	LINKED_LIST hLinkedListWords;
	FILE* fpInputFile;
	FILE* fpOutputFileWords;
	FILE* fpOutputFileHashTable;

	// validate command line arguments and open files
	validateCLA(argc, argv);
	openFileByNameAndMode(&fpInputFile, argv[1], "r");
	openFileByNameAndMode(&fpOutputFileWords, argv[2], "w");
	openFileByNameAndMode(&fpOutputFileHashTable, argv[3], "w");

	
	// read the input file and store all of the words and # of occurences in the hash table
	hHashTableWords = hashTableInitWithCapacity(HASH_TABLE_CAPACITY);
	if (!hHashTableWords) {
		printf("Error. Memory allocation failure during hash table creation. Exiting program.\n");
		exit(1);
	}
	char word[100];
	unsigned totalWords = 0;
	unsigned uniqueWords = 0;

	while (fscanf(fpInputFile, "%s", word) != EOF) {
		hashTablemodifyWord(word);
		if (strcmp(word, "")) {
			++totalWords;
			if (hashTableKeyExists(hHashTableWords, word))
				hashTableInsert(hHashTableWords, word, hashTablGetDataOfKey(hHashTableWords, word, NULL) + 1);
			else {
				hashTableInsert(hHashTableWords, word, 1);
				++uniqueWords;
			}
		}
	}


	// generate the output files
	hLinkedListWords = linkedListInitDefault();
	if (!hLinkedListWords) {
		printf("Error. Memory allocation failure during linked list creation. Exiting program.\n");
		exit(1);
	}

	if (!hashTableCreateLinkedListLTG(hHashTableWords, hLinkedListWords)) {
		printf("Error. Memory allocation failure during linked list creation from hash table. Exiting program.\n");
		exit(1);
	}

	fprintf(fpOutputFileWords, "%-20s%u\n", "Total words:", totalWords);
	fprintf(fpOutputFileWords, "%-20s%u\n\n", "Unique words:", uniqueWords);
	linkedListPrintHeadToTailMultiplelines(hLinkedListWords, fpOutputFileWords);
	hashTablePrint(hHashTableWords, fpOutputFileHashTable);
	

	// clean up resources and end program
	hashTableDestroy(&hHashTableWords);
	linkedListDestroy(&hLinkedListWords);
	fclose(fpInputFile);
	fclose(fpOutputFileWords);
	fclose(fpOutputFileHashTable);

	return 0;
}




/**** Helper functions ****/
void validateCLA(int argc, const char* argv[]) {
	// 3 CLA
	if (argc != 4) {
		printf("Error. Expected 3 command line arguments but got %d\n", argc);
		printf("Correct arguments - ./FileWordsAnalysis [input file] [word counter file] [hash table output file]\n");
		printf("Exiting the program.\n");
		exit(1);
	}

	// validate they're text files
	for (size_t i = 1; i < (size_t)argc; ++i) {
		size_t len = (strlen(argv[i]));
		if ((len < 5) || (argv[i][len - 1] != 't') || (argv[i][len - 2] != 'x') || (argv[i][len - 3] != 't') || (argv[i][len - 4] != '.')) {
			printf("Error. Each command line arguments must be a text file.\n");
			printf("Correct arguments - ./FileWordsAnalysis [input file] [word counter file] [hash table output file]\n");
			printf("Exiting the program.\n");
			exit(1);
		}
	}
}


void openFileByNameAndMode(FILE** pfp, const char* fileName, const char* mode) {
	*pfp = fopen(fileName, mode);
	if (!(*pfp)) {
		printf("Failed to open file %s\n", fileName);
		exit(1);
	}
}
