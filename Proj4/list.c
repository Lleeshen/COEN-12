/*
 *  Generic List
 *  Lyman Shen 5/28/17
 *  Description: This file provides an abstract data type for a dequeque.
 *  	It uses a circular, doubly-linked list as the implementation.
 *  	Most functions run in constant time, but finding an element to return and removing an arbitrary element can take linear time.
 */

#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct list {
    int count;
    int (*compare) ();
    struct node *head;
};

struct node {
    void *data;
    struct node *prev;
    struct node *next;
};

typedef struct node NODE;

/* Use an private auxillary function to make some functions cleaner */
static NODE *search (LIST *lp, void *item);

/*
 *  Function name: Create list
 *  Runtime: O(1)
 *  Description: Allocate memory for a list and the dummy node.
 *		The dummy node, currently without data in list, point to itself in next and prev
 *		The count is set to 0 and the compare function is inititializeed. 
 */

LIST *createList (int (*compare) ()) {

    //fprintf(stderr,"Create list called.\n");
    
 	LIST *lp;
 	lp = malloc(sizeof(LIST));
 	assert(lp != NULL);
 	NODE *head;
 	head = malloc(sizeof(NODE));
 	assert(head != NULL);
 	lp -> head = head;
 	lp -> head -> prev = lp -> head;
 	lp -> head -> next = lp -> head;
 	lp -> count = 0;
 	lp -> compare = compare;
 	return lp;
}

/*
 *  Function name: Destroy list
 *  Runtime: O(n)
 *  Description: Free all current data, free dummy node, free list
 */

void destroyList (LIST *lp) {

    //fprintf(stderr,"Destroy list called.\n");

 	assert(lp != NULL);
 	NODE *np = lp -> head;
 	while (np -> next != np) {
 		np -> next = np -> next -> next;
 		free(np -> next -> prev);
 		np -> next -> prev = np;
 		lp -> count --;
 	}
 	free(lp -> head);
 	free(lp);
}

/*
 *  Function name: Num items
 *  Runtime: O(1)
 *  Description: Return the count of elements in the list
 */

int numItems (LIST *lp) {

 	//fprintf(stderr,"Num items called.\n");
 	 
 	assert(lp != NULL);
 	return lp -> count;
 }

/*
 *  Function name: Add first
 *  Runtime: O(1)
 *  Description: Allocate memory for a new node. Set data to item.
 *		Place the node after the dummy node, and change affected pointers
 */

void addFirst (LIST *lp, void *item) {

	//fprintf(stderr,"Add first called.\n");
 	 
	assert(lp != NULL && item != NULL);
 	NODE *np;
 	np = malloc(sizeof(NODE));
 	assert(np != NULL);
 	np -> data = item;
 	np -> prev = lp -> head;
 	np -> next = lp -> head -> next;
 	lp -> head -> next -> prev = np;
 	lp -> head -> next = np;
 	lp -> count ++;
}

/*
 *  Function name: Add last
 *  Runtime: O(1)
 *  Description: Allocate memory for a new node. Set data to item.
 *		Place the node before the dummy node, and change affected pointers
 *		so that the list remains doubly linked. Increment count.
 */

void addLast (LIST *lp, void *item) {

 	//fprintf(stderr,"Add last called.\n"); 

	assert(lp != NULL && item != NULL);
	NODE *np;
 	np = malloc(sizeof(NODE));
 	assert(np != NULL);
 	np -> data = item;
 	np -> next = lp -> head;
 	np -> prev = lp -> head -> prev;
 	lp -> head -> prev -> next = np;
 	lp -> head -> prev = np;
 	lp -> count ++;
}

/*
 *  Function name: Remove first
 *  Runtime: O(1)
 *  Description: Save the data of the first item. Free the first item, or node after dummy node.
 *		Change affected pointers so the list remains doubly linked. Decrement count.
 *		Return the saved data
 */

