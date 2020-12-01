#include "thread.h"
#include "machine.h" 

extern int do_ThreadCreate(int f,int arg); //traitant de l'appel system, créant un nouveau thread

static void StartUserThread(void*schmurtz); //initialisation du nouveau thread(préparation des registres)

extern int  do_ThreadExit();//fini le thread courant