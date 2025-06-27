#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "functions.h"
#include "result_functions.h"
#include "stats.h"

int main() {
	uint8_t 	list_created = 0; 	// Has the list has been created?
	uintmax_t 	sims, trials;
	Params 		time_then_est[2], time_params, est_params;
	RNode 		*list = create_RNode();
	RNode 		*iter = list;

	while (1) {
		uint8_t choice = get_choice();	// Prompt user for action
		switch (choice) {
			case 1: 
				// Prompt for number of simulations, then number of trials
				sims	= get_num_sims();
				trials	= get_num_trials();
				create_list(trials, sims, iter); // Run tests
				list_created = 1;
				break;
			case 2: 
				// If tests have been run, then print list of results
				if (list_created) {
					print_list(list);
				}
				break;
			case 3:
				// Check if list has been created
				if (!list_created) {
					break;
				}
				// Acquire statistical parameters for times and estimations.
				get_params(list, time_then_est);
				time_params	= time_then_est[0];
				est_params	= time_then_est[1];
				print_params(&time_params, &est_params); // Then print them
				break;
			case 4:
				printf("Goodbye!\n");
		}
	}

	return 0;
}