/*
   Copyright (C) 2020 Benjamin G. Friedman. Code may be used or redistributed freely with credit given to the author.
   File Description:
		- name: hash_table.c
		- description: implementation file for the hash table interface.
   Contact: bfriedman12@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash_table.h"


typedef struct hash_table {
	NODE* hash_table;		// array of linked lists (chaining)
	unsigned capacity;		// capacity of hash table
} Hash_table;


HASH_TABLE ht_create(unsigned capacity) {
	Hash_table* ht = (Hash_table*)malloc(sizeof(*ht));
	if (ht) {
		if (!(ht->hash_table = (NODE*)malloc(sizeof(*(ht->hash_table)) * capacity))) {
			free(ht);
			return NULL;
		}
		for (unsigned i = 0; i < capacity; i++)
			ht->hash_table[i] = NULL;
		ht->capacity = capacity;
	}
	return (HASH_TABLE)ht;
}


Status ht_insert(HASH_TABLE hTable, char* key, int data) {
	Hash_table* ht = (Hash_table*)hTable;
	unsigned index = ht_hash(key, ht->capacity);
	return listHeadInsert(&(ht->hash_table[index]), key, data);
}


void ht_updateElement(HASH_TABLE hTable, char* key, unsigned index, int data) {
	Hash_table* ht = (Hash_table*)hTable;
	NODE nodeToUpdate = NULL;
	list_key_exists(ht->hash_table[index], key, &nodeToUpdate, NULL);
	list_updateExistingNode(nodeToUpdate, data);
}


Status ht_remove(HASH_TABLE hTable, char* key) {
	Hash_table* ht = (Hash_table*)hTable;
	unsigned index = ht_hash(key, ht->capacity);
	return list_remove(&(ht->hash_table[index]), key);
}


void ht_destroy(HASH_TABLE* phTable) {
	Hash_table* ht = (Hash_table*)*phTable;
	for (unsigned i = 0; i < ht->capacity; i++) {
		list_destroy(&(ht->hash_table[i]));
	}
	free(ht->hash_table);
	free(ht);
	*phTable = NULL;
}


unsigned ht_hash(char* key, unsigned capacity) {
	unsigned sum = 0;
	unsigned keyLen = strlen(key);
	for (unsigned i = 0; i < keyLen; i++)
		sum += key[i];
	return sum % capacity;
}


Boolean ht_keyExists(HASH_TABLE hTable, char* key, unsigned* pIndex, int* pData) {
	Hash_table* ht = (Hash_table*)hTable;
	unsigned index = ht_hash(key, ht->capacity);
	if (ht->hash_table[index] == NULL)
		return FALSE;
	if (list_key_exists(ht->hash_table[index], key, NULL, pData)) {
		if (pIndex != NULL)
			*pIndex = index;
		return TRUE;
	}
	if (pIndex != NULL)
		*pIndex = -1;
	return FALSE;
}


Status ht_getDataByKey(HASH_TABLE hTable, char* key, int* pData) {
	Hash_table* ht = (Hash_table*)hTable;
	unsigned index = ht_hash(key, ht->capacity);
	if (ht->hash_table[index] == NULL) {
		*pData = 0;
		return FAILURE;
	}
	return list_getDataByKey(ht->hash_table[index], key, pData);
}


void ht_print(HASH_TABLE hTable, FILE* fp) {
	Hash_table* ht = (Hash_table*)hTable;
	unsigned occupiedIndexes = 0;	// the number of occupied indexes in the hash table
	unsigned totalEntries = 0;		// the number of entries in the hash table
	unsigned listSize = 0;			// the size of a single linked list in the hash table
	for (unsigned i = 0; i < ht->capacity; i++) {
		fprintf(fp, "Index: %-5d", i);
		if (ht->hash_table[i] == NULL) {
			fprintf(fp, "NULL\n");
		}
		else {
			occupiedIndexes++;
			list_printHeadToTailOneLine(ht->hash_table[i], fp, &listSize);
			totalEntries += listSize;
		}
	}

	fprintf(fp, "\n\n\nHASH TABLE PERFORMANCE\n\n");
	fprintf(fp, "%-48s%u\n", "Hash Table Size:", ht->capacity);
	fprintf(fp, "%-48s%u\n", "Number of Occupied Indexes:", occupiedIndexes);
	fprintf(fp, "%-48s%u\n\n", "Number of Entries:", totalEntries);
	fprintf(fp, "%-48s%g%%\n", "Percentage of indexes occupied:", (double)occupiedIndexes / ht->capacity * 100);
	fprintf(fp, "%-48s%g\n", "Ratio of Total Entries to Hash Table Size:", (double)totalEntries / ht->capacity);
	fprintf(fp, "%-48s%g\n\n", "Average Number of Entries Per Occupied Index:", (double)totalEntries / occupiedIndexes);
}


Status ht_createListLTG(HASH_TABLE hTable, NODE* phNewList) {
	Hash_table* ht = (Hash_table*)hTable;

	// find the first linked list in the hash table and sort it
	unsigned firstIndex = 0;
	while (firstIndex < ht->capacity && ht->hash_table[firstIndex] == NULL)
		firstIndex++;
	// hash table was empty
	if (ht->hash_table[firstIndex] == NULL)
		return SUCCESS;
	// hash table has at least one linked list
	if (!list_copyList(ht->hash_table[firstIndex], phNewList))
		return FAILURE;
	list_mergesortLTG(phNewList);
	firstIndex++;


	/* take all nodes from the hash table and put them into one giant linked list
	   in order least to greatest lexicographic order. */
	for (unsigned i = firstIndex; i < ht->capacity; i++) {
		// index has no list. Continue to next index
		if (ht->hash_table[i] == NULL)
			continue;
		// index has a list. Insert list into giant list
		if (!list_combineListsLTG(phNewList, ht->hash_table[i])) {
			list_destroy(phNewList);
			return FAILURE;
		}
	}

	return SUCCESS;
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

	// copy the modified word back into unmodified word
	for (unsigned i = 0; i < index; i++)
		unmodifiedWord[i] = modifiedWord[i];
	unmodifiedWord[index] = '\0';

	return unmodifiedWord;
}
