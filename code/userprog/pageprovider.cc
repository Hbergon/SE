#ifdef CHANGED
#include "bitmap.h"
#include "system.h"



PageProvider::PageProvider(){
    bitmap = new BitMap(NumPhysPages);

}

int PageProvider::GetEmptyPage(){
    int numPage = bitmap->Find();
    if(numPage!=-1){
        memset(machine->mainMemory + numPage*PageSize, 0, PageSize);
    }
    return numPage;
}

void PageProvider::ReleasePage(int i){
    bitmap->Clear(i);

}

int PageProvider::NumAvailPage(){
    return bitmap->NumClear();
}

#endif //CHANGED