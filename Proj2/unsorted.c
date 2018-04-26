#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"

struct set 
{
	int count; /* number of elements */
	int length; /* length of array */
	char ** data; /* array strings */
};

typedef struct set SET;

static int search (SET *sp, char *elt);

/* 
 * Function Name: createSet
 * Time Complexity: O(n)
 * Description: Create a new set
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
 * Function Name: destroySet
 * Time Complexity: O(n)
 * Description: Free a previously created set if possible
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
 * Description: Add an element to the set
 */
void addElement (SET *sp, char *elt)
{
	/* Assert ability to add string to set */
	assert((elt != NULL) && (sp != NULL) && (sp->length > sp->count));
	char * NewElt;
	int posn;
	/* Search for new element */
	posn = search(sp, elt);
	/* Add if not found*/
	if(posn == -1)
	{
		/* Make sure element has memory, add element, increment counter */
		NewElt = strdup(elt);
		assert(NewElt != NULL);
		sp->data[sp->count++] = NewElt;
	}
}

/*
 * Function Name: removeElement
 * Time Complexity: O(n)
 * Description: Remove an Element in the set
 */
void removeElement (SET *sp, char *elt)
{
	/* Assert ability to remove element from set */
	assert((sp != NULL) && (elt != NULL));
	int locn;
	/* Search for element to delte */
	locn = search(sp,elt);
	/* Remove if found */
	if(locn != -1) {
		/* Free element, move last element to empty space, decrement counter */
		free(sp->data[locn]);
		sp -> data [locn] = sp->data[--sp->count];
	}
}

/*
 * Function Name: findElement
 * Time Complexity: O(n)
 * Description: Find a element in a set and return the pointer to the one in the set
 */
char * findElement (SET *sp, char *elt)
{
	/* Assert ability to search for element */
	assert((sp != NULL) && (elt != NULL));
	int locn;
	/* Search for element */
	locn = search(sp, elt);
	/* Return NULL if not found */
	if(locn == -1)
		return NULL;
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
 * Time Complexity: O(n)
 * Description: Utility function; Use sequential search to return location of element, return -1 if element is not there
 */
static int search (SET *sp, char *elt)
{
	int i;
	/* Traverse data to look for elt */
	for(i=0;i<sp->count;i++)
	{
		/* Return location if found */
		if(strcmp(sp->data[i], elt) == 0)
			return i;
	}
	/* Return -1 if not found */
	return -1;
}
