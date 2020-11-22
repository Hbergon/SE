

#include "thread.h"
#include "machine.h" 
#include "system.h"



struct structsc
{
int a; 
int b;
};

static  void StartUserThread(void*schmurtz){

    int i;

    struct structsc * sch = (struct structsc * ) schmurtz;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);



    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, sch->a);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    int tmp = AllocateUserStack();
    machine->WriteRegister (StackReg, tmp  );
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   tmp);



    machine->WriteRegister (4, sch->b);

     machine->Run();
 }




 int do_ThreadCreate(int f,int arg){

     Thread * nt = new Thread("newThread");


    struct structsc * schmurtz;

    schmurtz->a = f; 
    schmurtz->b = arg;


     nt->Start(StartUserThread, (void*) schmurtz );





     return 0;
 }

 

 int do_ThreadExit(){
    currentThread->Finish();
    return 0;
 } 