#ifndef STATS_H
#define STATS_H
/**
 * Finds the means of the times and PI-estimations of each 
 * test result stored in each RNode in the linked list. Then,
 * mutates the 'both' array with the means.
 * @param list the head to the linked list of RNodes
 * @param both 
 *      both[0]: time mean
 *      both[1]: estimation mean  
 * @param len the length of the linked list
 */
void mean(RNode *list, long double both[2], uintmax_t len);

/**
 * @return the square root of the number as a long double via Newton's mathod
 */
long double long_sqrt(long double num);

/**
 * Calculates the parameters of the times 
 * @param args a void pointer to an object containing all arguments to 
 *      this function
 * @returns NULL
 */
void *calc_params(void *args);

/**
 * Populates an array of objects containing all statistical parameters of the tests
 * @param list the head of a linked list of RNodes
 * @param both will be populated with results after this function calls
 *      both[0]: the Params of the time durations of the simulations
 *      both[1]: the Params of the estimations of the simulations
 */
void get_params(RNode *list, Params both[2]);
#endif