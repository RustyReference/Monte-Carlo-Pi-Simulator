
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
	uintmax_t id;
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
long double rand_double();

/**
 * Runs the Monte Carlo Simulation to estimate Pi
 */
long double mc_sim(uintmax_t num_trials, uintmax_t res_id); 

/**
 * Prompts for number of trials
 * @return the number of trials
 */
uintmax_t get_num_trials();

/**
 * Prompt for and
 * @return the number of simulations 
 */
uintmax_t get_num_sims();

/**
 * Runs the Monte Carlo simulation with 'trials' number of trials
 * @return an object describing the results
 */
Result *run_test(uintmax_t trials, uintmax_t res_id);

/**
 * Creates a  linked list of RNodes that iter points to by running 
 * 'trials' tests
 * 
 * @param trials the number of points randomly generated in each test
 * @param tests the number of Monte Carlo simulations run 
 * @param iter the pointer used to initialize each node, initially
 * 		set equal to the address of head of the list.
 */
void create_list(uintmax_t trials, uintmax_t tests, RNode *iter);

/**
 * Prints the parameters of the durations and pi-estimations of 
 * the simulations
 * @param time_params the address to parameters of the durations
 * @param est_params the address to parameters of the estimations
 */
void print_params(Params *time_params, Params *est_params);

/**
 * Prints all Results in the RNode linked list
 * @param iter the pointer to the head of the linked list
 */
void print_list(RNode *list);

/**
 * Prompts the user for a number to choose what to do
 * 
 * @return the number representing the user's desired course of action.
 */
uint8_t get_choice();

/** 
 * Prompt-loop for the Simulation ID of the simulation the user wants
 * to visualize.
 * 
 * @param num_sims the number of simulations the user wanted to create
 */
uint8_t visualize_num(uintmax_t num_sims);

/**
 * Runs a Python program to visualize a simulation. 
 * 
 * The simulation consists of showing the entire 1x1 square in which
 * every point is randomly generated, the quarter circle that sweeps from 
 * the left side to the bottom side, and every single point that was 
 * randomly generated, so the user can see every point plotted.
 * 
 * @param num_sims the number of simulations the user wanted to create
 */
void visualize(uintmax_t num_sims);
#endif
