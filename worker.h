#ifndef WORKER_H
#define WORKER_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/poll.h>
#include "job.h"

typedef struct worker_s {
    int sv[2]; // sv[0] is for master, sv[1] is for worker
    pid_t pid;
    int id; //for offsets in shared memory
    job* current_job;
    job_status current_job_status;
} worker;


int worker_main(worker* self);
bool is_prime();

#endif /* WORKER_H */