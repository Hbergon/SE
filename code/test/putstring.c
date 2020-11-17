#include "syscall.h"


int main(){
    char * test = "test\n 123";
    int i;
    for(i = 0; i< 10; i++){
        PutString(test);
    } 

    Halt();

}