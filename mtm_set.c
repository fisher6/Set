/*
 * mtm_set.c
 *
 *  Created on: Aug 18, 2015
 *      Author: Gal
 */

#include "mtm_set.h"
#include <stdio.h> // !!!! TODO: might need to remove !!!!
#include <stdlib.h>
#include <assert.h>

#define IF_NULL_RETURN_NULL(var) { \
		if ( (var) == NULL) return NULL; }

#define IF_NULL_RETURN_SET_NULL_ARGUMENT(var) { \
		if ( (var) == NULL) return SET_NULL_ARGUMENT; }

#define IS_SET_VALID(set) assert(set != NULL);
/*assert ( (set)->dummy != NULL && set->size >= 0 && (set)->copyFunc != NULL \
		&& (set)->freeFunc != NULL && (set)->cmpFunc != NULL );*/

/* The set implementation will use a list, where every node saves setElement */
struct Node_t {
	SetElement data;
	struct Node_t* next;
};

typedef struct Node_t* Node;

struct Set_t {
	Node dummy;
	Node current;
	int size;
	copySetElements copyFunc;
	freeSetElements freeFunc;
	compareSetElements cmpFunc;
};

Set setCreate(copySetElements copyElement, freeSetElements freeElement,
		compareSetElements compareElements) {
	// if one of the client's function pointers is NULL return SET_NULL_ARGUMENT
	IF_NULL_RETURN_NULL(copyElement)
	IF_NULL_RETURN_NULL(freeElement)
	IF_NULL_RETURN_NULL(compareElements)
	Set set = malloc(sizeof(*set)); // allocated memory for the new set
	IF_NULL_RETURN_NULL(set)
	set->dummy = malloc(sizeof(*set->dummy)); // allocate memory for the list
	if (set->dummy == NULL) { // if allocation fails free previous malloced mem
		free(set);
		return NULL;
	}
	set->dummy->data = NULL; // list is allocated, assign NULL to dummy node's data
	set->dummy->next = NULL; // dummy's next is NULL
	set->current = NULL; // current (set's iterator) is NULL when undefined
	set->copyFunc = copyElement;
	set->freeFunc = freeElement;
	set->cmpFunc = compareElements;
	set->size = 0;
	return set;
}

Set setCopy(Set set) {
	IF_NULL_RETURN_NULL(set)
	IS_SET_VALID(set)
	Set newSet = setCreate(set->copyFunc, set->freeFunc, set->cmpFunc);
	IF_NULL_RETURN_NULL(newSet)
	Node nodeToCopy = set->dummy->next;
	Node lastCopiedNode = newSet->dummy;
	newSet->current = NULL; // in case current was undefined in original set
	while (nodeToCopy != NULL) {
		Node currNode = malloc(sizeof(*currNode));
		if (currNode == NULL) {
			setDestroy(newSet);
			return NULL;
		}
		if (nodeToCopy->data == NULL) {
			currNode->data = NULL;
		} else {
			currNode->data = set->copyFunc(nodeToCopy->data);
			if (currNode->data == NULL) {
				free(currNode);
				setDestroy(newSet);
				return NULL;
			}
		}
		if (nodeToCopy == set->current) {
			newSet->current = currNode;
		}
		currNode->next = NULL;
		lastCopiedNode->next = currNode;
		lastCopiedNode = currNode;
		nodeToCopy = nodeToCopy->next;
	}
	newSet->size = setGetSize(set);
	return newSet;
}

int setGetSize(Set set) {
	if (set == NULL) {
		return -1;
	}
	assert(set->size >= 0);
	return set->size;
}

SetElement setGetFirst(Set set) {
	if (set == NULL || setGetSize(set) == 0) {
		return NULL;
	}
	set->current = set->dummy->next;
	return set->current->data;
}

SetElement setGetNext(Set set) {
	if (set == NULL || set->current == NULL) {
		return NULL;
	}
	set->current = set->current->next;
	return set->current != NULL ? set->current->data : NULL;
}

SetElement setContains(Set set, SetElement element) {
	IF_NULL_RETURN_NULL(set)
	IF_NULL_RETURN_NULL(element)
	IS_SET_VALID(set)
	Node iteratingNode = set->dummy->next;
	while (iteratingNode != NULL) {
		if (iteratingNode->data != NULL
				&& set->cmpFunc(iteratingNode->data, element) == 0) {
			set->current = iteratingNode;
			return iteratingNode->data;
		}
		iteratingNode = iteratingNode->next;
	}
	return NULL;
}

SetResult setAdd(Set set, SetElement element) {
	IF_NULL_RETURN_SET_NULL_ARGUMENT(set)
	IF_NULL_RETURN_SET_NULL_ARGUMENT(element)
	Node iteratingNode = set->dummy->next;
	Node beforeNode = set->dummy;
	while (iteratingNode != NULL) {
		assert(iteratingNode->data != NULL);
		int cmpResult = set->cmpFunc(iteratingNode->data, element);
		if (cmpResult > 0) {
			break; // Need to add element before iteratingNode
		}
		if (cmpResult < 0) { // add element after iterating Node
			iteratingNode = iteratingNode->next;
			beforeNode = beforeNode->next;
			continue;
		}
		return SET_ITEM_ALREADY_EXISTS; // cmpResult == 0
	}
	Node newNode = malloc(sizeof(*newNode));
	if (newNode == NULL) {
		return SET_OUT_OF_MEMORY;
	}
	newNode->data = set->copyFunc(element);
	if (newNode->data == NULL) {
		free(newNode);
		return SET_OUT_OF_MEMORY;
	}
	newNode->next = iteratingNode;
	beforeNode->next = newNode;
	set->size++;
	set->current = NULL;
	return SET_SUCCESS;
}

SetResult setRemove(Set set, SetElement element) {
	IF_NULL_RETURN_SET_NULL_ARGUMENT(set)
	IF_NULL_RETURN_SET_NULL_ARGUMENT(element)
	set->current = NULL; // iterator is undefined for every setRemove result
	Node iteratingNode = set->dummy->next;
	Node beforeNode = set->dummy;
	while (iteratingNode != NULL) {
		assert(iteratingNode->data != NULL);
		int cmpResult = set->cmpFunc(iteratingNode->data, element);
		if (cmpResult > 0) {
			return SET_ITEM_DOES_NOT_EXIST; // current element greater
		}
		if (cmpResult < 0) { // add element after iterating Node
			iteratingNode = iteratingNode->next;
			beforeNode = beforeNode->next;
			continue;
		}
		// cmpResult == 0 => delete the node
		beforeNode->next = iteratingNode->next;
		set->freeFunc(iteratingNode->data);
		free(iteratingNode);
		set->size--;
		return SET_SUCCESS;
	}
	return SET_ITEM_DOES_NOT_EXIST;
}

SetResult setClear(Set set) {
	IF_NULL_RETURN_SET_NULL_ARGUMENT(set)
	IS_SET_VALID(set)
	Node nodeToDelete = set->dummy->next;
	while (nodeToDelete != NULL) {
		if (nodeToDelete->data != NULL) {
			set->freeFunc(nodeToDelete->data);
		}
		Node nextNode = nodeToDelete->next;
		free(nodeToDelete);
		nodeToDelete = nextNode;
	}
	set->dummy->next = NULL;
	set->current = NULL;
	set->size = 0;
	return SET_SUCCESS;
}

void setDestroy(Set set) {
	if (set == NULL) {
		return; // mimic free() behavior
	}
	setClear(set);
	free(set->dummy);
	free(set);
}
