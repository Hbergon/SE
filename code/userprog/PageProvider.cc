
#include "PageProvider.h"
#include "bitmap.h"



PageProvider::PageProvider(){
    BitMap btmp[NumPhysPages];
    int available[NumPhysPages];
    for(int i =0; i<NumPhysPages; i++){
        btmp[i] = new BitMap(PageSize);
        available[i] = 0;
    }

}

int PageProvider::GetEmptyPage(){
    for(int i =0; i<NumPhysPages; i++){
        if(available[i] == 1)
        {
            available[i] == 0;
            for(int bt =0; bt<PageSize; bt++){
                btmp[i].Clear(bt);
            }
            return i;
        }
    }
    return -1;
}

int PageProvider::ReleasePage(int i){
    if(available[i] == 1){
        return -1;
    }else{
        
    }

}

int PageProvider::NumAvailPage(){

}

