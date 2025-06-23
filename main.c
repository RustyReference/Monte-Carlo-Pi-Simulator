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

	iter = list;
	while (iter != NULL) {
		print_res(iter->res);
		iter = iter->next;
	}

	printf("\n\n");
	
	// Acquire statistical parameters for times and estimations.
	iter = list; // Reset iter to the beginning
	get_params(iter, time_then_est);
	time_params = time_then_est[0];
	est_params = time_then_est[1];

	printf("--- PARAMETERS FOR TIME ---\n");
	printf("%-20s %.18Lf\n", "Expected Value:", time_params.avg);
	printf("%-20s %.18Lf\n", "Variance:", time_params.variance);
	printf("%-20s %.18Lf\n\n", "Standard Deviation:", time_params.std_dev);

	printf("--- PARAMETERS FOR ESTIMATIONS ---\n");
	printf("%-20s %.18Lf\n", "Expected Value:", est_params.avg);
	printf("%-20s %.18Lf\n", "Variance:", est_params.variance);
	printf("%-20s %.18Lf\n\n", "Standard Deviation:", est_params.std_dev);
}

int main() {
	run();

}
