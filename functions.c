#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>
#include "functions.h"
#define MAX_INPUT 11	// The max number of digits in uintmax_t
#define BASE 10			// The number base used

/**
 * Prompt for and
 * @return the number of trials 
 */
uintmax_t get_num_tests() {
	uintmax_t tests;
	char buff[MAX_INPUT], *endptr = NULL;
	printf("Enter the number of tests: ");

	// Get number of tests from user
	errno = 0;
	if (fgets(buff, MAX_INPUT, stdin) == NULL) {
		printf("\nError reading number of tests: %s\n", strerror(errno));
		exit(1);
	}

	// Convert input to uintmax_t
	errno = 0;
	tests = (uintmax_t)strtoumax(buff, &endptr, BASE);
	if (errno) {
		printf("Error converting number of tests to number: %s\n", strerror(errno));
		exit(1);
	}

	return tests;
}

/**
 * @return random double [0, 1.0)
 */
double rand_double() {
	unsigned long long r = 0;
	#define RANDOM_BITS 15
	int i;
	for (i = 0; i < DBL_MANT_DIG; i += RANDOM_BITS) {
		r <<= RANDOM_BITS;
		r ^= rand();
	}

	r %= 1uLL << DBL_MANT_DIG; // Remove lower bits
	double dr = r; // Convert to double
	dr /= 1uLL << DBL_MANT_DIG;
	return dr;
}

/**
 * Runs a Monte Carlo simulation of random points that lay in a
 * square with dimensions 1x1, counting the number of points that
 * landed inside a quarter circle whose radius is 1 and sweeps from 
 * one side to another side adjacent.
 *
 * @param num_trials the number of points that will be randomly 
 * 		placed in the square
 * @return an estimation of pi (the percentage of points that landed
 * 		inside the quarter circle, multiplied by 4)
 */
long double mc_sim(uintmax_t num_trials) {
	srand(clock());
	
	// Number of points that landed inside the quarter circle
	uintmax_t num_in = 0;
	uintmax_t i;
	for (i = 0; i < num_trials; i++) {
		long double x = rand_double();
		long double y = rand_double();
		if (x * x + y * y <= 1.0) {
			num_in++;
		}
	}
	
	return 4.0 * (long double) num_in / num_trials;
}

/**
 * @return the number of trials after prompting for it
 */
uintmax_t get_num_trials() {
	uintmax_t trials;
	char input[MAX_INPUT];

	// Prompt for number of trials
	printf("Enter the number of trials you want to use: ");
	fflush(stdout);

	// Input with error checking
	errno = 0;
	if (fgets(input, MAX_INPUT, stdin) == NULL) {
		printf("\nError reading trials: %s\n", strerror(errno));
		exit(1);
	}

	// Conversion to uint w/ error checking
	errno = 0;
	char *endptr = NULL;
	trials = (uintmax_t)strtoumax(input, &endptr, BASE);
	if (errno) {
		printf("\nError converting input: %s", strerror(errno));
		exit(1);
	}

	return trials;
}

/**
 * Runs the Monte Carlo simulation with 'trials' number of trials
 * @return an object describing the results
 */
Result *run_test(uintmax_t trials) {
	clock_t start;
	long double estimation, elapsed;

	// Run simulation and record it's duration 
	start = clock();
	estimation = mc_sim(trials);
	elapsed = ((long double)(clock() - start)) / CLOCKS_PER_SEC;

	// Return object with Results
	Result *res = (Result *) malloc(sizeof(Result));
	res->time = elapsed;
	res->estimate = estimation;
	res->num_trials = trials;
	return res;
}