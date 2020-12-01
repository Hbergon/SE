// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userThread.h"







//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


int copyStringFromMachine(int from, char *to, unsigned size);


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }
		  #ifdef CHANGED
		case SC_PutChar:
		{
			DEBUG ('s', "PutChar.\n");
			int param = machine->ReadRegister (4); //récuperation du premier parametre et appel du consoledriver dessus
			consoledriver->PutChar(param);
			break;
		}
		case SC_PutString:
		{
			DEBUG ('s', "PutString.\n");
			int param = machine->ReadRegister (4); //récuperation du premier parametre adresse cible de l'écriture
			int MAX_STRING_SIZE =10;
        	char tmp[MAX_STRING_SIZE];  
        	copyStringFromMachine(param, tmp, MAX_STRING_SIZE);  //copy du string a l adresse param jusq'au tableau local tmp
			consoledriver->PutString(tmp);  //on écrit le contenu de tmp
			break;
		}
		
		case SC_GetChar:
		{
			DEBUG ('s', "GetChar.\n");
			machine->WriteRegister(2,   consoledriver->GetChar()); // appel du consoledriver dessus et renvoit sur second registre
			break;
		}
		case SC_GetString:
		{
			DEBUG ('s', "GetString.\n");
			int param1 = machine->ReadRegister (4); //récuperation du premier parametre représente une adresse de string
			int param2 = machine->ReadRegister (5); //récuperation du second parametre represente une taille
			int MAX_STRING_SIZE =10;
        	char tmp[MAX_STRING_SIZE];
			copyStringtoMachine(tmp, param1, param2); 
			consoledriver->GetString(tmp,param2);
			break;
		}



		case SC_ThreadCreate :
		{
			int param1 = machine->ReadRegister (4); //lis les 2 parametres dans r4 et r5
			int param2 = machine->ReadRegister (5);
			DEBUG ('s', "ThreadCreate.\n");
			int success;
			success = do_ThreadCreate(param1, param2); //lance le traitant
			machine->WriteRegister(2, success); //renvoie le résultat de l'appel systeme
			break;

		}

		case SC_ThreadExit :
		{
			int success = do_ThreadExit(); //lance le traitant
			machine->WriteRegister(2, success); //renvoie le résultat de l'appel systeme
			break;
		}

		#endif
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}




