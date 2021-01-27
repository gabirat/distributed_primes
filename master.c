#include "master.h"

int spawn_workers(master* master, int n, worker* worker_self) {
    master->workers = malloc(sizeof(*master->workers) * n);
    if (master->workers == NULL) return -1;
    for (size_t i = 0; i < (size_t)n; i++) {
        worker* new_worker = calloc(1, sizeof(*new_worker));
        if (new_worker == NULL) return -1;
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, new_worker->sv) < 0) {
            free(new_worker);
            return -1;
        }
        new_worker->current_job_status = JOB_NOJOB;
        new_worker->id = i;
        new_worker->current_job = &(master->shared_jobs_space[i]);
        new_worker->pid = fork();
        if (new_worker->pid < 0) {
            free(new_worker);
            return -1;
        }
        if (new_worker->pid == 0) {
            *worker_self = *new_worker;
            master->number_of_workers = i+1;
            return 1;
        }
        master->workers[i] = new_worker;
        master->number_of_workers++;
    }
    return 0;
}

int enqueue_primes_from_file(queue* q, char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) return -1;
    uint64_t tmp;
    while (fscanf(f, "%lu", &tmp) != EOF) {
        enqueue(q, tmp);
    }
    fclose(f);
    return 0;
}

void init_master(master* master) {
    memset(master, 0, sizeof(*master));
}

void free_master(master* master) {
    free_queue(&master->primes_queue);
    for (size_t i = 0; i < master->number_of_workers; i++) {
        free(master->workers[i]);
    }
    free(master->workers);
}

void close_all_connections(master* master) {
    for (size_t i = 0; i < master->number_of_workers; i++) {
        close(master->workers[i]->sv[0]);
        close(master->workers[i]->sv[1]);
    }
} 

int master_handle_incoming_data(worker* worker) {
    int ret = read(worker->sv[0], &worker->current_job_status, sizeof(worker->current_job_status));
    if (ret < 0) return -1;
    if (ret == 0) return 1;
    return 0;
}

void print_result(worker* worker) {
    printf("Worker id: %d Number: %lu is_prime: %s\n", 
            worker->id,
            worker->current_job->data,
           (worker->current_job->result) ? "true" : "false");
}

int master_send_status(worker* worker) {
    int ret = write(worker->sv[0], &worker->current_job_status,
                                    sizeof(worker->current_job_status));
    if (ret <= 0) return -1;
    return 0;
}

int give_job(master* master, worker* worker) {
    worker->current_job->data = dequeue(&master->primes_queue);
    worker->current_job_status = JOB_REQUEST;
    int ret = master_send_status(worker);
    if (ret < 0) {
        close_all_connections(master);
        return -1;
    }
    master->num_given_jobs++;
    return 0;
}

int quit_worker(master* master, worker* worker) {
    int ret = master_send_status(worker);
    if (ret < 0) {
        close_all_connections(master);
        return -1;
    }
    return 0;
}

int master_main(master* master) {
    if (enqueue_primes_from_file(&master->primes_queue, 
                                  master->primes_file_path) < 0) {
        close_all_connections(master);
        return -1;
    }
    master->num_to_check = queue_length(&master->primes_queue);
    struct pollfd *pfds = calloc(master->number_of_workers, sizeof(*pfds));
    if (pfds == NULL) {
        close_all_connections(master);
        return -1;
    }

    for (size_t i = 0; i < master->number_of_workers; i++) {
        close(master->workers[i]->sv[1]); //close workers sockets
        pfds[i].fd = master->workers[i]->sv[0];
        pfds[i].events |= POLLIN;
    }
    
    for (size_t i = 0; i < master->number_of_workers; i++) { 
        worker* curr_worker = master->workers[i];
        curr_worker->current_job = &(master->shared_jobs_space[i]);
        if (is_empty(&master->primes_queue)) break;
        if (give_job(master, curr_worker) < 0) {
            close_all_connections(master);
            return -1;
        }
    }

    while (master->num_results < master->num_to_check) {
        int ret = poll(pfds, master->number_of_workers, -1);
        if (ret < 0) {
            close_all_connections(master);
            return -1;
        }
        for (size_t i = 0; i < master->number_of_workers; i++) {
            worker* curr_worker = master->workers[i];
            if (curr_worker->current_job_status == JOB_NOJOB) continue;
            if (pfds->revents & POLLIN) { //Data is ready to be read
                if (master_handle_incoming_data(curr_worker)) {
                    return -1;
                }
                if (curr_worker->current_job_status == JOB_FINISHED) {
                    print_result(curr_worker);
                    master->num_results++;
                    //Give a new job
                    if (master->num_given_jobs != master->num_to_check) {
                        if (give_job(master, curr_worker) < 0) {
                            return -1;
                        }
                    }
                    else {
                        curr_worker->current_job_status = JOB_NOJOB;
                    }
                }
            }
        }  
    }
    //Nothing to to, quit workers
    for (size_t i = 0; i < master->number_of_workers; i++) { 
        worker* curr_worker = master->workers[i];
        curr_worker->current_job_status = JOB_QUIT;
        if (quit_worker(master, curr_worker) < 0) {
            return -1;
        }
    }
    close_all_connections(master);
    return 0;
}