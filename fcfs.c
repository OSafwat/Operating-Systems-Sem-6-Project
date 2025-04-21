#include "fcfs.h"
#include "PCB.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

Scheduler *fcfs_create(int count)
{
    Scheduler *s = malloc(sizeof *s);
    s->tasks = calloc(count, sizeof s->tasks);
    s->maxCapacity = count;
    s->size = 0;
    return s;
}

int fcfs_insert_task(Scheduler *s, PCB pcb, char *path)
{
    if (s->size == s->maxCapacity)
        return -1; // to mark invalidity

    Task *currentSlot = &s->tasks[s->size];
    currentSlot->path = strdup(path);
    currentSlot->pcb = pcb;
    s->size++;
    return 0;
}

void fcfs_start(Scheduler *s)
{
    for (int i = 0; i < s->size; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            execl(s->tasks[i].path, s->tasks[i].path, (char *)NULL);
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }
}