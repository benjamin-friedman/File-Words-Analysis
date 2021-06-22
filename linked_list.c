/*
   Copyright (C) 2020 Benjamin G. Friedman. Code may be used or redistributed freely with credit given to the author.
   File Description:
	- name: linked_list.c
	- description: implementation file for the linked list interface.
   Contact: bfriedman12@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"


typedef struct node {
	int data;						// number of words corresponding to the key
	char* key;						// key for hash table (i.e. the word)
	struct node *previous, *next;
} Node;


/* Helper Functions */
/* PRECONDITION: key and data are the key-data pair for a new node in the linked list.
   POSTCONDITION: returns the newly created node or NULL for any memory allocation failure. */
Node* createNode(const char* key, int data);

/* Functions to merge_sort a linked list */
Node* mergesort(Node* head);
Node* split(Node* head);
Node* merge(Node* first, Node* second);


Status listHeadInsert(NODE* phList, const char* key, int data) {
	Node** head = (Node**)phList;

	// Key doesn't already exist. Perform head insertion.
	Node* newHead = createNode(key, data);
	if (newHead == NULL)
		return FAILURE;

	// perform the head insert
	newHead->next = *head;
	if (*head != NULL)
		(*head)->previous = newHead;
	*head = newHead;

	return SUCCESS;
}


Status list_LTGInsert(NODE* phList, const char* key, int data) {
	Node** head = (Node**)phList;
	
	// create the new node
	Node* newNode = createNode(key, data);
	if (newNode == NULL)
		return FAILURE;

	// empty list
	if (*head == NULL) {
		*head = newNode;
	}
	// list size 1
	else if ((*head)->next == NULL) {
		if (strcmp(newNode->key, (*head)->key) < 0) {
			newNode->next = *head;
			(*head)->previous = newNode;
			*head = newNode;
		}
		else {
			newNode->previous = *head;
			(*head)->next = newNode;
		}
	}
	// list size > 1. Insert node in proper order
	else {
		Node* prev = NULL;
		Node* current = *head;
		Node* next = current->next;
		Boolean hasBeenInserted = FALSE;
		// continue while not at end of list and node hasn't been inserted
		while (current != NULL && !hasBeenInserted) {	
			// key of current node is larger. Insert between previous and current
			if (strcmp(newNode->key, current->key) < 0) {
				newNode->next = current;
				current->previous = newNode;
				newNode->previous = prev;
				if (prev != NULL)	// not a head insert
					prev->next = newNode;
				else    // head insert
					*head = newNode;
				hasBeenInserted = TRUE;
			}
			// key of current node is smaller. Continue searching
			else {
				prev = current;
				current = next;
				if (current != NULL)
					next = next->next;
			}
		}

		// larger than all keys in list, at the end of the list
		if (!hasBeenInserted) {
			newNode->previous = prev;
			prev->next = newNode;
		}
	}

	return SUCCESS;
}


void list_updateExistingNode(NODE hNode, int data) {
	Node* node = (Node*)hNode;
	node->data = data;
}


Status list_remove(NODE* phList, const char* key) {
	Node** head = (Node**)phList;

	// list size 0
	if (*head == NULL)
		return FAILURE;
	// list size 1
	else if ((*head)->next == NULL) {
		if (!strcmp((*head)->key, key)) {
			free((*head)->key);
			free(*head);
			*head = NULL;
			return SUCCESS;
		}
	}
	// list size > 1
	else {
		Node* previous = NULL;
		Node* current = *head;
		Node* next = (*head)->next;
		while (current != NULL) {
			// found node to be removed
			if (!strcmp(current->key, key)) {
				free(current->key);
				free(current);
				// node to be removed is head node
				if (previous == NULL) {
					next->previous = NULL;
					(*head) = next;
				}
				// node to be removed is tail node
				else if (next == NULL) {
					previous->next = NULL;
				}
				// node to be removed is in the middle
				else {
					previous->next = next;
					next->previous = previous;
				}
				return SUCCESS;
			}
			// continue searching for node to be removed
			else {
				previous = current;
				current = next;
				if (current != NULL)
					next = next->next;
			}
		}
		return FAILURE;
	}
	return FAILURE;
}


Status list_mergesortLTG(NODE* phList) {
	Node** pList = (Node**)phList;
	if (*pList == NULL)
		return FAILURE;

	*pList = mergesort(*pList);
	return SUCCESS;
}


Boolean list_isEmpty(NODE hList) {
	Node* head = (Node*)hList;
	return head == NULL;
}


Boolean list_key_exists(NODE hList, const char* key, NODE* pFoundNode, int* pData) {
	Node* head = (Node*)hList;
	Node** foundNode = (Node**)pFoundNode;
	Node* current = head;

	while (current != NULL) {
		if (!strcmp(current->key, key)) {
			if (foundNode != NULL)
				*foundNode = current;
			if (pData != NULL)
				*pData = current->data;
			return TRUE;
		}
		current = current->next;
	}

	if (foundNode != NULL)
		*foundNode = NULL;
	if (pData != NULL)
		*pData = 0;
	return FALSE;
}


Status list_getDataByKey(NODE hList, const char* key, int* pData) {
	Node* head = (Node*)hList;
	Node* current = head;
	while (current != NULL) {
		if (!strcmp(current->key, key)) {
			*pData = current->data;
			return SUCCESS;
		}
		current = current->next;
	}
	*pData = 0;
	return FAILURE;
}


