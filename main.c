#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    int a, b;
} NumberPair;

void *calculateSumAverageProduct(void *arg)
{
    NumberPair *value = (NumberPair *)arg;
    int sum = (value->b * (value->b + 1)) / 2 - (value->a * (value->a - 1)) / 2;
    int product = 1;
    for (int i = value->a; i <= value->b; i++)
        product *= i;
    int average = (value->a + value->b) / 2;
    printf("The sum, product, and average of of all integers between and including those two numbers are %d, %d, and %d respectively.\n", sum, product, average);
}

int main()
{
    int a, b;
    printf("Please enter the two numerical inputs.\n");
    scanf("%d%d", &a, &b);
    NumberPair input;
    input.a = a;
    input.b = b;
    pthread_t thread;
    pthread_create(&thread, NULL, calculateSumAverageProduct, &input);
    pthread_join(thread, NULL);
    return 0;
}
