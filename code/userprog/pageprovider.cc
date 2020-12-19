#ifdef CHANGED
//#include "pageprovider.h"
#include "bitmap.h"
#include "system.h"



PageProvider::PageProvider(){
    // BitMap btmp[NumPhysPages];
    // int available[NumPhysPages];
    // for(int i =0; i<NumPhysPages; i++){
    //     btmp[i] = new BitMap(PageSize);
    //     available[i] = 0;
    // }
    bitmap = new BitMap(NumPhysPages);

}

int PageProvider::GetEmptyPage(){
    // for(int i =0; i<NumPhysPages; i++){
    //     if(available[i] == 1)
    //     {
    //         available[i] == 0;
    //         for(int bt =0; bt<PageSize; bt++){
    //             btmp[i].Clear(bt);
    //         }
    //         return i;
    //     }
    // }
    // return -1;
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