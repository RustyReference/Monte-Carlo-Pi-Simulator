#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "functions.h"
#include "result_functions.h"

void test() {
	uintmax_t tests, trials;
	RNode *list = create_RNode();
	RNode *iter = list;

	tests = get_num_tests();
	trials = get_num_trials();

	uintmax_t i;
	Result *res;
	for (i = 0; i < tests - 1; i++) {
		res = run_test(trials);
		iter->res = res;
		iter->next = create_RNode();
		iter = iter->next;
	}
	res = run_test(trials);
	iter->res = res;
	iter->next = NULL;

	iter = list;
	while (iter != NULL) {
		print_res(iter->res);
		iter = iter->next;
	}
}

int main() {
	/*
	test();
	*/
}
