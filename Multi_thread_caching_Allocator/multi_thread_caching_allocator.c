#include <stdio.h>

const size_t size_classes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096,8192, 16384};

int get_size_index(size_t request){
 
    if(request>16384){
        return -1;
    }

    if(request<=16){
        request = 16;
    }

    request -= 1;

    request |= request>>1;  
    request |= request>>2;
    request |= request>>4;
    request |= request>>8;
    request |= request>>16;
    
    request+=1;

    return __builtin_ctz(request) - 4;
}

struct Block{
 struct Block* next;
}
