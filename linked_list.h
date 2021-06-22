/*
   Copyright (C) 2020 Benjamin G. Friedman. Code may be used or redistributed freely with credit given to the author.
   File Description:
	- name: linked_list.h
	- description: header file for the linked list interface.
   Contact: bfriedman12@gmail.com
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "status.h"

typedef void* NODE;	// opaque object handle


/* PRECONDITION: phList is a pointer to a handle to a linked list that is empty (NULL) or exists.
   POSTCONDITION: inserts new key-data pair the at the head and returns SUCCESS.
   Allows for duplicates. Returns FAILURE for any memory allocation failure. */
Status listHeadInsert(NODE* phList, char* key, int data);


/* PRECONDITION: phList is a pointer to a handle to a linked list that is empty (NULL) or exists. key and data are the
   key-data pair for the hash table element to be inserted.
   POSTCONDITION: inserts a new key-data pair in least-to-greatest lexicographic order and
   returns SUCCESS. Returns FAILURE for any memory allocation failure */
Status list_LTGInsert(NODE* phList, char* key, int data);


/* PRECONDITION: hNode is a handle to a node that exists in the linked list.
   POSTCONDITION: the nodes data is updated with "data" */
void list_updateExistingNode(NODE hNode, int data);


/* PRECONDITION: phList is a pointer to a handle to a linked list that is empty (NULL) or exists, and
   key is the key to be removed.
   POSTCONDITION: if the key was found, its corresponding node is removed and returns SUCCESS.
   If the key does not exist or the list is empty it returns FAILURE. */
Status list_remove(NODE* phList, char* key);


/* PRECONDITION: phList is a pointer to a handle to a linked list that is empty (NULL) or exists.
   POSTCONDITION: sorts the list in order least to greatest using the lexicographical values of
   the keys and returns SUCCESS. Returns FAILURE if the list is empty. Uses merge sort. */
Status list_mergesortLTG(NODE* phList);


/* PRECONDITION: hList is a handle to a linked list that is empty (NULL) or exists.
   POSTCONDITION: if the list exists, returns TRUE. Else returns FAILURE. */
Boolean list_isEmpty(NODE hList);


/* PRECONDITION: hList is a handle to a linked list. key is the key to be searched for, pFoundNode
   is a pointer to a node handle or is NULL.
   POSTCONDITION: if the key exists, stores the address of the node in pFoundNode, stores its
   data in the integer pointed to by pData and returns TRUE. If the key doesn't exist, NULL and 0
   are stored in pFoundNode and the integer pointed to by pData respectively and returns FALSE.
   In either case, if pFoundNode was passed NULL it will be ignored. */
Boolean list_key_exists(NODE hList, char* key, NODE* pFoundNode, int* pData);


/* PRECONDITION: hList is a handle to a linked list and key is the key to search for.
   POSTCONDITION: if the key is found, its data is stored in the integer pointed to by pData
   and returns FAILURE. */
Status list_getDataByKey(NODE hList, char* key, int* pData);


/* PRECONDITION: hList is a handle to a linked list that is empty (NULL) or exists. phListToAddTo
   is a pointer to a handle to a linked list the is empty (NULL) or exists. If it exists, the
   nodes are already sorted in order of least to greatest according to lexicographic order of
   the keys.
   POSTCONDITION: adds all of the entries from hList into the lilnked list pointed to by
   phListToAddTo and returns SUCCESS. Returns FAILURE for any memory allocation failure.
   For clarity, if hList wrere NULL the function would return SUCCESS without anything happening.*/
Status list_combineListsLTG(NODE* phListToAddTo, NODE hList);


/* PRECONDITION: hList is a handle to a linked list that is empty (NULL) or exists. fp is to print
   to a file or stdout. pList size is a pointer to a number that will hold the list size
   POSTCONDITION: prints the list in order of head to tail and returns SUCCESS. If the list
   is empty it prints an error message and returns FAILURE. The variable pointed to by pListSize
   holds the size of the linked list.
   Version one prints each node on its own line.
   Version two prints all nodes on one line. Used in ht_print. */
Status list_printHeadToTail(NODE hList, FILE* fp);
Status list_printHeadToTailOneLine(NODE hList, FILE* fp, unsigned *pListSize);


/* PRECONDITION: hList is a handle to a linked list that is empty (NULL) or exists.
   POSTCONDITION: displays all info about each node for debugging purposes and returns SUCCESS. This
   includes the address of the previous/current/next node and the key-data pair in the current node.
   Returns FAILURE and prints an error mesage if the list is empty. */
Status list_debug(NODE hList);


/* PRECONDITION: hListToCopy is a handle to a linked list. phNewList is a pointer to a handle to a linked list.
   POSTCONDITION: a deep copy of hListToCopy is made and stored in the handle pointed to by phNewList and returns
   SUCCESS. Any list that was previously existing in the handle pointed to by phNewList is first destroyed.
   Returns FAILURE for any memory allocation failures. */
Status list_copyList(NODE hListToCopy, NODE* phNewList);


/* PRECONDITION: phList is a pointer to a handle to a linked that is empty (NULL) or exists.
   POSTCONDITION: frees all memory associated with the linked list and sets the handle pointed
   to by phList to NULL. */
void list_destroy(NODE* phList);

#endif
