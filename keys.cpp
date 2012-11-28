#include "keys.h"

void CKeys::setkey(int key, int pos){
    keys[key] = pos;
}

int CKeys::getkey(int key){
    return keys[key];
}

void CKeys::reset()
{
    for(int i=0; i < 256; i++)
        if(keys[i] == -1)
            keys[i] = 0;
}
