/*
   Lyman Shen
   April 6, 2017
   Purpose:To count number of words in a text file
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD_LENGTH 30

int main(int argc, char *argv[])
{
	//Open file
	FILE *fp = fopen(argv[1],"r");
	//Check if file is successfully inserted
	if(fp == NULL)
	{
		printf("Error reading file.\n");
		return 1;
	}
	int num = 0;
	char word[MAX_WORD_LENGTH+1];
	int read;
	//Read words while there are words to read
	while(fscanf(fp,"%30s%n",word,&read) == 1)
	{
		*(word+read) = '\0';
		//Update counter
		num++;
	}
	//Close file
	fclose(fp);
	//Show counter
	printf("%d total words\n",num);
	return 0;
}
