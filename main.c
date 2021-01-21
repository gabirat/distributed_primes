#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void usage() {
    puts("usage: primes number_of_workers primes_file");
}

int main(int argc, char const *argv[]) {
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
    

    return 0;
}
