/*
   Copyright (C) 2020 Benjamin G. Friedman. Code may be used or redistributed freely with credit given to the author.
   File Description:
		- name: hash_table.h
		- description: header file for the hash table interface.
   Contact: bfriedman12@gmail.com
*/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "status.h"
#include "linked_list.h"

typedef void* HASH_TABLE;		// opaque object handle


/* PRECONDITION: capacity is an integer greater than 0.
   POSTCONDITION: returns a handle to an empty hash table with the given capacity.
   Returns NULL upon any memory allocation failiure. */
HASH_TABLE ht_create(unsigned capacity);


/* PRECONDITION: hTable is a handle to a valid hash table, key and data are the key-data
   pair to be inserted into the hash table.
   POSTCONDITION: inserts the key data pair into the hash table. If the key already exists, the
   previous data will get overriden. Returns SUCCESS in either case. 
   Returns FAILURE for memory allocation failure. */
Status ht_insert(HASH_TABLE hTable, const char* key, int data);


/* PRECONDITION: hTable is a handle to a valid hash table, key is a key that already exists in 
   the hash table and will get updated, index is the index of the linked list in the hash table
   where the key exists, and data is the new piece of data. Use ht_key_exists to first ensure the
   key already exists. 
   POSTCONDITION: the data corresponding to the key gets updated. */
void ht_updateElement(HASH_TABLE hTable, const char* key, unsigned index, int data);


/* PRECONDITION: hTable is a handle to a valid hash table, key is the key to be looked for.
   POSTCONDITION: if the key exists, returns SUCCESS and removes the key-data pair.
   Returns FAILURE if the key doesn't exist. */
Status ht_remove(HASH_TABLE hTable, const char* key);


/* PRECONDITION: phTable is a pointer to a handle to a hash table.
   POSTCONDITION: all memory associated with the hash table has been freed
   and the handle has been set to NULL. */
void ht_destroy(HASH_TABLE* phTable);


/* PRECONDITION: key is the key to be hashed, capacity is the capacity of the hash table.
   POSTCONDITION: returns an index within the range of the hash table. For example,
   if the hash table is size 1000 then the index returned is 0 - 999 inclusive. The hash function 
   used is the same as Java's hashCode() function since it's demonstrated to have relatively good
   performance. */
unsigned ht_hash(const char* data, unsigned capacity);


/* PRECONDITION: hTable is a handle to a valid hash table, key is the key to be looked for.
   POSTCONDITION: if the key exists in the hash table, it stores the index of the key-data pair
   in the integer pointed to by pIndex, its data value in the integer pointed to by pData, and
   returns SUCCESS. If the key doesn't exist, -1 and 0 are stored in the integers pointed to by
   pIndex and pData respectively and returns FALSE. */
Boolean ht_keyExists(HASH_TABLE hTable, const char* key, unsigned* pIndex, int* pData);


/* PRECONDITION: hTable is a handle to a valid hash table, key is the key to be looked for.
   POSTCONDITION: if the key exists, its data value is stored in the integer pointed to by pData
   and returns SUCCESS. If the key doesn't exist, a 0 is stored in the integer pointed to by pData
   and returns FAILURE. */
Status ht_getDataByKey(HASH_TABLE hTable, const char* key, int* pData);


/* PRECONDITION: hTable is a handle to a valid hash table. fp is for a file or stdout.
   POSTCONDITION: prints out the hash table for debugging purposes and info about its
   performance at the end of the output. */
void ht_print(HASH_TABLE hTable, FILE* fp);


/* PRECONDITION: hTable is a handle to a valid hash table, phNewList is a pointer to a handle to
   a linked list that can be empty (NULL) or already have entries in it.
   POSTCONDITION: a deep copy of all of the key-data pairs in the hash table have been stored in
   the handle pointed to by phNewList and returns SUCCESS. Returns FAILURE for any memory allocation
   failure. */
Status ht_createListLTG(HASH_TABLE hTable, NODE* phNewList);


/* PRECONDITION: unmodifiedWord is a c-string that is anything other than the empty string.
   POSTCONDITION: if the word does not need to be modified, it is left unchanged and returned as it.
   If the word needs to be modified, it is changed and the modified version is returned. If the c-string
   is a standalone punctuation character, it is changed to the empty string. If the c-string is a word
   with punctuation characters in it (like word; or word) or word?) then the punctuation characters are
   removed. The only acceptable puncutation character is the apostrophe: words, word's, and words' are
   all considered different words. */
char* modifyWord(char* unmodifiedWord);

#endif
