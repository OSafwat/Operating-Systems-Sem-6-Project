#ifndef FCFS_H
#define FCFS_H

#include "pcb.h"

/// one FCFS task = its PCB + path to its “file” + arrival time
typedef struct
{
    PCB pcb;
    char *path;
    int arrival;
} FCFS_Task;

typedef struct FCFS_Scheduler FCFS_Scheduler;

FCFS_Scheduler *fcfs_create(int initial_capacity);

/**

Destroy an FCFS scheduler and free all resources.
*/
void fcfs_destroy(FCFS_Scheduler *s);

/**

Insert a new FCFS task:



arrival: arrival time for ordering



path:    string identifier or executable path



priority: stored in pcb.Priority (ignored by FCFS)

Returns 0 on success, -1 on failure.
*/
int fcfs_insert_task(FCFS_Scheduler *s,
                     int arrival,
                     const char *path,
                     int priority);

/**

Run the next FCFS task (smallest arrival):



forks & execs the task->path



waits for completion, updates PCB state to Blocked

Returns exit code (>=0) or -1 if empty.
*/
int fcfs_run_next(FCFS_Scheduler *s);

/**

Remove a task by its ProcessID (from pcb):

Returns 0 if found & removed, -1 if not found.
*/
int fcfs_remove_task(FCFS_Scheduler *s, int process_id);

/**

Number of tasks currently queued.
*/
int fcfs_task_count(const FCFS_Scheduler *s);

#endif // FCFS_H