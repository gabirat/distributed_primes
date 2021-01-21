#ifndef JOB_H
#define JOB_H

#include <stdbool.h>

typedef enum job_status_e {
    JOB_REQUEST,
    JOB_INPROGRESS,
    JOB_FINISHED
} job_status;

typedef struct job_s {
    u_int64_t data;
    bool result;
} job;


#endif /* JOB_H */