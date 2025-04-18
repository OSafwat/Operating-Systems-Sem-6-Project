
#include <stdio.h>
#include <string.h>


enum State {
  Ready,
  Running,
  Blocked
};

struct PCB
{
   int ProcessID;
   enum State ProcessState;
   int Priority;
   long ProgramCounter;
   long Mem_Bound_Up;
   long Mem_Bound_Down;
};



  int MemorySize = 60;
  int WordSize = 32;
  

  static FILE *fptr = fopen("Program_1.txt", "r");

  
  char[] -,getInstruction(){
    char Line[WordSize];


    if (fgets(Line, sizeof(Line), fptr) != NULL)
    {
      //printf("%s", Line);
      return Line
    }
    else{
    return NULL;
    }

  }

  
  
  int main()
  {
 
  
char *token;
char Line[];
Line = getInstruction();
printf("%s",*(Line));



  }


