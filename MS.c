#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "PCB.h"
// #include <queue.h>

typedef struct
{
    char *name;
    char *value; // can be long or string, and can be parsed later
} Memory_Word;

long ParseInt(Memory_Word *memoryWord)
{
    return strtol(memoryWord->value, NULL, 10);
}

char *NumToString(long number)
{
    if (number == 0)
    {
        return (char *)"0";
    }
    int sz = 0;
    long temp = number;
    while (temp)
    {
        sz++;
        temp /= 10;
    }
    temp = number;
    char *string = (char *)calloc(sz, sizeof(char));
    int position = sz - 1;
    while (temp)
    {
        int digit = temp % 10;
        temp /= 10;
        string[position--] = '0' + digit;
    }
    return string;
}

long MemorySize = 60;
long WordSize = 32;

int Pid = 1;

Memory_Word memory[60] = {0};
long Currentline = 0;

//
//
//
//
//
//
//

//
//
//
//
//
//
//

char *GetInstruction(FILE *file, int line_number)
{
    if (file == NULL || line_number < 1)
        return NULL;

    static char buffer[99]; // Adjust size as needed
    int current_line = 1;

    rewind(file);

    while (fgets(buffer, sizeof(buffer), file))
    {
        if (current_line == line_number)
        {
            return buffer;
        }
        current_line++;
    }

    return NULL; // Line not found
}

//
//
//
//
//
//
//

void makeReady(PCB *pcb)
{
    memory[(pcb->memStart + 1)].value = (char *)"Ready";
}

//
//
//
//
//
//
//

static bool fileKey = true;
static bool inputKey = true;
static bool outputKey = true;

bool FileMutexAcquire()
{
    if (!fileKey)
        return false;
    else
        return true;
}

void FileMutexRelease()
{
    fileKey = true;
}

bool InputMutexAcquire()
{
    if (!inputKey)
        return false;
    else
        return true;
}
void InputMutexRelease()
{
    inputKey = true;
}

bool OutputMutexAcquire()
{
    if (!outputKey)
        return false;
    else
        return true;
}

void OutputMutexRelease()
{
    outputKey = true;
}

//
//
//
//
//
//
//

char *readFile(char *line, PCB *currentProcess)
{
    printf("%s\n", line);
    if (strcmp(line, "readFile") == 0)
    {
        line = strtok(NULL, " \n");
        for (long i = currentProcess->memStart; i < currentProcess->memEnd; i++)
        {
            if (strcmp(line, memory[i].name) == 0)
            {
                return memory[i].value;
            }
        }
    }
    if (strcmp(line, "input") == 0)
    {
        printf("im in input 1010101");
        char *input = (char *)malloc(100 * sizeof(char));
        printf("Enter input: ");
        scanf("%s", input);
        return input;
    }
    return (char *)"INCORRECT";
}

