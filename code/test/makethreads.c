#include "syscall.h"

void start_t(char n){

    char * c = "je suis un thread ";
    int i;
    for(i=0;i<18;i++){
        PutChar(c[i]);
        }
    PutChar(n);
    PutChar('\n');
    ThreadExit();
}

int main(){
    char ch1= '7';
    int value = (int) &start_t;
    ThreadCreate(value, &ch1);
    Halt();

}