void *removeFirst (LIST *lp) {

	//fprintf(stderr,"Remove first called.\n"); 

	assert(lp != NULL);
	NODE *np  = lp -> head;
    void *ret = np -> next -> data;
	assert (np -> next != np);
	np -> next = np -> next -> next;
	free(np -> next -> prev);
	np -> next -> prev = np;
	lp -> count --;
	return ret;
}


/*
 *  Function name: Remove first
 *  Runtime: O(1)
 *  Description: Save the data of the last item. Free the last item, or node before dummy node.
 * 		Change affected pointers so the list remains doubly linked. Decrement count.
 *		Return the saved data
 */

void *removeLast (LIST *lp) {

	//fprintf(stderr,"Remove last called.\n"); 

	assert(lp != NULL);
	NODE * np = lp -> head;
    void *ret = np -> prev -> data;
	assert (np -> prev != np);
	np -> prev = np -> prev -> prev;
	free(np -> prev -> next);
	np -> prev -> next = np;
	lp -> count --;
	return ret;
}

/*
 *  Function name: Get first
 *  Runtime: O(1)
 *  Description: Return the data of the first node
 */

void *getFirst (LIST *lp) {

	//fprintf(stderr,"Get first called.\n");

	assert(lp != NULL);
	return lp -> head -> next -> data;
}

/*
 *  Function name: Get last
 *  Runtime: O(1)
 *  Description: Return the data of the last node
 */

void *getLast(LIST *lp) {

	//fprintf(stderr,"Get last called.\n"); 

	assert(lp != NULL);
	return lp -> head -> prev -> data;
}

/*
 *	Function name: Remove item
 *  Runtime: O(n)
 *  Description: Remove the data in the list if found
 *		Use the search function to get the node with data.
 *		If node is found, free the node and change the pointers to preserve the list
 *		Decrement count.
 */

void removeItem (LIST *lp, void *item) {

	//fprintf(stderr,"Remove item called.\n"); 

	assert(lp != NULL && lp->compare != NULL);
	NODE *elt = search(lp, item);
	if(elt != NULL) {
		elt -> prev -> next = elt -> next;
		elt -> next -> prev = elt -> prev;
		free(elt);
		lp -> count--;
	}
}

/*
 *	Function name: Find item
 *  Runtime: O(n)
 *  Description: Return the data in the list if found
 *		Use the search function to get the node with data.
 *		If search returns NULL, return NULL, else return data of node returned
 */

void *findItem (LIST *lp, void *item) {

	//fprintf(stderr,"Find item called.\n");

	assert(lp != NULL && item != NULL && lp->compare != NULL);
	NODE *np = search(lp, item);
	if(np == NULL)
		return NULL;
	return np -> data;
}

/*
 *	Function name: Get items
 *  Runtime: O(n)
 *  Description: Return a copy of the data with array data type
 *		Allocate memory for an array of elements.
 *		Traverse the linked list from first to last, and set each node's data to the array index.
 */

void *getItems (LIST *lp) {

	//fprintf(stderr,"Get items called.\n"); 

	assert(lp != NULL);
	void **items;
	items = malloc(sizeof(void *) * lp -> count);
	assert(items != NULL);
	int i = 0;
	NODE *np = lp -> head -> next;
	while(np -> next != lp -> head) {
		items[i] = np -> data;
		i++;
	}
	return items;
}

/*
 *  Function name: Search (private)
 *  Runtime: O(n)
 *  Description: This is an auxillary function used in Remove item and Find item
 *     Find a node with the data item using sequential search
 *     Return the node with data if found, else return NULL.
 */

static NODE *search (LIST *lp, void *item) {
    //fprintf(stderr,"Search called.\n");
    
	NODE *np = lp -> head -> next;
	while (np != lp -> head) {
        if((*lp -> compare) (np -> data, item) == 0) {
            //fprintf(stderr,"Search found.\n");
			return np;
        }
         np = np -> next;
	}
    //fprintf(stderr,"Search not found.\n");
	return NULL;
}
