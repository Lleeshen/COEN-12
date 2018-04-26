/*
 *	File: pqueue.c
 *	Name: Lyman Shen
 *	Date: June 1
 *	Description: This file provides an ADT implementation for the Priority Queue using array representation of binary heap.
 *		Operations add and remove has better time complexity compare to other implementations not using a heap.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pqueue.h"

#define START_LENGTH 10
#define p(x) (((x)-1)/2)
#define l(x) (((x)*2)+1)
#define r(x) (((x)*2)+1)

struct pqueue {
	void **data;
	int count;
	int length;
	int (*compare)();
};

static void swap(void **first, void **second);

/*
 * Function Name: createQueue
 * Time Complexity: O(1)
 * Description: Creates a priority queue of length 10.
 * 			The length can be increased dynamically with addEntry as needed.
 */

PQ *createQueue(int (*compare)()) {
	PQ *pq;
	pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	pq -> data = malloc(sizeof(void *) * START_LENGTH);
	pq -> count = 0;
	pq -> length = START_LENGTH;
	pq -> compare = compare;
	return pq;
}

/*
 * Function Name: destroyQueue
 * Time Complexity: O(1)
 * Description: Free data used by priority queue. The data and the queue itself are freed.
 */

void destroyQueue(PQ *pq) {
	free(pq -> data);
	free(pq);
}

/*
 * Function Name: numEntries
 * Time Complexity: O(1)
 * Description: Return number of entries in the priority queue.
 */

int numEntries(PQ *pq) {
	assert(pq != NULL);
	return pq -> count;
}

/*
 * Function Name: addEntry
 * Time Complexity: O(log n)
 * Description: Add an entry to the priority queue.
 *			The new entry is placed at the end, and is moved toward the
 *			front as needed to keep the heap's structure.
 */

void addEntry(PQ *pq, void *entry) {
	//fprintf(stderr,"Add entry called.\n");
	if(pq -> count == pq -> length) {
		pq -> data = realloc(pq -> data, sizeof(void *) * (pq -> length) * 2);
		assert(pq -> data != NULL);
		pq -> length *= 2;
	}
	pq -> data [pq -> count] = entry;
	int index = pq -> count;
	while(((*pq -> compare)(pq -> data[index], pq -> data[p(index)]) < 0)) {
			swap(&pq -> data[index], &pq -> data[p(index)]);
			index = p(index);
	}
	pq -> count ++;
	//fprintf(stderr,"Add entry finished.\n");
}			

/*
 * Function Name: removeEntry
 * Time Complexity: O(log n)
 * Description: Remove an entry to the priority queue.
 *			The entry to be removed is the root of the heap.
 *			It is swapped with an element that would remain in the heap but would otherwise be left out.
 *			The new root moves down the tree as needed to preserve the heap's structure.
 */

void *removeEntry(PQ *pq) {
	//fprintf(stderr,"Remove entry called.\n");
	assert(pq -> count != 0);
	void *entry = pq -> data[0];
	int index =0;
	int smaller;
	swap(&pq -> data[index], &pq -> data[--pq -> count]);
	while(l(index) < pq -> count) {
		if(r(index) < pq->count) {
			//fprintf(stderr,"Right child exists.\n");
			smaller = ((*pq -> compare)(pq -> data[r(index)], pq -> data[l(index)]) < 0) ? r(index) : l(index);
			//fprintf(stderr,"Know which child is smaller.\n");
			if((*pq -> compare)(pq -> data[index], pq -> data[smaller]) > 0) {
				//fprintf(stderr,"Smaller child prepare to swap with parent.\n");
				swap(&pq -> data[index], &pq -> data[smaller]);
				//fprintf(stderr,"Swap complete.\n");
				index = smaller;
				//fprintf(stderr,"Update current index.\n");
			} else
				break;
		} else {
			//fprintf(stderr,"Right child does not exist.\n");
			if((*pq -> compare)(pq -> data[index], pq -> data[l(index)]) > 0) {
				//fprintf(stderr,"Swap left child with parent.\n");
				swap(&pq -> data[index], &pq -> data [l(index)]);
				index = l(index);
				//fprintf(stderr,"Swap complete.\n");
			}
		}
	}
	//fprintf(stderr,"Remove entry finished.\n");
	return entry;
}

/*
 * Function Name: swap
 * Time Complexity: O(1)
 * Description: Auxillary function.
 *			Used in add and remove entry when placing entries in proper place to keep heap's sturcture.
 */

static void swap(void **first, void **second) {
	void *temp = *first;
	*first = *second;
	*second = temp;
}
