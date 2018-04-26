all: unsortedP unsortedU sortedP unsortedP

U: unsortedP unsortedU

S: sortedP sortedU

unsortedP: parity.c unsorted.c set.h
	gcc -o unsortedParity parity.c unsorted.c

unsortedU: unique.c unsorted.c set.h
	gcc -o unsortedUnique unique.c unsorted.c

sortedP: parity.c sorted.c set.h
	gcc -o sortedParity parity.c sorted.c

sortedU: unique.c sorted.c set.h
	gcc -o sortedUnique unique.c sorted.c
