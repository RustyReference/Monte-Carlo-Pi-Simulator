#ifndef RES_FUNCS_
#define RES_FUNCS_
#include "functions.h"

/**
 * @return pointer to an empty RNode (for linked lists)
 */

RNode *create_RNode();

/**
 * @return the length of the list of RNodes
 */
uintmax_t length(RNode *list);

/** 
 * Prints the Result's attributes
 */
void print_res(Result *res);
#endif
