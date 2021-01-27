#define _GNU_SOURCE
#include <sys/mman.h>
#include <time.h>
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
    if (argc != 3) {
        printf("%d", argc);
        usage();
        return EXIT_FAILURE;
    }
    int num_workers = atoi(argv[1]);
    if (num_workers <= 0) {
        puts("Invalid number of workers");
        return EXIT_FAILURE;
    }
    master master;
    init_master(&master);
    worker worker_self;
    master.primes_file_path = argv[2];
    job* shared_jobs_space = mmap(NULL, sizeof(*shared_jobs_space) * num_workers,
                                  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if (shared_jobs_space == MAP_FAILED) {
        print_err_msg();
        return EXIT_FAILURE;
    }
    master.shared_jobs_space = shared_jobs_space;

    switch (spawn_workers(&master, num_workers, &worker_self)) {
        case 1:
            free_master(&master);
            if (worker_main(&worker_self) < 0) {
                print_err_msg();
                return EXIT_FAILURE;
            }
            break;
        case 0:
            if (master_main(&master) < 0) {
                print_err_msg();
                return EXIT_FAILURE;
            }
            free_master(&master);
            break;
        default:
            print_err_msg();
            return EXIT_FAILURE;
    }

    if (munmap(shared_jobs_space, sizeof(*shared_jobs_space) * num_workers) < 0) {
        print_err_msg();
        return EXIT_FAILURE;
    }
    return 0;
}
