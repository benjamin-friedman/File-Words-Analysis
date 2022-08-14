/*
  Author: Benjamin G. Friedman
  Date: 12/25/20
  File: LinkedList.h
  Description:
      - Header file for the linked list interface.
*/


#ifndef LIST_H
#define LIST_H


#include "Status.h"

typedef void* LINKED_LIST;    // opaque object handle


/*
PRECONDITION:
  - None.
POSTCONDITION:
  - Returns a handle to an empty linked list or NULL for any memory allocation failure.
*/
LINKED_LIST linkedListInitDefault(void);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
  - key and data are the key-data pair to be added to the linked list. It is not guaranteed that
    there is not already a node with the same key.
POSTCONDITION
  - Inserts the new key-data pair the at the head of the linked list (allows for duplicates).
  - Returns SUCCESS, else FAILURE for any memory allocation failure.
*/
Status linkedListHeadInsert(LINKED_LIST hLinkedList, const char* key, int data);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
  - key and data are the key-data pair for the hash table element to be inserted.
POSTCONDITION:
  - Inserts a new key-data pair in least-to-greatest lexicographic order of the keys (allows for duplicates).
  - Returns SUCCESS, else FAILURE for any memory allocation failure.
*/
Status linkedListInsertLTG(LINKED_LIST hLinkedList, const char* key, int data);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
  - key is a key that is not guaranteed to be in the linked list.
POSTCONDITION:
  - Returns TRUE if the key exists, else FALSE.
*/
Boolean linkedListKeyExists(LINKED_LIST hLinkedList, const char* key);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
  - key is the key of the data to be updated - it is not guaranteed to be in the linked list.
  - data is the new data that will override the previous data.
POSTCONDITION:
  - Item exists - Overrides the previous data of the key returns SUCCESS.
  - Item doesn't eixst - returns FAILURE.
*/
Status linkedListUpdateExistingKey(LINKED_LIST hLinkedList, const char* key, int data);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
  - key is not guaranteed to be in the linked list.
  - pStatus is a pointer to a status that could be NULL.
POSTCONDITION
  - key exists - Returns the data associated with the key and sets the Status pointed to by pStatus to SUCCESS.
  - key doesn't exist - Returns 0 and sets the Status pointed to by pStatus to FAILURE.
  - In either case, if pStatus is NULL it is ignored.
*/
int linkedListGetDataOfKey(LINKED_LIST hLinkedList, const char* key, Status* pStatus);


/*
PRECONDITION:
  - hLinkedList is a handle to a valid linked list.
POSTCONDITION:
  - Adds items from hLinkedListToAdd to hLinkedList in order of least to greatest
    lexicographic order of the keys.
  - Returns SUCCESS, else FAILURE for any memory allocation failure.
*/
Status linkedListCombineLTG(LINKED_LIST hLinkedList, LINKED_LIST hLinkedListToAdd);


/*
PRECONDITION
  - hLinkedList is a handle to a valid linked list.
POSTCONDITION:
  - Prints the list from head to tail with each item on its own line
        key: data
  - If empty, prints EMPTY LIST.
*/
void linkedListPrintHeadToTailMultiplelines(LINKED_LIST hLinkedList, FILE* fp);

/*
PRECONDITION
    - hLinkedList is a handle to a valid linked list.
POSTCONDITION:
  - Prints the list from head to tail with each list on a single line unless there
    are too many words to fit on a single line.
*/
void linkedListPrintHeadToTailSingleLine(LINKED_LIST hLinkedList, FILE* fp);


/*
PRECONDITION:
  - hLinkedList is a handle to a valid linked list.
POSTCONDITION
  - Returns the size of the linked list
*/
size_t linkedListGetSize(LINKED_LIST hLinkedList);


/*
PRECONDITION:
  - hLinkedList is a handle to a valid linked list.
POSTCONDITION
  - Returns TRUE if the linked list is empty, else FALSE.
*/
Boolean linkedListIsEmpty(LINKED_LIST hLinkedList);


/*
PRECONDITION:
  - phLinkedList is a pointer to a handle to a valid linked list.
POSTCONDITION:
  - Frees up all memory associated with the linked list and sets the handle to NULL.
*/
void linkedListDestroy(LINKED_LIST* phLinkedList);


#endif
