#include "worker.h"

bool is_prime(uint64_t num) {
     if (num <= 1) return 0;
     if (num % 2 == 0 && num > 2) return 0;
     for(uint64_t i = 3; i * i <= num; i+= 2) {
         if (num % i == 0) return false;
     }
     return true;
}

int worker_handle_incoming_data(worker* self) {
    int ret = read(self->sv[1], &self->current_job_status, sizeof(self->current_job_status));
    if (ret < 0) return -1;
    if (ret == 0) return 1;
    return 0;
}

int worker_send_status(worker* self) {
    int ret = write(self->sv[1], &self->current_job_status, sizeof(self->current_job_status));
    if (ret <= 0) return -1;
    return 0;
}

int worker_main(worker* self) {
    close(self->sv[0]); //close master socket
    int nfds = 1;
    struct pollfd *pfds = calloc(nfds, sizeof(*pfds));
    if (pfds == NULL) {
        close(self->sv[1]);
        return -1;
    }
    pfds->fd = self->sv[1];
    pfds->events |= POLLIN;
    pfds->events |= POLLHUP;

    //main loop
    while (1) {
        //wait until some data is avaliable
        int ret = poll(pfds, nfds, -1);
        if (ret < 0) {
            close(self->sv[1]);
            return -1;
        }

        //Data is ready to be read
        if (pfds->revents & POLLIN) {
            //read freom the socket
            if (worker_handle_incoming_data(self) < 0) {
                close(self->sv[1]);
                free(pfds);
                return -1;
            }

            //do the job or quit
            if (self->current_job_status == JOB_REQUEST) {
                self->current_job->result = is_prime(self->current_job->data);
                self->current_job_status = JOB_FINISHED;
                worker_send_status(self);
            }
            else if (self->current_job_status == JOB_QUIT) {
                close(self->sv[1]);
                free(pfds);
                return 0;
            }
            
        }
        //master closed the socket
        //specs say POLLHUP is set when other end of the channel is closed
        else if (pfds->revents & POLLHUP) {
            close(self->sv[1]);
            free(pfds);
            return 0;
        }
    }
    return 0;
}