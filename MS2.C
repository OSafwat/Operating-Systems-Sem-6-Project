#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "PCB.h"
#include <queue.h>

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

long Pid = 0;

Memory_Word memory[60] = {0};
long Currentline = 0;

//
//
//
//
//
//
//

static FILE *fptr1 = fopen("Program_1.txt", "r");

char *GetInstruction(FILE *fptr)
{
  char *line = (char *)malloc(99);

  if (fgets(line, 99, fptr) != NULL)
  {
    // printf("%s", line);
    // printf("%s",line);
    return line;
  }
  else
  {
    return NULL;
  }
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

void Parse(char *line, PCB *CurrentProcess)
{
  line = strtok(line, " ");

  if (strcmp(line, "print") == 0)
  {
    line = strtok(NULL, " ");
    printf("%s", line);
  }

  else
  {
    if (strcmp(line, "assign") == 0)
    {

      char *charValue;
      long intValue;
      line = strtok(NULL, " ");
      char *variableName = line;     // we saved the name of the variable they used
      printf("%s \n", variableName); //
      line = strtok(NULL, " ");

      charValue = readFile(line, CurrentProcess);

      for (long i = 0; i < 3; i++)
      {
        if (memory[CurrentProcess->memStart + 6 + i].name == NULL && memory[CurrentProcess->memStart + 6 + i].value == NULL)
        {
          Memory_Word memoryWord;
          memoryWord.name = variableName;
          memoryWord.value = charValue;
          memory[CurrentProcess->memStart + 6 + i] = memoryWord;
          break;
        }
      }
    }
    else
    {
      if (strcmp(line, "writeFile") == 0)
      {
        for (long i = 0; i < 3; i++)
        {
          if (memory[CurrentProcess->memStart + 6 + i].name && memory[CurrentProcess->memStart + 6 + i].value == NULL)
          {
            line = strtok(NULL, " ");
            Memory_Word memoryWord;
            memoryWord.name = line;
            line = strtok(NULL, " ");
            memoryWord.value = line;
            memory[CurrentProcess->memStart + 6 + i] = memoryWord;
            break;
          }
        }
      }
      else
      {
        if (strcmp(line, "printFromTo") == 0)
        {
          if (!OutputMutexAcquire())
          {
          }

          long Value1;
          long Value2;
          line = strtok(NULL, " ");
          Value1 = strtol(line, NULL, 10);
          line = strtok(NULL, " ");
          Value2 = strtol(line, NULL, 10);

          for (long i = Value1; i <= Value2; i++)
          {
            if (i != Value2)
              printf("%d,", i);
            else
            {
              printf("%d", i);
            }
          }
        }

        else
        {
          if (strcmp(line, "semWait") == 0)
          {
            line = strtok(NULL, " ");
            if (line = "userInput")
              InputMutexAcquire();
            if (line = "userOutput")
              OutputMutexAcquire();
            if (line = "file")
              FileMutexAcquire();
          }

          else
          {
            if (strcmp(line, "semSignal") == 0)
            {
              line = strtok(NULL, " ");
              if (line = "userInput")
                InputMutexRelease();
              if (line = "userOutput")
                OutputMutexRelease();
              if (line = "file")
                FileMutexRelease();
            }
            else
            {
              printf("Error: Instruction Not Recognized\n");
            }
          }
        }
      }
    }
  }
}

char *readFile(char *line, PCB *currentProcess)
{
  if (strcmp(line, "readFile") == 0)
  {
    line = strtok(NULL, " ");
    for (long i = currentProcess->memStart; i < currentProcess->memEnd; i++)
    {
      if (strcmp(line, memory[i].name) == 0)
      {
        return memory[i].value;
      }
    }
  }
  else if (strcmp(line, "input") == 0)
  {
    char *Charvalue;
    printf("Please Enter a Value: ");
    scanf("%s", Charvalue);
    return Charvalue;
  }
  else
  {
    return line;
  }
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

long main()
{
  char *Line;

  do
  {
    Line = GetInstruction();
    Parse(Line, DummyPCB);
  } while (Line != NULL)
}

void createPCB(FILE *fptr, long priority)
{
  PCB pcb;
  bool f = true;

  for (int i = 0; i < 60; i++)
  {
    long count = 0;
    long start = i;
    long sz = countFile(fptr);
    while (i < 60 && count < 9 + sz && memory[i].name == NULL && memory[i].value == NULL)
    {
      i++;
      count++;
    }
    if (count == 9 + sz)
    {
      long startIndex = start;
      Memory_Word memoryWord;
      memoryWord.name = "Process ID";
      memoryWord.value = NumToString(Pid);
      Pid++;
      memory[start++] = memoryWord;

      memoryWord.name = "Process State";
      memoryWord.value = "Ready";
      memory[start++] = memoryWord;

      memoryWord.name = "Current Priority";
      memoryWord.value = NumToString(priority);
      memory[start++] = memoryWord;

      memoryWord.name = "Program Counter";
      memoryWord.value = "0";
      memory[start++] = memoryWord;

      memoryWord.name = "Memory Start";
      memoryWord.value = NumToString(startIndex);
      memory[start++] = memoryWord;

      memoryWord.name = "Memory End";
      memoryWord.value = NumToString(startIndex + 9 + sz);
      memory[start] = memoryWord;

      start += 4;
      memoryWord.name = "Instruction";
      while (start < startIndex + 9 + sz)
      {
        memoryWord.value = GetInstruction(fptr);
        memory[start] = memoryWord;
        start++;
      }
    }
  }
}
// long processID;
// State *processState;
// long priority;
// long programCounter;
// long memStart;
// long memEnd;

long countFile(FILE *fptr)
{
  long count = -1;
  char *Line;
  do
  {
    Line = GetInstruction();
    count++;

  } while (Line != NULL);
}