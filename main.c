#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <windows.h>

typedef struct
{
    double release_time;
    double start_time;
    double finish_time;
    double execution_time;
    double waiting_time;
    double response_time;
    double turnaround_time;
    double CPU_usefulwork;

    pthread_t tid;
} thread_info_t;

double getTimeMilliseconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

void calculateData(thread_info_t *data)
{
    data->execution_time = data->finish_time - data->start_time;
    data->waiting_time = data->start_time - data->release_time;
    data->response_time = data->start_time - data->release_time;
    data->turnaround_time = data->finish_time - data->release_time;
    data->CPU_usefulwork = data->execution_time / (data->execution_time + data->waiting_time);
}

void displayData(thread_info_t *data)
{
    printf("The metrics for thread %lu are the following:\n", data->tid);
    printf("The release time is: %lf\n", data->release_time);
    printf("The start time is: %lf\n", data->start_time);
    printf("The finish time is: %lf\n", data->finish_time);
    printf("The execution time is: %lf\n", data->execution_time);
    printf("The waiting time is: %lf\n", data->waiting_time);
    printf("The response time is: %lf\n", data->response_time);
    printf("The turnaround time is: %lf\n", data->turnaround_time);
    printf("The CPU Useful Work is: %lf\n", data->CPU_usefulwork);
    printf("\n\n\n");
}

thread_info_t tinfo[3];

typedef struct
{
    int a, b;
} NumberPair;

typedef struct
{
    char a, b;
} CharacterPair;

void *calculateSumAverageProduct(void *arg)
{
    // 0
    tinfo[0].tid = pthread_self();
    tinfo[0].start_time = getTimeMilliseconds();

    NumberPair *input = (NumberPair *)arg;
    int sum = (input->b * (input->b + 1)) / 2 - (input->a * (input->a - 1)) / 2;
    long product = 1;
    for (int i = input->a; i <= input->b; i++)
        product *= i;
    int average = (input->a + input->b) / 2;
    printf("The sum, product, and average of of all integers between and including those two numbers are %d, %ld, and %d respectively.\n", sum, product, average);

    tinfo[0].finish_time = getTimeMilliseconds();
    return NULL;
}

void *printThreeStatements(void *arg)
{
    // 1
    tinfo[1].tid = pthread_self();
    tinfo[1].start_time = getTimeMilliseconds();

    printf("Meow\n");
    printf("I really love cats\n");
    printf("Ok I'll print the thread ID now\n");
    pthread_t id = pthread_self();
    int tid = (int)id;
    printf("The thread ID is: %d\n", tid);

    tinfo[1].finish_time = getTimeMilliseconds();
    return NULL;
}

void *displayAllLettersInBetween(void *arg)
{
    // 2
    tinfo[2].tid = pthread_self();
    tinfo[2].start_time = getTimeMilliseconds();
    CharacterPair *input = (CharacterPair *)arg;
    printf("The characters in between your input characters are: \n");
    for (char c = input->a; c <= input->b; c++)
    {
        printf("%c ", c);
    }
    printf("\n");

    tinfo[2].finish_time = getTimeMilliseconds();
    return NULL;
}

void *emptyFunction(void *arg)
{
    return NULL;
}

int main()
{
    HANDLE hProcess = GetCurrentProcess();
    if (!SetProcessAffinityMask(hProcess, 1))
    {
        printf("Failed to set process affinity.\n");
        return 1;
    }

    int a, b;
    char c, d;
    printf("Please enter the two numerical inputs.\n");
    scanf("%d%d", &a, &b);
    printf("Please enter the two character inputs with a single space in between.\n");
    scanf(" %c %c", &c, &d);
    NumberPair input;
    input.a = a;
    input.b = b;
    CharacterPair input1;
    input1.a = c;
    input1.b = d;

    /* FIFO */
    // pthread_attr_t attr;
    // pthread_attr_init(&attr);
    // pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    // struct sched_param param;
    // param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    // pthread_attr_setschedparam(&attr, &param);
    // pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    /* RR */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    struct sched_param param;
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_attr_setschedparam(&attr, &param);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

    /* THIS SEGMENT OF CODE IS TO WARM UP */
    pthread_t dummy;
    pthread_create(&dummy, &attr, emptyFunction, NULL);
    pthread_join(dummy, NULL);
    /* THIS SEGMENT OF CODE IS TO WARM UP */

    pthread_t threads[3];

    tinfo[0].release_time = getTimeMilliseconds();
    pthread_create(&threads[0], &attr, calculateSumAverageProduct, &input);

    tinfo[1].release_time = getTimeMilliseconds();
    pthread_create(&threads[1], &attr, printThreeStatements, NULL);

    tinfo[2].release_time = getTimeMilliseconds();
    pthread_create(&threads[2], &attr, displayAllLettersInBetween, &input1);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
        calculateData(&tinfo[i]);
    }

    printf("\n\n");

    displayData(&tinfo[0]);
    displayData(&tinfo[1]);
    displayData(&tinfo[2]);

    double CPU_useful_work = (tinfo[0].CPU_usefulwork + tinfo[1].CPU_usefulwork + tinfo[2].CPU_usefulwork) / 3.0;
    double max_finish = max(max(tinfo[0].finish_time, tinfo[1].finish_time), tinfo[2].finish_time);
    double min_release = min(min(tinfo[0].release_time, tinfo[1].release_time), tinfo[2].release_time);
    double total_execution_time = tinfo[0].execution_time + tinfo[1].execution_time + tinfo[2].execution_time;
    double total_span = max_finish - min_release;
    double CPU_utilization = (total_execution_time / total_span) * 100;

    printf("\n");

    printf("The Average CPU Useful Work metric is: %lf\n", CPU_useful_work);
    printf("The CPU Utilization Percentage is: %lf%%\n", CPU_utilization);

    return 0;
}
