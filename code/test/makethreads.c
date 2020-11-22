#include "syscall.h"

void fonc(char n){

    char * c = "bonjour je suis un thread ";
    int i;
    for(i=0;i<n;i++){
        PutChar(c[i]);
        }
    PutChar(n);
    PutChar('\n');
    ThreadExit();
}

int main(){
    PutChar('\n');
    char c = '7';
    void f(void *) = fonc;
    ThreadCreate(f, &c);
    Halt();

}