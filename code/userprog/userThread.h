#include "thread.h"
#include "machine.h" 

extern int do_ThreadCreate(int f,int arg);

static void StartUserThread(void*schmurtz);

extern int  do_ThreadExit();