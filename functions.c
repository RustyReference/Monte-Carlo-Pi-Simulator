#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "functions.h"
#include "result_functions.h"
#define MAX_INPUT 11	// The max number of digits in uintmax_t
#define BASE 10			// The number base used

typedef struct {
	uintmax_t 	val;	// Stores the returned uintmax_t if function succeeded
	uint8_t		err_flag;
} StouResult;

/**
 * Takes user input, with error checking
 * 
 * @param input the input buffer to store input in
 */
void get_input(char input[MAX_INPUT]) {
	errno = 0;
	if (fgets(input, MAX_INPUT, stdin) == NULL) {
		fprintf(stderr, "\nget_input(): Error reading input: %s\n", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/**
 * Convert a string into a uintmax_t with error checking.
 * (should be casted by caller if a smaller int type is desired.)
 * 
 * Will error if... 
 * 		- the resulting number is out of bounds for uintmax_t
 *		- the string is not fully a number
 * 
 * @param input the string to convert to an unsigned int
 * @return the string as an int, or 
 */
uintmax_t str_to_uint(char input[MAX_INPUT]) {
	uintmax_t val;
	char *endptr;
	
	// Check if the number is negative
	if (*input == '-') {
		fprintf(stderr, "\nstr_to_uint(): Error converting input: %s\n",
			"Cannot convert negative numbers.");
		exit(EXIT_FAILURE);
	}

	// Conversion to uint w/ error checking
	endptr = NULL;
	val = strtoumax(input, &endptr, BASE);
	if (errno) {
		fprintf(stderr, "\nstr_to_uint(): Error converting input: %s\n", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Check if the string has any letters in it 
	if (*endptr != '\0' && *endptr != '\n') {
		fprintf(stderr, "\nstr_to_uint(): Error converting input: %s\n",
			"Input parameter 'char *input' is not fully a number.");
		exit(EXIT_FAILURE);
	}
	
	return val;
}

/**
 * Convert a string into a uintmax_t with error checking, but will
 * return an error flag if a recoverable error is met.
 * (Output should be casted by caller if a smaller int type is desired.)
 * 
 * Will fatally error if... 
 * 		- the resulting number is out of bounds for uintmax_t
 * Will return an error flag if...
 *		- the string is not fully a number
 * 		- the string begins with a negative number
 *
 * @param input the string to convert to an unsigned int
 * @return a struct containing the value and an error flag, where
 * 		the error flag will be equal to 1 if the function failed
 * 		in a recoverable way.
 */
StouResult stou_checked(char input[MAX_INPUT]) {
	errno = 0;
	StouResult res = {0};
	char *endptr;
	
	// Conversion to uint w/ error checking
	endptr 	= NULL;
	res.val = strtoumax(input, &endptr, BASE);
	if (errno && (errno != EINVAL)) {
		fprintf(stderr, "\nstou_checked(): Error converting input: %s\n", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Check if the number is negative or if the string has non-numbers
	if (*input == '-' || (*endptr != '\0' && *endptr != '\n')) {
		res.err_flag = 1;
	}
	
	return res;
}

/**
 * @return random long double [0, 1.0)
 */
long double rand_double() {
	#define RANDOM_BITS 15
	unsigned long long r = 0;
	int i;
	for (i = 0; i < LDBL_MANT_DIG; i += RANDOM_BITS) {
		r <<= RANDOM_BITS;
		r ^= rand();
	}

	r %= 1uLL << LDBL_MANT_DIG; // Remove lower bits
	long double dr = r; 		// Convert to long double
	dr /= 1uLL << LDBL_MANT_DIG;
	return dr;
}

/**
 * Runs a Monte Carlo simulation of random points that lay in a
 * square with dimensions 1x1, counting the number of points that
 * landed inside a quarter circle whose radius is 1 and sweeps from 
 * one side to another side adjacent.
 * 
 * Records each point's coordinates in points.csv
 *
 * @param num_trials the number of points that will be randomly 
 * 		placed in the square
 * @param res_id the id of the Result
 * @return an estimation of pi (the percentage of points that landed
 * 		inside the quarter circle, multiplied by 4)
 * 		*** OR -1 ON ERROR ***
 */
long double mc_sim(uintmax_t num_trials, uintmax_t res_id) {
	errno = 0;
	
	// Create the directory to hold the .csv files
	struct stat st;
	if (stat("plots", &st) == -1) {
		if (mkdir("plots", 0700) == -1) {
			fprintf(
				stderr, 
				"mc_sim(): Error creating directory 'plots': %s\n", 
				strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	// Construct file name w/ id to tell it apart from the others
	char filename[25];
	snprintf(filename, sizeof(filename), "plots/points%ju.csv", res_id);

	FILE *csv = fopen(filename, "w");
	if (!csv) {
		fprintf(stderr, "mc_sim(): Error opening file: %s\n", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}
	fprintf(csv, "x,y,inside\n");

	srand(clock());
	
	// Number of points that landed inside the quarter circle
	int is_inside = 0;						// Point is in quarter circle?
	uintmax_t i, num_in = 0;
	for (i = 0; i < num_trials; i++) {
		long double x = rand_double();
		long double y = rand_double();
		if ((is_inside = (x * x + y * y <= 1.0))) {
			// Set is_inside to 1 if the point is inside; 0 otherwise
			num_in++;
		}
		fprintf(csv, "%Lf,%Lf,%d\n", x, y, is_inside);
	}
	
	fclose(csv);
	return 4.0 * (long double) num_in / num_trials;
}

/**
 * Runs a Monte Carlo simulation of random points that lay in a
 * square with dimensions 1x1, counting the number of points that
 * landed inside a quarter circle whose radius is 1 and sweeps from 
 * one side to another side adjacent.
 * 
 * Records each point's coordinates in points.csv
 *
 * @param num_trials the number of points that will be randomly 
 * 		placed in the square
 * @param res_id the id of the Result
 * @return an estimation of pi (the percentage of points that landed
 * 		inside the quarter circle, multiplied by 4)
 * 		*** OR -1 ON ERROR ***
 */
uintmax_t get_num_trials() {
	errno = 0;
	uintmax_t 	trials;
	char 		input[MAX_INPUT];

	printf("Enter the number of trials you want to use: ");
	fflush(stdout);				// Flush for purposes of testing functions

	get_input(input);			// Get number of trials
	trials = str_to_uint(input);// Conversion to uint w/ error checking

	return trials;
}

/**
 * @return the number of simulations after prompting for it
 */
uintmax_t get_num_sims() {
	uintmax_t	tests;
	char 		buff[MAX_INPUT];

	printf("Enter the number of tests: ");
	get_input(buff); 				// Get number of tests from user
	tests = str_to_uint(buff); 		// Convert input to uintmax_t

	return tests;
}

/**
 * Runs the Monte Carlo simulation with 'trials' number of trials
 * @param trials the number of trials (random variables) used
 * @param res_id the id of the simulation to tell it apart from the others
 * @return an object describing the results
 */
Result *run_test(uintmax_t trials, uintmax_t res_id) {
	clock_t 	start;
	long double estimation, elapsed;

	// Run simulation and record it's duration 
	start = clock();
	estimation = mc_sim(trials, res_id);
	elapsed = ((long double)(clock() - start)) / CLOCKS_PER_SEC;

	// Return object with Results
	Result *res = (Result *) malloc(sizeof(Result));
	res->time = elapsed;
	res->estimate = estimation;
	res->num_trials = trials;
	res->id = res_id;
	return res;
}

/**
 * Creates a linked list of RNodes that iter points to by running 
 * 'trials' tests
 * 
 * @param trials the number of points randomly generated in each test
 * @param tests the number of Monte Carlo simulations run 
 * @param iter the pointer used to initialize each node, initially
 * 		set equal to the address of head of the list.
 */
void create_list(uintmax_t trials, uintmax_t tests, RNode *iter) {
	uintmax_t i;
	Result *res;
	for (i = 0; i < tests - 1; i++) {
		res = run_test(trials, i);
		iter->res = res;
		iter->next = create_RNode();
		iter = iter->next;
	}
	res = run_test(trials, i);
	iter->res = res;
	iter->next = NULL;
}

/**
 * Prints the parameters of the durations and pi-estimations of 
 * the simulations
 * @param time_params the address to parameters of the durations
 * @param est_params the address to parameters of the estimations
 */
void print_params(Params *time_params, Params *est_params) {
	printf("--- PARAMETERS FOR TIME ---\n");
	printf("%-20s %.18Lf\n", "Expected Value:", time_params->avg);
	printf("%-20s %.18Lf\n", "Variance:", time_params->variance);
	printf("%-20s %.18Lf\n\n", "Standard Deviation:", time_params->std_dev);

	printf("--- PARAMETERS FOR ESTIMATIONS ---\n");
	printf("%-20s %.18Lf\n", "Expected Value:", est_params->avg);
	printf("%-20s %.18Lf\n", "Variance:", est_params->variance);
	printf("%-20s %.18Lf\n\n", "Standard Deviation:", est_params->std_dev);
}

/**
 * Prints all Results in the RNode linked list
 * @param iter the pointer to the head of the linked list
 */
void print_list(RNode *list) {
	RNode *iter = list;
	while (iter != NULL) {
		print_res(iter->res);
		iter = iter->next;
	}
}

/**
 * Prompts the user for a number to choose what to do
 * 
 * @return the number representing the user's desired course of action.
 */
uint8_t get_choice() {
	errno = 0;
	StouResult	choice;
	char 		input[MAX_INPUT];

	while (1) {
		printf("\nWhat do you want to do?\n");
		printf("%-10s %40s\n", "1:", "Run tests");
		printf("%-10s %40s\n", "2:", "Print test results");
		printf("%-10s %40s\n", "3:", "Get and Print Population Parameters");
		printf("%-10s %40s\n", "4:", "Visualize a simulation");
		printf("%-10s %40s\n", "5:", "Exit");

		get_input(input);
		choice = stou_checked(input);

		if (choice.err_flag == 0) {
			break;
		}

		if (choice.val > 4) {
			printf("\nNumber choice must be between 1 and 4, inclusive: ");
		} else {
			printf("\nInvalid choice: ");
		}
	}

	return (uint8_t) choice.val;
}

/** 
 * Prompt-loop for the Simulation ID of the simulation the user wants
 * to visualize.
 * 
 * @param num_sims the number of simulations the user wanted to create
 */
uint8_t visualize_num(uintmax_t num_sims) {
	char input[MAX_INPUT];
	StouResult choice;
	
	while (1) {
		printf("\n%s%s",
			"Which simulation do you want to visualize? ",
			"(Enter the Simulation ID)\n");

		get_input(input);
		choice = stou_checked(input);

		if (choice.err_flag == 0) {
			break;
		}

		if (choice.val >= num_sims) {
			printf("\nNumber choice must be between 1 and 4, inclusive: ");
		} else {
			printf("\nInvalid choice\n");
		}
	}

	return (uint8_t) choice.val;
}

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
void visualize(uintmax_t num_sims) {
	errno = 0;
	#define COMM_LEN 25		// assert: strlen(command) <= 24
	
	uint8_t num = visualize_num(num_sims);
	char command[COMM_LEN];
	int n = snprintf(command, COMM_LEN, "python3 plotter.py %hhu", num);
	if (n < 0) {
		fprintf(
			stderr,
			"visualize(): Error generating visualization command: %s", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("Loading...\n");
	system(command);		// Run the Python program
	if (errno != 0) {
		fprintf(
			stderr,
			"visualize(): Error visualizing: %s", 
			strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Rendering succeeded!\n");
}