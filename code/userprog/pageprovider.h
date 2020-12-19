#include "bitmap.h"

class PageProvider{
    public :
        PageProvider();

        int GetEmptyPage();

        void ReleasePage(int i);

        int NumAvailPage();

    private : BitMap *bitmap;
};