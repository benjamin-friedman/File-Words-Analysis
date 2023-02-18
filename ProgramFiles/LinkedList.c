/*
  Author: Benjamin G. Friedman
  Date: 08/11/2022
  File: LinkedList.c
  Description:
	  - Implementation file for the linked list interface
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"


/***** Structures *****/
typedef struct node {
	int data;             // number of words corresponding to the key
	char* key;            // key for hash table (i.e. the word)
	struct node* prev;    // previous/next pointers
	struct node* next;
} Node;


typedef struct linkedList {
	Node* head;
	Node* tail;
	size_t size;
} LinkedList;




/***** Helper functions use only in this file *****/
/*
PRECONDITION
  - key and data are the key-data pair for a new node in the linked list.
POSTCONDITION
  - Returns the newly created node or NULL for any memory allocation failure.
*/
static Node* createNode(const char* key, int data);




/***** Functions declared in LinkedList.h *****/
LINKED_LIST linkedListInitDefault(void) {
	LinkedList* pLinkedList = malloc(sizeof(*pLinkedList));
	if (pLinkedList) {
		pLinkedList->head = pLinkedList->tail = NULL;
		pLinkedList->size = 0;
	}
	return pLinkedList;
}



Status linkedListHeadInsert(LINKED_LIST hLinkedList, const char* key, int data) {
	LinkedList* pLinkedList = hLinkedList;

	// create new node
	Node* newHead = createNode(key, data);
	if (!newHead)
		return FAILURE;

	// head insert
	newHead->next = pLinkedList->head;
	if (!pLinkedList->head)    // empty list
		pLinkedList->tail = newHead;
	else                       // non-empty list
		pLinkedList->head->prev = newHead;
	pLinkedList->head = newHead;
	++pLinkedList->size;

	return SUCCESS;
}



Status linkedListInsertLTG(LINKED_LIST hLinkedList, const char* key, int data) {
	LinkedList* pLinkedList = hLinkedList;

	// create new node
	Node* newNode = createNode(key, data);
	if (newNode == NULL)
		return FAILURE;

	// empty list
	if (!pLinkedList->head) {
		pLinkedList->head = pLinkedList->tail = newNode;
		return SUCCESS;
	}

	// non-empty list
	Node* cur = pLinkedList->head;
	Node* prev = NULL;
	Boolean hasBeenInserted = FALSE;
	// continue while not at end of list and key hasn't been inserted
	while (cur && !hasBeenInserted) {
		// key of current node is larger, insert between previous and current
		if (strcmp(newNode->key, cur->key) < 0) {
			newNode->next = cur;
			cur->prev = newNode;
			if (prev)    // not a head insert
				prev->next = newNode;
			else         // head insert
				pLinkedList->head = newNode;
			hasBeenInserted = TRUE;
		}
		// key of current node is smaller, continue searching
		else {
			prev = cur;
			cur = cur->next;
		}
	}

	// larger than all keys in the list, insert at end
	if (!hasBeenInserted) {
		newNode->prev = prev;
		prev->next = newNode;
		pLinkedList->tail = newNode;
	}

	++pLinkedList->size;
	return SUCCESS;
}



Boolean linkedListKeyExists(LINKED_LIST hLinkedList, const char* key) {
	LinkedList* pLinkedList = hLinkedList;
	Node* cur = pLinkedList->head;

	while (cur) {
		if (!strcmp(cur->key, key))
			return TRUE;
		cur = cur->next;
	}

	return FALSE;
}



Status linkedListUpdateExistingKey(LINKED_LIST hLinkedList, const char* key, int data) {
	LinkedList* pLinkedList = hLinkedList;
	Node* cur = pLinkedList->head;

	if (!cur)                       // empty list
		return FAILURE;

	while (strcmp(cur->key, key)) 	// find key if it exists
		cur = cur->next;

	if (cur) {                     	// existed
		cur->data = data;
		return SUCCESS;
	}

	return FAILURE;                	// didn't exist
}



int linkedListGetDataOfKey(LINKED_LIST hLinkedList, const char* key, Status* pStatus) {
	LinkedList* pLinkedList = hLinkedList;
	Node* cur = pLinkedList->head;

	while (cur) {
		if (!strcmp(cur->key, key)) {
			if (pStatus)
				*pStatus = SUCCESS;
			return cur->data;
		}
		cur = cur->next;
	}

	if (pStatus)
		*pStatus = FAILURE;
	return 0;
}



Status linkedListCombineLTG(LINKED_LIST hLinkedList, LINKED_LIST hLinkedListToAdd) {
	LinkedList* pLinkedListToAdd = hLinkedListToAdd;
	Node* curLinkedListToAdd = pLinkedListToAdd->head;

	while (curLinkedListToAdd) {
		if (!linkedListInsertLTG(hLinkedList, curLinkedListToAdd->key, curLinkedListToAdd->data))
			return FAILURE;
		curLinkedListToAdd = curLinkedListToAdd->next;
	}

	return SUCCESS;
}



void linkedListPrintHeadToTailMultiplelines(LINKED_LIST hLinkedList, FILE* fp) {
	LinkedList* pLinkedList = hLinkedList;
	Node* cur = pLinkedList->head;

	if (!cur)
		fprintf(fp, "EMPTY LIST\n");

	while (cur) {
		fprintf(fp, "%-20s%d\n", cur->key, cur->data);
		cur = cur->next;
	}
}



void linkedListPrintHeadToTailSingleLine(LINKED_LIST hLinkedList, FILE* fp) {
	LinkedList* pLinkedList = hLinkedList;

	Node* cur = pLinkedList->head;
	char buffer[100];
	char spacing[] = "\n            ";
	int count = 0;
	while (cur) {
		sprintf(buffer, "%s %d", cur->key, cur->data);
		fprintf(fp, "%-22s", buffer);
		++count;
		if (count == 5 && cur->next != NULL) {
			fprintf(fp, "%s", spacing);
			count = 0;
		}
		cur = cur->next;
	}
	fprintf(fp, "\n");
}



size_t linkedListGetSize(LINKED_LIST hLinkedList) {
	LinkedList* pList = hLinkedList;
	return pList->size;
}



Boolean linkedListIsEmpty(LINKED_LIST hLinkedList) {
	LinkedList* pList = hLinkedList;
	return pList->head == NULL;
}



void linkedListDestroy(LINKED_LIST* phLinkedList) {
	LinkedList* pLinkedList = *phLinkedList;
	Node* cur = pLinkedList->head;
	Node* next;

	while (cur) {
		next = cur->next;
		free(cur->key);
		free(cur);
		cur = next;
	}

	free(pLinkedList);
	*phLinkedList = NULL;
}




/***** Helper functions used only in this file *****/
static Node* createNode(const char* key, int data) {
	// create the node
	Node* newNode = (Node*)malloc(sizeof(*newNode));
	if (newNode == NULL)
		return NULL;
	size_t keyLen = strlen(key);
	if (!(newNode->key = malloc(sizeof(*(newNode->key)) * (keyLen + 1)))) {
		free(newNode);
		return NULL;
	}

	// copy over the key and data
	for (size_t i = 0; i < keyLen; ++i)
		newNode->key[i] = key[i];
	newNode->key[keyLen] = '\0';
	newNode->data = data;
	newNode->prev = NULL;
	newNode->next = NULL;

	return newNode;
}
