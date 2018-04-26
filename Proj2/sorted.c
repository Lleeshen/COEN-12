#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "set.h"

struct set 
{
	int count; /* number of elements */
	int length; /* length of array */
	char ** data; /* array strings */
};

typedef struct set SET;

static int search (SET *sp, char *elt, bool *found);

/*
 * Function Name: createSet
 * Time Complexity: O(n)
 * Deescription: Create a new Set
 */
SET *createSet (int maxElts)
{
	SET *sp;
	/* Allocate memory for set, assert set has memory */
	sp = malloc(sizeof(SET));
	assert(sp != NULL);
	/* Initialize counter and length */
	sp -> count = 0;
	sp -> length = maxElts;
	/* Allocate data, assert data has memory */
	sp ->data = malloc(sizeof(char *) * maxElts);
	assert(sp ->data != NULL);
	return sp;
}

/* 
 * Function Name: deestroySet
 * Time Complexity: O(n)
 * Description: Free a previously created set
 */
void destroySet (SET *sp)
{
	int i;
	/* Free data */
	for(i=0; i<sp->count; i++)
		free(sp->data[i]);
	/* Free array */
	free(sp->data);
	/* Free set */
	free(sp);
}

/* 
 * Function Name: numElements
 * Time Complexity: O(1)
 * Description: Return number of elements given a valid set
 */
int numElements (SET *sp)
{
	assert(sp != NULL);
	return sp->count;
}

/*
 * Function Name: addElement
 * Time Complexity: O(n)
 * Description: Add an element to the sorted location in the set
 */
void addElement (SET *sp, char *elt)
{
	/* Assert ability to add string to set */
	assert((elt != NULL) && (sp != NULL) && (sp->length > sp->count));
	char * NewElt;
	int posn;
	int i;
	bool found;
	/* Search for new element */
	posn = search(sp, elt,&found);
	/* Add if not found*/
	if(!found)
	{
		/* Make sure element has memory, add element, increment counter */
		NewElt = strdup(elt);
		assert(NewElt != NULL);
		for(i=sp->count;i>posn;i--)
			sp->data[i] = sp->data[i-1];
		sp->data[posn] = NewElt;
		sp->count++;
	}
}

/* 
 * Function Name: removeElement
 * Time Complexity: O(n)
 * Description: Remove an element and keep rest of elements sorted in the set
 */
void removeElement (SET *sp, char *elt)
{
	/* Assert ability to remove element from set */
	assert((sp != NULL) && (elt != NULL));
	int locn;
	int i;
	bool found;
	/* Search for element to delte */
	locn = search(sp,elt,&found);
	/* Remove if found */
	if(found) {
		/* Free element, move last element to empty space, decrement counter */
		free(sp->data[locn]);
		for(i=locn+1;i<sp->count;i++)
			sp->data[i-1] = sp->data[i];
		sp->count--;
	}
}

/* 
 * Function Name: findElement
 * Time Complexity: O(log(n))
 * Description: Find a element in a set and return the pointer to the one in the set
 */
char * findElement (SET *sp, char *elt)
{
	/* Assert ability to search for element */
	assert((sp != NULL) && (elt != NULL));
	int locn;
	bool found;
	/* Search for element */
	locn = search(sp, elt, &found);
	/* Return NULL if not found */
	if(!found){
		return NULL;
	}
	/* Return pointer to location if found */
	return sp->data[locn];
}

/* 
 * Function Name: getElements
 * Time Complexity: O(n)
 * Description: Create a soft copy of the set
 */
char ** getElements (SET *sp)
{
	/* Assert ability to get elements */
	assert(sp != NULL);
	/* Create memory for copy, assert copy has memory*/
	char ** DataCopy;
	DataCopy = malloc(sizeof(char *) * sp -> length);
	assert(DataCopy != NULL);
	/* Inititialize Pointers to elemens of original array */
	int i;
	for(i=0; i<sp->count; i++)
		DataCopy[i] = sp->data[i];
	return DataCopy;
}

/*
 * Function Name: search
 * Time Complexity: O(log(n))
 * Description: Utility function; Use binary search to return location of element or where it would be inserted and if the element is found
 */
static int search (SET *sp, char *elt, bool *found)
{
	/* Initialize variables */
	int lo,hi,mid,diff;
	lo = 0;
	hi = sp->count-1;
	/* Loop for binary search */
	while(lo <= hi){
		mid = (lo+hi)/2;
		/* Compare middle element with elt, then change bounds*/
		diff = strcmp(elt,sp->data[mid]);
		if(diff < 0)
			hi = mid - 1;
		else if (diff > 0)
			lo = mid + 1;
		else {
			/* Set found to true, return element index */
			*found = true;
			return mid;
		}
	}
	/* Set found to false, return index where insertion would start */
	*found = false;
	return lo;
}
