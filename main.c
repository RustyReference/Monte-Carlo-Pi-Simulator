#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "functions.h"
#include "result_functions.h"
#include "stats.h"

void run() {
	uintmax_t tests, trials;
	Params time_then_est[2], time_params, est_params;
	RNode *list = create_RNode();
	RNode *iter = list;

	tests = get_num_sims();
	trials = get_num_trials();

	// Create linked list of RNodes (Result-Nodes)
	create_list(trials, tests, iter);

	// Print the list
	print_list(list);
	printf("\n\n");
	
	// Acquire statistical parameters for times and estimations.
	iter = list; 						// Reset iter to the beginning
	get_params(iter, time_then_est);
	time_params	= time_then_est[0];
	est_params	= time_then_est[1];

	// Print the parameters
	print_params(&time_params, &est_params);
}

int main() {
	run();

}
