
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <PCB.h>

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

int main()
{
  char *Line;
  Line = getInstruction();
  Line = getInstruction();
  strtok(Line, " ");

  if (strcmp(Line, "print") == 0)
  {
    Line = strtok(NULL, " ");
    printf("%s", Line);
  }

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
  if (strcmp(Line, "writeFile") == 0)
  {
  }

  if (strcmp(Line, "readFile") == 0)
  {
  }

  if (strcmp(Line, "printFromTo") == 0)
  {
  }

  if (strcmp(Line, "semWait") == 0)
  {
  }

  if (strcmp(Line, "semSignal") == 0)
  {
  }
}
