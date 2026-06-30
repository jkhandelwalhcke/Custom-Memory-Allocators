#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>

#define NUM_CLASSES 11

// Declaring bucket sizes upto 16kb
const size_t size_classes[] = {16, 32, 64, 128, 256, 512, 1024, 2048, 4096,8192, 16384};

// Mutex lock for the individual threads that the freed memory would have, so that threads looking for a same memory cache, it hits that mutex lock, and waits in que till the lock is opened when A opens up....
pthread_mutex_t central_locks[NUM_CLASSES];

// array to hold pointers to the requested and achieved memory chucks of stoen.. This also keeps sureity that 


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

// Structure to maintain links to allocated blocks
struct Block{
 struct Block* next;
}

// This function fetches the raw memory blocks from the OS, which can then be passed on to the central heap. 
// Utilizes mmap, with flags that specify read/write memory and for the private/anonymous mamory declaration.
void* fetch_from_os(size_t size){
    void* mmap_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(mmap_ptr == MAP_FAILED ){
        return NULL;
    }

    return mmap_ptr;
}

// Function to initialize the memory to lock free array. 
void init_allocator(){
    for(int i = 0; i<NUM_CLASSES; i++){
        pthread_mutex_init(&central_locks[i], NULL);
    }
}


