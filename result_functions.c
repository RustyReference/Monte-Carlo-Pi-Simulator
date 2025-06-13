#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "result_functions.h"
#include <errno.h>

/* All functions for RNode and Result (RNode stands for Result-Node) */

/**
 * @return pointer to an empty RNode (for linked lists)
 */
RNode *create_RNode() {
	errno = 0;
	RNode *new_rn = (RNode *) malloc(sizeof(RNode));
	if (new_rn == NULL) {
		printf("Failed to allocate memory for RNode: %s\n", strerror(errno));
		exit(1);
	}

	return new_rn;
}

/**
 * @return the length of the list of RNodes
 */
uintmax_t length(RNode *list) {
	uintmax_t n = 0;
	while (list != NULL) {
		n++;
		list = list->next;
	}
	return n;
}

/** 
 * Prints the Result's attributes
 */
void print_res(Result *res) {
	printf("\nTime:             %Lf seconds\n",res->time);
	printf("Number of Trials: %ju\n", res->num_trials);
	printf("Estimation:       %Lf\n\n", res->estimate);
}
