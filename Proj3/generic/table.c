/*
 *  File: table.c
 *  Name: Lyman Shen
 *  Date: May 7, 2017
 *  Description: This implementation of the set uses a hash table of generic pointers
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"

struct set{
	void **data; /* Data */
	char *flag; /* Flags for data */
	int count; /* Count of elements */
	int length; /* length */
	int (*compare)(); /* Use instead of string compare */
	unsigned (*hash)(); /* Use instead of string hash */
};

static int search (SET *sp, void *elt, bool *found);

/*
 * Function Name: createSet
 * Time Complexity: O(n)
 * Description: Create a hashtable with an appropiate identifier array
 */

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)() )
{
	SET *sp;
	sp = malloc(sizeof(SET)); /* Create Set */
	assert(sp != NULL);
	sp -> count = 0; /* Set count to 0 */
	sp -> length = maxElts; /* Set length to max elements */
	sp -> compare = compare;
	sp -> hash = hash;
	sp -> flag = malloc(sizeof(char) * maxElts); /* Allocate array of flags */
	assert(sp -> flag != NULL);
	sp -> data = malloc(sizeof(void *) * maxElts); /* Allocate array of data */
	assert(sp -> data != NULL);
	for(int i = 0; i<maxElts; i++)  /* Set every data to empty */
		sp->flag[i] = 'E';
	return sp;
}

/*
 * Function Name: destroySet
 * Time Complexity: O(1)
 * Description: Free data array, identifier array, and hashtable
 */

void destroySet(SET *sp)
{
	assert(sp != NULL);
	free(sp->data); /* Free data */
	free(sp->flag); /* Free flags */
	free(sp); /* Free set */
}

/*
 * Function Name: numElements
 * Time Complexity: O(1)
 * Description: Get the number of elements in the set
 */

int numElements(SET *sp)
{
	assert(sp != NULL);
	return sp-> count;
}

/*
 * Function Name: addElement
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Add an element to the set, change corresponding identifier, increment count
 */

void addElement(SET *sp, void *elt)
{
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int index = search(sp,elt,&found); /* Search for element */
	if(!found){ /* Add element if it is not there */
		sp->data[index] = elt;
		sp->flag[index] = 'F'; /* Set flag to filled */
		sp->count ++;
	}
}

/*
 * Function Name: removeElements
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Remove an element from the set, change corresponding identifier, decrement count
 */

void removeElement(SET *sp, void *elt)
{
	assert((sp!= NULL) && (elt != NULL));
	bool found;
	int index = search(sp, elt, &found); /* Search for element */
	if(found){ /* Remove elements if it is there */
		sp->flag[index] = 'D'; /* Set flag to deleted */
		sp->count--;
	}
}

/*
 * Function Name: findElement
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Find element in the set, return the pointer to the element if found, else return NULL
 */

void *findElement (SET *sp, void *elt)
{
	assert((sp != NULL) && (elt != NULL));
	bool found;
	int index = search(sp, elt, &found);
	if(!found) {
		return NULL;
	}
	return sp->data[index];
}

/*
 * Function Name: getElements
 * Time Complexity: O(n)
 * Description: Create a soft copy of the elements in the set.
 */

void *getElements(SET *sp)
{
	assert(sp != NULL);
	void **dataCopy;
	dataCopy = malloc(sizeof(void *)*sp->length);
	int i;
	for(i=0;i<sp->length;i++){
		if(sp->flag[i] == 'F') {
			dataCopy[i] = sp->data[i];
		}
	}
	return dataCopy;
}

/*
 * Function Name: search
 * Time Complexity: O(1) average, O(n) worst case
 * Descriptoin: Search for elemnt in set with the hash function, using linear probing.
 *          Return index if found, else return index where element would be inserted.
 *          Use a pass-by-reference variable to tell client if the element is found.
 */

static int search (SET *sp, void *elt, bool *found)
{
	assert((sp!= NULL) && (elt != NULL));
	int index = (*sp->hash)(elt)%sp->length;
	int deleted = -1;
	int locn;
	int i = 0;
	while(i<sp->length){
		locn = (index + i) % (sp->length);
		if(sp->flag[locn] == 'D') {
			if(deleted == -1) { /* Record first deleted location, otherwise keep searching */
				deleted = locn;
			}
		}
		else if(sp->flag[locn] == 'E') { /* Case to stop searching */
			*found = false;
			if(deleted == -1) /* Preference to give deleted index if there is one when not found */
			{
				return locn;
			}
			return deleted;
		}
		else if((*sp->compare)(sp->data[locn],elt) == 0){ /* Case found */
			*found = true;
			return locn;
		}
		i++;
	}
	*found = false;
	return deleted;
}