Status list_combineListsLTG(NODE* phListToAddTo, NODE hList) {
	Node* current = (Node*)hList;	// first node of list which is being added into the larger list
	while (current != NULL) {
		if (!list_LTGInsert(phListToAddTo, current->key, current->data))
			return FAILURE;
		current = current->next;
	}
	return SUCCESS;
}


Status list_printHeadToTail(NODE hList, FILE* fp) {
	Node* head = (Node*)hList;
	if (head == NULL) {
		printf("Error. List is empty. Cannot print.\n");
		return FAILURE;
	}

	Node* current = head;
	while (current != NULL) {
		fprintf(fp, "%-20s%d\n", current->key, current->data);
		current = current->next;
	}
	return SUCCESS;
}


Status list_printHeadToTailOneLine(NODE hList, FILE* fp, unsigned* pListSize) {
	Node* head = (Node*)hList;
	if (head == 0) {
		printf("Error. List is empty. Cannot print.\n");
		return FAILURE;
	}

	Node* current = head;
	char buffer[100];
	char spacing[] = "\n            ";
	unsigned count = 0;
	unsigned listSize = 0;
	while (current != NULL) {
		listSize++;
		sprintf_s(buffer, 50, "%s %d", current->key, current->data);
		fprintf(fp, "%-15s", buffer);
		count++;
		if (count == 5 && current->next != NULL) {
			fprintf(fp, "%s", spacing);
			count = 0;
		}
		current = current->next;
	}

	fprintf(fp, "\n");
	*pListSize = listSize;
	return SUCCESS;
}


Status list_debug(NODE hList) {
	Node* head = (Node*)hList;
	if (head == NULL) {
		printf("Error. List is empty. Cannot print.\n");
		return FAILURE;
	}

	Node* current = head;
	while (current != NULL) {
		if (current->previous == NULL) printf("Previous: NULL\n");
		else printf("Previous: %p\n", current->previous);
		printf("Current: %p  Data: %d  Key: %s\n", current, current->data, current->key);
		if (current->next == NULL) printf("Next: NULL\n");
		else printf("Next: %p\n\n", current->next);
		current = current->next;
	}
	printf("\n");
	return SUCCESS;
}


Status list_copyList(NODE hListToCopy, NODE* phNewList) {
	Node* copyHead = (Node*)hListToCopy;
	Node** pNewHead = (Node**)phNewList;

	// destroy list if it exists to make room for the list to be copied.
	if (*pNewHead != NULL) {
		list_destroy(phNewList);
	}

	// list to be copied is empty
	if (copyHead == NULL) {
		*pNewHead = NULL;
		return SUCCESS;
	}

	// list to be copied is not empty
	if (!(*pNewHead = createNode(copyHead->key, copyHead->data)))
		return FAILURE;

	Node* nodeToCopy = copyHead->next;	// tracks the nodes to be copied
	Node* newNode;						// holds each new node created
	Node* prev = *pNewHead;				// used for tail insert operation
	while (nodeToCopy != NULL) {
		if (!(newNode = createNode(nodeToCopy->key, nodeToCopy->data))) {
			list_destroy(phNewList);
			return FAILURE;
		}
		newNode->previous = prev;
		prev->next = newNode;
		prev = newNode;
		nodeToCopy = nodeToCopy->next;
	}

	return SUCCESS;
}


void list_destroy(NODE* phList) {
	Node* head = (Node*)*phList;
	if (head != NULL) {
		Node* current = head;
		Node* next = current->next;
		free(current->key);
		free(current);
		while (next != NULL) {
			current = next;
			next = next->next;
			free(current->key);
			free(current);
		}
		*phList = NULL;
	}
}




/* Helper Functions */
Node* createNode(const char* key, int data) {
	// create the node
	Node* newNode = (Node*)malloc(sizeof(*newNode));
	if (newNode == NULL)
		return FAILURE;
	unsigned keyLen = strlen(key);
	if (!(newNode->key = malloc(sizeof(*(newNode->key)) * (keyLen + 1)))) {
		free(newNode);
		return FAILURE;
	}

	// copy over the key and data
	for (unsigned i = 0; i < keyLen; i++)
		newNode->key[i] = key[i];
	newNode->key[keyLen] = '\0';
	newNode->data = data;
	newNode->previous = NULL;
	newNode->next = NULL;

	return newNode;
}


Node* mergesort(Node* head) {
	if (head == NULL || head->next == NULL)
		return head;
	Node* second = split(head);

	// recursion for the left and right halves
	head = mergesort(head);
	second = mergesort(second);

	// merge the two sorted halves
	return merge(head, second);
}


Node* merge(Node* first, Node* second) {
	// first list is empty
	if (first == NULL)
		return second;

	// second list is empty
	if (second == NULL)
		return first;

	// choose smaller value
	if (strcmp(first->key, second->key) < 0) {
		first->next = merge(first->next, second);
		first->next->previous = first;
		first->previous = NULL;
		return first;
	}
	else {
		second->next = merge(first, second->next);
		second->next->previous = second;
		second->previous = NULL;
		return second;
	}
}


Node* split(Node* head) {
	Node* fast = head;
	Node* slow = head;

	while (fast->next && fast->next->next) {
		fast = fast->next->next;
		slow = slow->next;
	}

	Node* temp = slow->next;
	slow->next = NULL;
	return temp;
}
