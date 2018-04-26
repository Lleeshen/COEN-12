/*
 *  File: set.c
 *  Name: Lyman Shen
 *  Date: May 18, 2017
 *  Description: This implementation of the set uses a hash table of generic data, using hashing with chaining
 *      The list ADT operations are used to deal the list in each possible hashing location.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "set.h"

#define EXP 20 /* Expected operations to find something */

struct set{
	LIST ** list; /* Data */
	int count; /* Count of elements */
	int length; /* length of table */
	int (*compare)(); /* Use instead of string compare */
	unsigned (*hash)(); /* Use instead of string hash */
};

/*
 * Function Name: createSet
 * Time Complexity: O(n)
 * Description: Create a hashtable with a list at hashed location
 * 			Uses the List ADT for each list
 */

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)() )
{
    //fprintf(stderr,"Create set called.\n");
    
	SET *sp;
	sp = malloc(sizeof(SET)); /* Create Set */
	assert(sp != NULL);
	sp -> count = 0; /* Set count to 0 */
	sp -> length = (maxElts / EXP); /* Set length to max elements */
	sp -> compare = compare;
	sp -> hash = hash;
	sp -> list = malloc(sizeof(LIST*) * sp->length); /* Allocate array of data */
	assert(sp -> list != NULL);
    int i;
    //fprintf(stderr, "Length: %d\n", sp->length);
    for(i= 0; i < sp->length ;i++) {
        //fprintf(stderr,"Create list %d.\n", i);
        sp -> list[i] = createList(compare); /* Create a list at each hashed location */
	}
    
    //fprintf(stderr,"Create set finished.\n");
    
	return sp;
}

/*
 * Function Name: destroySet
 * Time Complexity: O(n)
 * Description: Free data in lists, hash table of lists, and hashtable
 */

void destroySet(SET *sp)
{
    //fprintf(stderr,"Destroy set called.\n");
    
	assert(sp != NULL);
	int i;
	for(i=0;i<sp->length;i++) {
        destroyList(sp->list[i]);
	}
    free(sp->list);
	free(sp); /* Free set */
    
    //fprintf(stderr,"Destroy set finished.\n");
}

/*
 * Function Name: numElements
 * Time Complexity: O(1)
 * Description: Get the number of elements in the set
 */

int numElements(SET *sp)
{
    //fprintf(stderr,"Num elements called.\n"); 
    
	assert(sp != NULL);
	return sp-> count;
}

/*
 * Function Name: addElement
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Add an element to the correct list if not found
 *      use index through hashing, increment count
 */

void addElement(SET *sp, void *elt)
{
    //fprintf(stderr,"Add element called.\n"); 
    
	assert((sp != NULL) && (elt != NULL));
    int index = (*sp -> hash)(elt) % sp->length;
    void *data = findItem(sp->list[index], elt);
    if(data == NULL) {
        addFirst(sp -> list[index] ,elt);
        sp -> count++;
    }
    //fprintf(stderr,"Add element finished.\n");
    
}

/*
 * Function Name: removeElements
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Remove an element from the set if found
 *      Use index through hashing, decrement count
 */

void removeElement(SET *sp, void *elt)
{
    //fprintf(stderr,"Remove element called.\n");    
    
	assert((sp!= NULL) && (elt != NULL));
    int index = (*sp -> hash)(elt) % sp->length;
    void *data = findItem(sp->list[index], elt);
    if(data != NULL) {
        removeItem(sp -> list[index] ,elt);
        sp -> count--;
    }
    
    //fprintf(stderr,"Remove element finished.\n");  
}

/*
 * Function Name: findElement
 * Time Complexity: O(1) average, O(n) worst case
 * Description: Find element in the set, return the pointer to the element if found, else return NULL
 */

void *findElement (SET *sp, void *elt)
{
    //fprintf(stderr,"Find element called.\n"); 
    
	assert((sp != NULL) && (elt != NULL));
    int index = (*sp -> hash)(elt) % sp->length;
    return findItem(sp->list[index],elt);
}

/*
 * Function Name: getElements
 * Time Complexity: O(n)
 * Description: Create a soft copy of the elements in the set.
 *      Get soft copies of each list, and append them in the list to return
 */

void *getElements(SET *sp)
{
    //fprintf(stderr,"Get elements called.\n"); 
    
	assert(sp != NULL);
	void **dataCopy;
	dataCopy = malloc(sizeof(void *)*sp->length*EXP);
    void **dataInsert;
    int numInsert;
    int size;
	int i;
    int index = 0;
	for(i=0;i<sp->length;i++){
        dataInsert = getItems(sp->list[i]);
        numInsert = numItems(sp->list[i]);
        size = sizeof(getFirst(sp->list[i]));
        memcpy(dataCopy+index,dataInsert,numInsert*size);
        index += numInsert;
	}
    
    //fprintf(stderr,"Get elements finished.\n");
    
	return dataCopy;
}
