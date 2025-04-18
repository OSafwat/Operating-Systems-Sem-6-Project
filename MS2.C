
#include <stdio.h>



struct PCB
{
   int ProcessID;
   enum State ProcessState;
   int Priority;
   long ProgramCounter;
   long Mem_Bound_Up;
   long Mem_Bound_Down;
};

enum State {
    Ready,
    Running,
    Blocked
  };

  int MemorySize = 60;
  
  int main()
  {
    FILE *fptr = fopen("text.txt", "r");
  
    char stra[9999];
    while (fgets(stra, sizeof(stra), fptr) != NULL)
    {
      printf("%s", stra);
    }
    fclose(fptr);
    return 0;
  }
