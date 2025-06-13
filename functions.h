
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include <inttypes.h>

/* All type/struct definitions */
/**
 * Contains information about each test
 */
typedef struct {
	long double time; 		// Elapsed time
	long double estimate;
	uintmax_t num_trials;
} Result;

/**
 * Linked list node for Results
 */
typedef struct Node {
	Result *res;
	struct Node *next;
} RNode;

/**
 * Contains statistical parameters for a group of Monte Carlo simulations
 */
typedef struct {
	long double avg;
	long double variance;
	long double std_dev;
} Params;


/* Function Definitions */
/**
 * @return random double [0, 1.0)
 */
double rand_double();

/**
 * Runs the Monte Carlo Simulation to estimate Pi
 */
long double mc_sim(uintmax_t num_trials); 

/**
 * Prompts for number of trials
 * @return the number of trials
 */
uintmax_t get_num_trials();

/**
 * Prompt for and
 * @return the number of trials 
 */
uintmax_t get_num_tests();

/**
 * Runs the Monte Carlo simulation with 'trials' number of trials
 * @return an object describing the results
 */
Result *run_test(uintmax_t trials);
#endif
