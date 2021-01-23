#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include "job.h"

#include "master.h"
#include "worker.h"

void usage() {
    puts("usage: primes number_of_workers primes_file");
}

void print_err_msg() {
    perror("Error");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
        return EXIT_FAILURE;
    }
    int num_workers = atoi(argv[1]);
    if (num_workers <= 0) {
        puts("Invalid number of workers");
        return EXIT_FAILURE;
    }
    char* primes_file_path = argv[2];
    job* shared_jobs_space = mmap(NULL, sizeof(*shared_jobs_space) * num_workers,
                                  PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
    if (shared_jobs_space == MAP_FAILED) {
        print_err_msg();
        return EXIT_FAILURE;
    }
    

    return 0;
}
