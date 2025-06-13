#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "functions.h"
#include "result_functions.h"
#include "stats.h"
#define TIME 0
#define EST 1

/* Struct Definitions */
typedef struct {
    int flag;               // Time or Estimation?
    uintmax_t len;
    long double avg;
    RNode *list;
    Params *params_ret;
} ParamArgs;


/* Function Definitions */
/**
 * Finds the means of the times and PI-estimations of each 
 * test result stored in each RNode in the linked list. Then,
 * mutates the 'both' array with the means.
 * @param list the head to the linked list of RNodes
 * @param both 
 *      both[0]: time mean
 *      both[1]: estimation mean  
 * @param len the length of the linked list
 */
void mean(RNode *list, long double both[2], uintmax_t len) {
    long double time, est;
    time = est = 0;

    while (list != NULL) {
        time += list->res->time;
        est += list->res->estimate;
        list = list->next;
    }

    // Divide to get mean
    both[0] = time /= len;
    both[1] = est /= len;
}

/**
 * @return the square root of the number as a long double via Newton's mathod
 */
long double long_sqrt(long double num) {
    long double root = 1;
    int i;
    for (i = 0; i < 20; i++) {
        root -= (root * root - num) / (2 * root);
    }

    return root;
}

/**
 * Calculates the parameters of the times 
 * @param args a void pointer to an object containing all arguments to 
 *      this function
 * @returns NULL
 */
void *calc_params(void *args) {
    ParamArgs *full_args = (ParamArgs *)args;
    RNode *list = full_args->list;
    long double rand_var, variance, delta = 0;

    // Check if this calculation is for times or estimations.
    if (full_args->flag == TIME) {
        rand_var = list->res->time;
    } else if (full_args->flag == EST) {
        rand_var = list->res->estimate;
    } else {
        printf("Failed to run thread for parameter calculation: Invalid flag.\n");
        exit(1);
    }

    // variance = (sum(x_i - AVERAGE))^2 / N
    while (list != NULL) {
        long double diff = (rand_var - full_args->avg);
        delta += diff * diff;
        list = list->next;
    }
 
    full_args->params_ret->variance = variance = delta / full_args->len;
    full_args->params_ret->std_dev = long_sqrt(variance);
    full_args->params_ret->avg = full_args->avg;

    return NULL;
}

/**
 * Populates an array of objects containing all statistical parameters of the tests
 * @param list the head of a linked list of RNodes
 * @param both will be populated with results after this function calls
 *      both[0]: the Params of the time durations of the simulations
 *      both[1]: the Params of the estimations of the simulations
 */
void get_params(RNode *list, Params both[2]) {
    pthread_t time_thr, est_thr;
    long double time_then_est[2]; // [0]: average of times
                                  // [1]: average of estimations
    
    uintmax_t len = length(list);
    mean(list, time_then_est, len);
    ParamArgs time_params = { TIME, len, time_then_est[0], list, &both[0] };
    ParamArgs est_params = { EST, len, time_then_est[1], list, &both[1] };

    // Start one thread for the statistical parameters for time durations
    pthread_create(&time_thr, NULL, calc_params, &time_params);

    // Start one thread for the statistical parameters for the estimations
    pthread_create(&est_thr, NULL, calc_params, &est_params);
    
    // Wait for both threads to finish
    pthread_join(time_thr, NULL);
    pthread_join(est_thr, NULL);
}
