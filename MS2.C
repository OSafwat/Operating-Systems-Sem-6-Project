
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

  int MemorySize = 2048;
  

int main()
{

    printf("Hello World");

}