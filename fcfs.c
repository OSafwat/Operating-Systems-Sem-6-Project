// fcfs.c
#include "fcfs.h"

#include <stdlib.h>
#include <string.h>

#include <unistd.h>    // for fork(), _exit()
#include <sys/types.h> // for pid_t
#include <sys/wait.h>  // for waitpid(), WIFEXITED, WEXITSTATUS

struct FCFS_Scheduler
{
    FCFS_Task *tasks; // dynamic array of tasks
    int n;            // current number of tasks
    int cap;          // current capacity of the array
};

FCFS_Scheduler *fcfs_create(int initial_capacity)
{
    FCFS_Scheduler *S = malloc(sizeof *S);
    if (!S)
        return NULL;

    S->tasks = calloc(initial_capacity, sizeof *S->tasks);
    if (!S->tasks)
    {
        free(S);
        return NULL;
    }

    S->n = 0;
    S->cap = initial_capacity;
    return S;
}

void fcfs_destroy(FCFS_Scheduler *S)
{
    if (!S)
        return;

    // free each strdup'd path
    for (int i = 0; i < S->n; i++)
        free(S->tasks[i].path);

    free(S->tasks);
    free(S);
}

int fcfs_insert_task(FCFS_Scheduler *S,
                     int arrival,
                     const char *path)
{
    // grow array if needed
    if (S->n >= S->cap)
    {
        int nc = S->cap * 2;
        FCFS_Task *t = realloc(S->tasks, nc * sizeof *t);
        if (!t)
            return -1;
        S->tasks = t;
        S->cap = nc;
    }

    // initialize the new slot
    FCFS_Task *T = &S->tasks[S->n++];
    T->arrival = arrival;
    T->path = strdup(path);      // make our own copy
    T->pcb.ProcessID = S->n;     // simple ID assignment
    T->pcb.ProcessState = Ready; // initial state
    T->pcb.Priority = 0;         // stored but unused by FCFS
    T->pcb.ProgramCounter = 0;
    T->pcb.Mem_Bound_Up = 0;
    T->pcb.Mem_Bound_Down = 0;

    return 0;
}

int fcfs_task_count(const FCFS_Scheduler *S)
{
    return S->n;
}

int fcfs_run_next(FCFS_Scheduler *S)
{
    if (S->n == 0)
        return -1;

    // find the task with earliest arrival time
    int idx = 0;
    for (int i = 1; i < S->n; i++)
    {
        if (S->tasks[i].arrival < S->tasks[idx].arrival)
            idx = i;
    }

    FCFS_Task *T = &S->tasks[idx];
    T->pcb.ProcessState = Running;

    pid_t pid = fork();
    if (pid < 0)
    {
        return -1; // fork failed
    }
    if (pid == 0)
    {
        // child: replace with the task executable
        execl(T->path, T->path, NULL);
        _exit(1); // if exec fails
    }

    // parent: wait for the child to finish
    int status;
    waitpid(pid, &status, 0);
    T->pcb.ProcessState = Blocked;
    return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}

int fcfs_remove_task(FCFS_Scheduler *S, int process_id)
{
    for (int i = 0; i < S->n; i++)
    {
        if (S->tasks[i].pcb.ProcessID == process_id)
        {
            free(S->tasks[i].path);
            // shift remaining tasks down by one
            memmove(&S->tasks[i],
                    &S->tasks[i + 1],
                    (S->n - i - 1) * sizeof *S->tasks);
            S->n--;
            return 0;
        }
    }
    return -1; // not found
}