#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore*readAvail;
static Semaphore*writeDone;
static void ReadAvailHandler(void*arg) {
    (void) arg; readAvail->V();
}
static void WriteDoneHandler(void*arg) { 
    (void) arg; writeDone->V();
}

ConsoleDriver::ConsoleDriver(const char*in,const char*out){
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
    }

    ConsoleDriver::~ConsoleDriver(){
        delete console;
        delete writeDone;
        delete readAvail;
    }
    void ConsoleDriver::PutChar(int ch){ //écrit un char sur la sortie de console et attends que l'écriture soit bien terminée

        if (ch == EOF) {
          return;
        }

        
        console->TX (ch);
        writeDone->P ();

        
    }
    int ConsoleDriver::GetChar(){//attend que la lecture soit prete et récupère un char sur l'entrée de  console  

        readAvail->P ();
        int ch = console->RX ();

        return ch;

    }
    void ConsoleDriver::PutString(const char s[]){//écrit un string, char par char en attendant que lécriture soit disponible
        for(int i=0;;i++){
            
            if (s[i] == '\0') {
                return;
            }

            
            console->TX (s[i]);
            writeDone->P ();
        }
    }
    void ConsoleDriver::GetString(char*s,int n){//lecture d'un string de taille n char par char en attendant a chaque fois que la lecture soit disponible,
    //se termine plus tot si il ya a un EOF, un \0 ou un \n  
        int tmp;

        for(int i=0;i<n;i++){

            readAvail->P ();
            tmp = console->RX ();

            if(tmp == '\0' || tmp == EOF){
                *(s+i) = '\0';
                return;
            }
            if(tmp == '\n'){
                *(s+i) = '\n';
                *(s+(i+1)) = '\0';
                return;
            }

            *(s+i) = tmp;


        
        }
    }

   
    int copyStringFromMachine(int from, char *to, unsigned size){ //copie vers to à partir de l'adresse from, size * char

        int tmp;


        for(unsigned int x=0; x <size; x++){

            machine->ReadMem(from + x, 1, &tmp); //récuperation d'un char
            
            to[x] = tmp;

            if(tmp == '\0'){ //on vérifie que la chaine ne fini pas avant
                return x;
            }

        }

        to[size] = '\0';


        return size;
    }


    int copyStringtoMachine(char * to, int from, unsigned size){ //ne fonctionne sans doute pas puisque est la copie de copyStringFromMachine
        int tmp;


        for(unsigned int x=0; x <size; x++){

            machine->ReadMem(from + x, 1, &tmp);
            

            to[x] = tmp;

            if(tmp == '\0'){
                return x;
            }

        }

        to[size] = '\0';
        return size;
    } 

    

    #endif // CHANGED
