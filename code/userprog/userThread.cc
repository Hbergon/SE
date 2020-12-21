

#include "thread.h"
#include "machine.h" 
#include "system.h"



struct structsc //utilisé pour passer 2 argument en 1 à StartUserThread
{
int a; 
int b;
};

static void StartUserThread(void*schmurtz){

    

    int i;

    struct structsc * sch = (struct structsc * ) schmurtz;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    //initilaisation du PC à l'adresse de la première fonction a executée et du stack pointer du nouveau thread à debut de l'espace alloué

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
    DEBUG ('x', "Initializing stack register to 0x%x\n",
	   tmp);

    //mise du parmatre de la première fonction à etre executée dans le registre 4

    machine->WriteRegister (4, sch->b);

    machine->Run();
 }




 int do_ThreadCreate(int f,int arg){
    Thread * nt = new Thread("newThread");
    struct structsc *ptr, schmurtz; 

    ptr = &schmurtz;

    ptr->a = f; 
    ptr->b = arg;

    DEBUG ('x', "start thread with param : 0x%x, 0x%x\n",ptr->b,ptr->a);

    nt->Start(StartUserThread, (void*) ptr );

    return 0;
 }

 

 int do_ThreadExit(){
    currentThread->Finish();
    return 0;
 }


 static void StartUserProc(void * arg){
    int i;
    for (i = 0; i < NumTotalRegs; i++)
	   machine->WriteRegister (i, 0);

    machine->WriteRegister (PCReg, 0);

    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    int tmp = AllocateUserStack();
    machine->WriteRegister (StackReg, tmp  );
    DEBUG ('x', "Initializing stack register to 0x%x\n",
	   tmp);

    machine->Run();
 }



 int do_ForkExec(const char *  s){
    Thread * nt = new Thread("newThread");

    nt->space = new AddrSpace( new OpenFile((int) (long) s)) ;

    nt->Start(StartUserProc, NULL); //pas de param encoreS

    return 0;
 } 