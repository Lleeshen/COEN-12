/*
 *	File: huffman.c
 *	Name: Lyman Shen
 *	Date: June 1
 *	Description:
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

static int compare(struct node *n1, struct node *n2);

int main (int argc, char *argv[]) {

	FILE *fp;
	char character;
    int i, j, counts[257], size[257];
    struct node *leaves[257], *n1, *n2, *new, *temp;
    PQ *priorityQueue;

    //fprintf(stderr,"Variables declared.\n");

	if(argc != 3) {
		fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
		return 1;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: cannot open %s\n", argv[0], argv[1]);
        return 1;
    }

    for (i=0; i<257; i++) {
    	counts[i] = 0;
    }

    //fprintf(stderr, "Count of charcters initialized to 0.\n");

    while(fscanf(fp, "%c", &character) == 1) {
    	counts[(int) character]++;
    }

    //fprintf(stderr, "Count of charcters initialized to actual count.\n");

    priorityQueue = createQueue(compare);

    //fprintf(stderr, "Priority queue initialized.\n");

    for (i=0;i<256;i++) {
        if(counts[i] > 0) {
            leaves[i] = malloc(sizeof(struct node));
            assert(leaves[i] != NULL);
            leaves[i] -> count = counts[i];
            leaves[i] -> parent = NULL;
    		addEntry(priorityQueue, leaves[i]);
        }
		else
			leaves[i] = NULL;
    }
	leaves[256] = malloc(sizeof(struct node));
	assert(leaves[256] != NULL);
	leaves[256] -> count = 0;
	leaves[256] -> parent = NULL;
	addEntry(priorityQueue, leaves[256]);

    //fprintf(stderr, "Array of leaves initialized, non 0 counts added to priority queue.\n");

    while (numEntries(priorityQueue) > 1) {
        new = malloc(sizeof(struct node));
        assert(new != NULL);
    	n1 = removeEntry(priorityQueue);
    	n2 = removeEntry(priorityQueue);
    	n1 -> parent = new;
    	n2 -> parent = new;
    	new -> count = (n1->count + n2->count);
    	new -> parent = NULL;
    	addEntry(priorityQueue, new);
    }

    //fprintf(stderr, "Huffman tree built with priority queue.\n");

    fclose(fp);

    if ((fp = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", argv[0], argv[1]);
        return 1;
    }

    fclose(fp);

    for(i=0;i<257;i++) {
    	if(leaves[i] != NULL) {
			j = 0;
    		temp = leaves[i];
    		while(temp -> parent != NULL) {
    			j++;
    			temp = temp -> parent;
    		}
    		size[i] = j;
    	}
    }

    //fprintf(stderr, "Size of non-0 chars found.\n");

    for(i=0;i<257;i++) {
    	if (leaves[i] != NULL)
    	{
    		if(isprint(i)) {
    			fprintf(stdout, "\'%c\'", i);
    		}
    		else {
    			fprintf(stdout, "%03o", i);
    		}
    		fprintf(stdout, ": %d x %d bits = %d bits\n", counts[i], size[i], counts[i] * size[i]);
    	}
    }

    //fprintf(stderr, "Printed count and bit sizes to stdout.\n");

    pack (argv[1], argv[2], leaves);
    //fprintf(stderr, "File compressed with pack function.\n");
	return 0;
}

static int compare(struct node *n1, struct node *n2) {
	return (n1->count < n2->count) ? -1 : (n1->count > n2->count);
}
