#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    NumberPair *input = (NumberPair *)arg;
    int sum = (input->b * (input->b + 1)) / 2 - (input->a * (input->a - 1)) / 2;
    int product = 1;
    for (int i = input->a; i <= input->b; i++)
        product *= i;
    int average = (input->a + input->b) / 2;
    printf("The sum, product, and average of of all integers between and including those two numbers are %d, %d, and %d respectively.\n", sum, product, average);
}

void *displayAllLettersInBetween(void *arg)
{
    CharacterPair *input = (CharacterPair *)arg;
    printf("The characters in between your input characters are: \n");
    for (char c = input->a; c <= input->b; c++)
    {
        printf("%c ", c);
    }
}

int main()
{
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
    pthread_t thread;
    pthread_create(&thread, NULL, calculateSumAverageProduct, &input);
    pthread_join(thread, NULL);
    pthread_t thread1;
    pthread_create(&thread1, NULL, displayAllLettersInBetween, &input1);
    pthread_join(thread1, NULL);
    return 0;
}
