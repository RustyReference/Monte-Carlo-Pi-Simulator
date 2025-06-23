#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "functions.h"
#define MAX_INPUT 11	// The max number of digits in uintmax_t
#define BASE 10			// The number base used

/**
 * Prompt for and
 * @return the number of trials 
 */
uintmax_t get_num_sims() {
	uintmax_t tests;
	char buff[MAX_INPUT], *endptr = NULL;
	printf("Enter the number of tests: ");

	// Get number of tests from user
	errno = 0;
	if (fgets(buff, MAX_INPUT, stdin) == NULL) {
		fprintf(stderr, "\nError reading number of tests: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Convert input to uintmax_t
	errno = 0;
	tests = (uintmax_t)strtoumax(buff, &endptr, BASE);
	if (errno) {
		fprintf(stderr, "Error converting number of tests to number: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return tests;
}

/**
 * @return random long double [0, 1.0)
 */
long double rand_double() {
	unsigned long long r = 0;
	#define RANDOM_BITS 15
	int i;
	for (i = 0; i < LDBL_MANT_DIG; i += RANDOM_BITS) {
		r <<= RANDOM_BITS;
		r ^= rand();
	}

	r %= 1uLL << LDBL_MANT_DIG; // Remove lower bits
	long double dr = r; // Convert to long double
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
				"mc_sim: Error creating directory 'plots': %s", 
				strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	// Construct file name w/ id to tell it apart from the others
	char filename[25];
	snprintf(filename, sizeof(filename), "plots/points%ju.csv", res_id);

	FILE *csv = fopen(filename, "w");
	if (!csv) {
		fprintf(stderr, "mc_sim: Error opening file: %s", strerror(errno));
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
 * @return the number of trials after prompting for it
 */
uintmax_t get_num_trials() {
	errno = 0;
	uintmax_t trials;
	char input[MAX_INPUT];

	// Prompt for number of trials
	printf("Enter the number of trials you want to use: ");
	fflush(stdout);

	// Input with error checking
	if (fgets(input, MAX_INPUT, stdin) == NULL) {
		fprintf(stderr, "\nError reading trials: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Conversion to uint w/ error checking
	char *endptr = NULL;
	trials = (uintmax_t)strtoumax(input, &endptr, BASE);
	if (errno) {
		fprintf(stderr, "\nError converting input: %s", strerror(errno));
		exit(EXIT_FAILURE);
	}

	return trials;
}

/**
 * Runs the Monte Carlo simulation with 'trials' number of trials
 * @param trials the number of trials (random variables) used
 * @param res_id the id of the simulation to tell it apart from the others
 * @return an object describing the results
 */
Result *run_test(uintmax_t trials, uintmax_t res_id) {
	clock_t start;
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