#ifndef MASTER_H
#define MASTER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include "worker.h"
#include "queue.h"

typedef struct master_s {
    worker** workers;
    size_t number_of_workers;
    queue primes_queue;
    char* primes_file_path;
    size_t num_to_check; //Amount of numbers to check
    size_t num_results; //Amount of results recived
    size_t num_given_jobs; //Amount of jobs assigned
    job* shared_jobs_space;
} master;


int enqueue_primes_from_file(queue* q, char* path);
int master_main(master* master);
int spawn_workers(master* master, int n, worker* worker_self);
void init_master(master* master);
void free_master(master* master);

#endif /* MASTER_H */