#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include "functions.h"
#define RAND_TESTS 5000
#define SIZE 11
int num_passed = 0;

/**
 * Generates a large number of random doubles in [0, 1.0)
 * and tests if they all are in that interval
 */
void test_rand_double() {
	int i;
	for (i = 0; i < RAND_TESTS; i++) {
		double rand = rand_double();
		assert(rand < 1.0 && rand >= 0);
	}

	printf("rand_double() has passed!\n");
	num_passed++;
}

/**
 * Tests if the program can accept user input correctly,
 * and most importantly, handle invalid input.
 */
void test_num_trials() {
	FILE *temp = tmpfile();
	int tmpfd = fileno(temp);	// File descriptor
	unsigned long num_trials;

	// Sample "user input"
	fputs("1234567890", temp);
	rewind(temp);

	// Allow stdin to read from temporary file instead
	int save_stdin = dup(STDIN_FILENO);
	FILE *save_stdout = fdopen(dup(STDOUT_FILENO), "w");
	dup2(tmpfd, STDIN_FILENO);

	FILE *null_out = freopen("/dev/null", "w", stdout);	// Close stdout for now
	if (!null_out) {
		perror("Failed to redirect stdout\n");
		exit(1);
	}

	num_trials = get_num_trials();

	dup2(save_stdin, STDIN_FILENO);		// Restore the real stdin
	close(save_stdin); 					// Close temp storage fd for actual stdin
										
	dup2(fileno(save_stdout), STDOUT_FILENO);	// Restore the real stdout
	fclose(save_stdout);					

	// Test if the result was expected
	assert(num_trials == 1234567890);
	printf("get_num_trials() has passed!\n");
	num_passed++;
}

int main() {
	printf("Running tests...\n");
	test_rand_double();
	test_num_trials();
	printf("%d tests have passed.\n", num_passed);

	return 0;
}