int Parse(PCB *CurrentProcess)
{
    CurrentProcess->processState = (State *)malloc(sizeof(State));
    *CurrentProcess->processState = Running;
    char *MemLine = (memory[CurrentProcess->programCounter]).value;
    if (MemLine == NULL)
    {
        printf("IT IS NULLLL\n");
        return 404;
    }

    char *line = strdup(MemLine);
    char *temp = strtok(line, " \n");
    line = temp;

    if (strcmp(line, "print") == 0)
    {
        line = strtok(NULL, " \n");
        printf("%s\n", line);
        return 0;
    }
    if (strcmp(line, "assign") == 0)
    {

        char *charValue;
        long intValue;

        line = strtok(NULL, " \n");
        char *variableName = line; // we saved the name of the variable they used
        line = strtok(NULL, " \n");

        printf("%s", line);
        charValue = readFile(line, CurrentProcess);

        for (long i = 0; i < 3; i++)
        {
            if (memory[CurrentProcess->memStart + 6 + i].name == NULL && memory[CurrentProcess->memStart + 6 + i].value == NULL)
            {
                printf("insideloop\n");
                Memory_Word memoryWord;
                memoryWord.name = variableName;
                memoryWord.value = charValue;
                memory[CurrentProcess->memStart + 6 + i] = memoryWord;
                break;
            }
        }
        return 0;
    }
    if (strcmp(line, "writeFile") == 0)
    {
        for (long i = 0; i < 3; i++)
        {
            if (memory[CurrentProcess->memStart + 6 + i].name && memory[CurrentProcess->memStart + 6 + i].value == NULL)
            {
                line = strtok(NULL, " \n");
                Memory_Word memoryWord;
                memoryWord.name = line;
                line = strtok(NULL, " \n");
                memoryWord.value = line;
                memory[CurrentProcess->memStart + 6 + i] = memoryWord;
                break;
            }
        }
        return 0;
    }
    if (strcmp(line, "printFromTo") == 0)
    {
        if (!OutputMutexAcquire())
        {
        }

        long Value1;
        long Value2;

        line = strtok(NULL, " \n");
        char *v1 = strdup(line);
        line = strtok(NULL, " \n");
        char *v2 = strdup(line);

        for (long i = CurrentProcess->memStart + 6; i < CurrentProcess->memStart + 9; i++)
        {
            if (strcmp(memory[i].name, v1) == 0)
            {
                Value1 = ParseInt(&memory[i]);
            }
            if (strcmp(memory[i].name, v2) == 0)
            {
                Value2 = ParseInt(&memory[i]);
            }
        }

        for (long i = Value1; i <= Value2; i++)
        {
            if (i != Value2)
                printf("%d,", i);
            else
            {
                printf("%d", i);
            }
        }
        return 0;
    }
    if (strcmp(line, "semWait") == 0)
    {
        printf("semWait\n");
        line = strtok(NULL, " \n");
        if (strcmp(line, "userInput"))
        {
            if (!InputMutexAcquire())
                return 2;
        }

        else
        {
            if (strcmp(line, "userOutput"))
            {
                if (!OutputMutexAcquire())
                    return 3;
            }
            else
            {
                if (strcmp(line, "file"))
                {
                    if (!FileMutexAcquire())
                        return 1;
                }

                return 0;
            }
        }
    }
    if (strcmp(line, "semSignal") == 0)
    {
        line = strtok(NULL, " \n");
        if (strcmp(line, "userInput"))
        {
            InputMutexRelease();
            return -2;
        }

        else
        {
            if (strcmp(line, "userOutput"))
            {
                OutputMutexRelease();
                return -3;
            }

            else
            {
                if (strcmp(line, "file"))
                {
                    FileMutexRelease();
                    return -1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    printf("Error: Instruction Not Recognized\n");
    return 0;
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//

long countFile(FILE *fptr)
{
    int count = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        count++;
    }
    return count;
}

PCB createPCB(FILE *fptr, long priority)
{
    PCB pcb;
    bool f = true;
    int count = 0;
    int start = 0;
    int sz = 0;
    if (fptr == NULL)
    {
        printf("File not found\n");
        return pcb;
    }
    for (int i = 0; i < 60; i++)
    {
        count = 0;
        start = i;
        sz = countFile(fptr);
        while (i < 60 && count < 9 + sz && memory[i].name == NULL && memory[i].value == NULL)
        {
            i++;
            count++;
        }
        if (count == 9 + sz)
        {
            long startIndex = start;
            Memory_Word memoryWord;
            memoryWord.name = (char *)"Process ID";
            memoryWord.value = (char *)NumToString(Pid);
            pcb.processID = Pid;
            Pid++;
            memory[start++] = memoryWord;

            memoryWord.name = (char *)"Process State";
            memoryWord.value = (char *)"Ready";
            State *s = (State *)malloc(sizeof(State));
            *s = Ready;
            pcb.processState = s;
            memory[start++] = memoryWord;

            memoryWord.name = (char *)"Current Priority";
            memoryWord.value = (char *)NumToString(priority);
            pcb.priority = priority;
            memory[start++] = memoryWord;

            memoryWord.name = (char *)"Program Counter";
            memoryWord.value = (char *)NumToString(startIndex + 9);
            pcb.programCounter = startIndex + 9;
            memory[start++] = memoryWord;

            memoryWord.name = (char *)"Memory Start";
            memoryWord.value = (char *)NumToString(startIndex);
            pcb.memStart = startIndex;
            memory[start++] = memoryWord;

            memoryWord.name = (char *)"Memory End";
            memoryWord.value = (char *)NumToString(startIndex + 9 + sz);
            pcb.memEnd = startIndex + 9 + sz;
            memory[start] = memoryWord;
            start += 4;

            int j = 1;

            for (int i = start; i < (startIndex + 9 + sz); i++)
            {
                Memory_Word temp;
                temp.name = (char *)"Instruction";
                temp.value = (GetInstruction(fptr, j));
                memory[i].name = strdup(temp.name);
                memory[i].value = strdup(temp.value);
                j++;
            }
            return pcb;
        }
    }

    pcb.processID = -1;
    return pcb;
}

int main()
{
    static FILE *fptr1 = fopen("Program_1.txt", "r");
    if (fptr1 == NULL)
    {
        printf("Error: Could not open Program_1.txt\n");
        return -1; // Handle error appropriately
    }

    PCB pcb1 = createPCB(fptr1, 2);
    // for(int i=6;i<15;i++)
    // printf("Instruction at MEM %d : %s \n",i,memory[i].value);      //Instructions ARE being inserted properly

    for (int i = 0; i < 15; i++)
    {
        Parse(&pcb1);
        (&pcb1)->programCounter++;
    }
    fclose(fptr1);
    for (int i = 6; i < 9; i++)
        printf("Instruction at MEM %d : %s \n", i, memory[i].value);
    return 0;
}

// GetInstruction -(used in)-> Inserting Instructions into Memory -(used in)->
// Whenever Parse is called, we extract instructions from memory