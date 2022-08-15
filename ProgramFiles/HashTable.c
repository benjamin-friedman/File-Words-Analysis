/*
  Author: Benjamin G. Friedman
  Date: 08/11/2022
  File: HashTable.c
  Description:
	  - Implementation file for the hash table interface.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "HashTable.h"


typedef struct hash_table {
	LINKED_LIST* linkedLists;    // array of linked lists (chaining)
	size_t capacity;             // capacity of hash table
} Hash_table;




/***** Helper functions *****/
/*
PRECONDITION
  - key is the key to be hashed.
  - capacity is the capacity of the hash table.
POSTCONDITION
  - Returns an index within the range of the hash table.
  - The hash function used is the same as Java's hashCode() function since it's demonstrated to have relatively good
    performance.
*/
static size_t hash(const char* key, size_t capacity);




HASH_TABLE hashTableInitWithCapacity(size_t capacity) {
	Hash_table* pHash_table = malloc(sizeof(*pHash_table));
	if (pHash_table) {
		if (!(pHash_table->linkedLists = malloc(sizeof(*(pHash_table->linkedLists)) * capacity))) {
			free(pHash_table);
			return NULL;
		}
		for (size_t i = 0; i < capacity; ++i) {
			pHash_table->linkedLists[i] = linkedListInitDefault();
			if (!pHash_table->linkedLists[i]) {
				for (int j = i - 1; j >= 0; --j)
					linkedListDestroy(&pHash_table->linkedLists[j]);
				free(pHash_table);
				return NULL;
			}
		}
		pHash_table->capacity = capacity;
	}

	return pHash_table;
}


Status hashTableInsert(HASH_TABLE hHashTable, const char* key, int data) {
	Hash_table* pHashTable = hHashTable;

	size_t index = hash(key, pHashTable->capacity);

	// item already exists
	if (linkedListKeyExists(pHashTable->linkedLists[index], key)) {
		linkedListUpdateExistingKey(pHashTable->linkedLists[index], key, data);
		return SUCCESS;
	}

	// item doesn't already exist
	return linkedListHeadInsert(pHashTable->linkedLists[index], key, data);
}


Boolean hashTableKeyExists(HASH_TABLE hHashTable, const char* key) {
	Hash_table* pHashTable = hHashTable;
	return linkedListKeyExists(pHashTable->linkedLists[hash(key, pHashTable->capacity)], key);
}


int hashTablGetDataOfKey(HASH_TABLE hHashTable, const char* key, Status* pStatus) {
	Hash_table* pHashTable = hHashTable;
	
	if (hashTableKeyExists(hHashTable, key)) {
		if (pStatus)
			*pStatus = SUCCESS;
		return linkedListGetDataOfKey(pHashTable->linkedLists[hash(key, pHashTable->capacity)], key, NULL);
	}

	if (pStatus)
		*pStatus = FAILURE;

	return 0;
}


Status hashTableGetCapacity(HASH_TABLE hHashTable) {
	Hash_table* pHashTable = hHashTable;
	return pHashTable->capacity;
}


Status hashTableCreateLinkedListLTG(HASH_TABLE hHashTable, LINKED_LIST hLinkedList) {
	Hash_table* pHashTable = hHashTable;

	// find the first non-empty linked list
	size_t i = 0;
	while (linkedListIsEmpty(pHashTable->linkedLists[i]))
		++i;

	// empty hash table
	if (i == pHashTable->capacity)
		return SUCCESS;

	// non-empty hash table
	for (; i < pHashTable->capacity; ++i) {
		if (!linkedListCombineLTG(hLinkedList, pHashTable->linkedLists[i]))
			return FAILURE;
	}

	return SUCCESS;
}


void hashTablePrint(HASH_TABLE hHashTable, FILE* fp) {
	Hash_table* pHashTable = (Hash_table*)hHashTable;
	unsigned occupiedIndexes = 0;    // the number of occupied indexes in the hash table
	unsigned totalEntries = 0;		 // the number of entries in the hash table
	for (size_t i = 0; i < pHashTable->capacity; ++i) {
		fprintf(fp, "Index: %-5zu", i);
		linkedListPrintHeadToTailSingleLine(pHashTable->linkedLists[i], fp);
		totalEntries += linkedListGetSize(pHashTable->linkedLists[i]);
		if (!linkedListIsEmpty(pHashTable->linkedLists[i]))
			++occupiedIndexes;
	}

	fprintf(fp, "\n\n\nHASH TABLE PERFORMANCE\n\n");
	fprintf(fp, "%-48s%zu\n", "Hash Table Size:", pHashTable->capacity);
	fprintf(fp, "%-48s%u\n", "Number of Occupied Indexes:", occupiedIndexes);
	fprintf(fp, "%-48s%u\n", "Number of Entries:", totalEntries);
	fprintf(fp, "%-48s%u\n\n", "Number of Collisions:", totalEntries - occupiedIndexes);
	fprintf(fp, "%-48s%g%%\n", "Percentage of indexes occupied:", (double)occupiedIndexes / pHashTable->capacity * 100);
	fprintf(fp, "%-48s%g\n", "Ratio of Total Entries to Hash Table Size:", (double)totalEntries / pHashTable->capacity);
	fprintf(fp, "%-48s%g\n", "Average Number of Entries Per Occupied Index:", (double)totalEntries / occupiedIndexes);
	if (totalEntries != occupiedIndexes)
		fprintf(fp, "%-48s%g\n\n", "Entries Per Collision:", (double)totalEntries / (totalEntries - occupiedIndexes));
	else
		fprintf(fp, "%-48s0", "Entries Per Collision:");
}


char* hashTablemodifyWord(char* unmodifiedWord) {
	size_t unmodifiedWordLength = strlen(unmodifiedWord);
	char modifiedWord[100] = { '\0' };
	size_t index = 0;
	char* apostrophe = "'";
	Boolean needsToBeModified = FALSE;

	// edge case for standalone punctuation (? : ; etc.)
	if (unmodifiedWordLength == 1 && !isalpha(unmodifiedWord[0])) {
		unmodifiedWord[0] = '\0';
		return unmodifiedWord;
	}

	// check if word needs to be modified. Alphabetic characters and apostrophes are ok.
	for (size_t i = 0; i < unmodifiedWordLength; ++i) {
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
	for (size_t i = 0; i < unmodifiedWordLength; ++i) {
		// alphabetic letter
		if (isalpha(unmodifiedWord[i]))
			modifiedWord[index++] = tolower(unmodifiedWord[i]);
		// apostrophes are ok
		else if (unmodifiedWord[i] == apostrophe[0])
			modifiedWord[index++] = unmodifiedWord[i];
	}

	// copy the modified word back into unmodified word
	for (size_t i = 0; i < index; ++i)
		unmodifiedWord[i] = modifiedWord[i];
	unmodifiedWord[index] = '\0';

	return unmodifiedWord;
}


void hashTableDestroy(HASH_TABLE* phHashTable) {
	Hash_table* pHashTable = *phHashTable;
	for (size_t i = 0; i < pHashTable->capacity; ++i)
		linkedListDestroy(&pHashTable->linkedLists[i]);
	free(pHashTable->linkedLists);
	free(pHashTable);
	*phHashTable = NULL;
}




/***** Helper functions *****/
static size_t hash(const char* key, size_t capacity) {
	unsigned sum = 0;
	unsigned a = 31;
	size_t keyLen = strlen(key);
	unsigned partialExp = 1;

	for (int i = keyLen - 1; i >= 0; i--) {
		sum += key[i] * partialExp;
		partialExp *= a;
	}

	return sum % capacity;
}
