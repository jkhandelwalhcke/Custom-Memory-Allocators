#include <stdio.h>
#include <stdlib.h>

//PASTE the allocator code here, directly
// THIS FUNCTION IS TO SPECIFICALLY HELP VISUALISE THE HEAP ALLOCATION LIVE AS IT HAPPENS
void debug_heap() {
    printf("\n--- Current Heap State ---\n");
    //this pointer is added at the top of the heap, to see as things happen
    block_header *current = (block_header *)heap_start;
    
    // Walk the heap until you hit the end
    while (current != NULL && current->size > 0) {
        printf("[Addr: %p | Size: %zu bytes | Free: %s]\n",
               (void*)current,
               current->size,
               current->is_free ? "YES" : "NO");
        
        // Jump to the next block using pointer arithmetic!
        // (Cast to char* to step by exact bytes, then cast back)
        current = (block_header *)((char *)current + current->size);
    }
    */
    
    printf("--------------------------\n\n");
}

/* * 2. THE TEST BENCH
 */
int main() {
    printf("Initializing heap...\n");
    // my_init(); // Call your setup/sbrk function if you have one

    printf("\n[Test 1] Allocating Block A (100 bytes)...\n");
    void *a = my_malloc(100);
    printf("Block A payload at: %p\n", a);
    debug_heap();

    printf("\n[Test 2] Allocating Block B (200 bytes)...\n");
    void *b = my_malloc(200);
    printf("Block B payload at: %p\n", b);
    
    printf("\n[Test 3] Allocating Block C (50 bytes)...\n");
    void *c = my_malloc(50);
    printf("Block C payload at: %p\n", c);
    debug_heap();

    printf("\n[Test 4] Freeing Block B (Creating a gap in the middle)...\n");
    // This is the freeing function, use the code within the main code to replace here
    //my_free(b);
    debug_heap(); 

    printf("\n[Test 5] Allocating Block D (150 bytes) - Should reuse Block B's space!\n");
 
    void *d = my_malloc(150);
    printf("Block D payload at: %p\n", d);
    debug_heap();

    printf("\nTests complete.\n");
    return 0;
}
