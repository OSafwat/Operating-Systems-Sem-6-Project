
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PCB.h"

int MemorySize = 60;
int WordSize = 32;

static FILE *fptr = fopen("Program_1.txt", "r");

char *getInstruction()
{
  char *Line = (char *)malloc(99);

  if (fgets(Line, 99, fptr) != NULL)
  {
    // printf("%s", Line);
    // printf("%s",Line);
    return Line;
  }
  else
  {
    return NULL;
  }
}

void parse(char *Line)
{
  Line = strtok(Line, " ");

  if (strcmp(Line, "print") == 0)
  {
    Line = strtok(NULL, " ");
    printf("%s", Line);
  }

  else
  {
    if (strcmp(Line, "assign") == 0)
    {

      char *Charvalue = (char *)malloc(100);
      int IntValue;
      Line = strtok(NULL, " ");
      char *VariableName = Line;     // we saved the name of the variable they used
      printf("%s \n", VariableName); //
      Line = strtok(NULL, " ");
      printf("Please Enter a Value: ");
      scanf("%s", Charvalue);
      if (strtol(Charvalue, NULL, 10) > 0)
      { // if its ==0 then input is a String
        IntValue = strtol(Charvalue, NULL, 10);
        free(Charvalue);
        printf("%d", IntValue);
      }
      else
      {
        printf("%s", Charvalue);
      }
    }
    else
    {
      if (strcmp(Line, "writeFile") == 0)
      {
      }

      else
      {
        if (strcmp(Line, "readFile") == 0)
        {
        }

        else
        {
          if (strcmp(Line, "printFromTo") == 0)
          {
            int Value1;
            int Value2;
            Line = strtok(NULL, " ");
            Value1 = strtol(Line, NULL, 10);
            Line = strtok(NULL, " ");
            Value2 = strtol(Line, NULL, 10);

            for (int i = Value1; i <= Value2; i++)
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
            if (strcmp(Line, "semWait") == 0)
            {
            }

            else
            {
              if (strcmp(Line, "semSignal") == 0)
              {
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
}

int main()
{
  char *Line;
  Line = getInstruction();

  // parse(Line);
  // Line = getInstruction();
  // printf("%s",Line);
  // parse(Line);
  // Line = getInstruction();
  // printf("%s",Line);
  // parse(Line);

  // do{
  //   Line=getInstruction();
  // }
  // while (Line!=NULL);
  // printf("%s",Line);
  // parse(Line);
}
