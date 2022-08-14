/*
  Author: Benjamin G. Friedman
  Date: 12/25/20
  File: HashTable.h
  Description:
      - Header file for the hash table interface.
*/


#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include "LinkedList.h"

typedef void* HASH_TABLE;        // opaque object handle


/*
PRECONDITION
  - capacity is an integer greater than 0.
POSTCONDITION
  - Returns a handle to an empty hash table with the given capacity, else NULL for any memory allocation failure.
*/
HASH_TABLE hashTableInitWithCapacity(size_t capacity);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
  - key and data are the key-data pair to be inserted into the hash table. The key is not guaranteed to be in the hash table.
POSTCONDITION
  - key doesn't exist - Inserts the key data-pair into the hash table.
  - key already exists - Overrides the previous data of the key.
  - Returns SUCCESS, else FAILURE for memory allocation failure (only possible when the key doesn't already exist).
*/
Status hashTableInsert(HASH_TABLE hHashTable, const char* key, int data);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
  - key is not guaranteed to be in the hash table.
POSTCONDITION
  - Returns TRUE if the key exists, else FALSE.
*/
Boolean hashTableKeyExists(HASH_TABLE hHashTable, const char* key);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
  - key is not guaranteed to be in the hash table.
  - pStatus is a pointer to a status that could be NULL.
POSTCONDITION
  - key exists - Returns the data associated with the key and sets the Status pointed to by pStatus to SUCCESS.
  - key doesn't exist - Returns 0 and sets the Status pointed to by pStatus to FAILURE.
  - In either case, if pStatus is NULL it is ignored.
*/
int hashTablGetDataOfKey(HASH_TABLE hHashTable, const char* key, Status* pStatus);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
POSTCONDITION
  - Returns the capacity of the hash table.
*/
Status hashTableGetCapacity(HASH_TABLE hHashTable);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
  - hLinkedList is a handle to a valid linked list.
POSTCONDITION
  - Takes all of the items in the hash table and stores them in order of least-to-greatest
    lexicographic order of the keys in hLinkedList.
  - Returns SUCCESS, else FAILURE for any memory allocation failure.
*/
Status hashTableCreateLinkedListLTG(HASH_TABLE hHashTable, LINKED_LIST hLinkedList);


/*
PRECONDITION
  - hHashTable is a handle to a valid hash table.
  - fp is the file stream to print to.
POSTCONDITION
  - Prints out the hash table for debugging purposes and info about its performance at the end of the output.
*/
void hashTablePrint(HASH_TABLE hHashTable, FILE* fp);


/*
PRECONDITION
  - unmodifiedWord is a string that is anything other than the empty string.
POSTCONDITION
  - If the word does not need to be modified, it is left unchanged and returned as it.
  - If the word needs to be modified, it is changed and the modified version is returned.
  - If the c-string is a standalone punctuation character, it is changed to the empty string.
  - If the c-string is a word with punctuation characters in it (like word; or word) or word?) then the punctuation characters are
    removed. The only acceptable puncutation character is the apostrophe: words, word's, and words' are all considered different words.
*/
char* hashTablemodifyWord(char* unmodifiedWord);


/*
PRECONDITION
  - phHashTable is a pointer to a handle to a valid hash table.
POSTCONDITION
  - Frees all memory associated with the hash table and sets the handle to NULL.
*/
void hashTableDestroy(HASH_TABLE* phHashTable);


#endif
