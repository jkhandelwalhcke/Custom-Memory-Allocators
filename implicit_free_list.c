#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // To use sbrk

/* Primary macros */
#define WSIZE       4       /* word / header/footer size in bytes */
#define DSIZE       8       /* Double word size / alignment in bytes */
#define CHUNKSIZE  (1<<12)  /* Initial heap size 4KB */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
/* Because blocks are 8-byte aligned, the lowest 3 bits of size are always 0. 
   The lowest bit is used to store if a block is allocated (1) or free (0) */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read / write a word at p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* For a given block payload with address p, computing the header and footer address using window size */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, computint address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */

/* Declarations for helper functions */
static void *extend_heap(size_t words);
static void *coalesce(void *bp);

/* * mm_init - Initializing the memory manager 
 */
int mm_init(void) {
    /* Initializing empty heap */
    if ((heap_listp = sbrk(4 * WSIZE)) == (void *)-1) {
        return -1;
    }
    
    // If the first block is freed, to retain the follower's info, we keep a dummy 'prologue' block, and 'epilogue' block to 

    PUT(heap_listp, 0);                            /* Alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));     /* Epilogue header */
    
    /* Moves the heap pointer forward to the prologue block's footer */
    heap_listp += (2 * WSIZE); 

    /* Extends the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
        return -1;
    }
    
    return 0;
}

//Here, we also declare an extensoion function, since we may need to ask for more memory from the heap.
/* extend_heap - Extend heap with free block and return its block pointer */
static void *extend_heap(size_t words) {
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain 8-byte alignment */
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    if ((long)(bp = sbrk(size)) == -1) {
        return NULL;
    }

    /* Initialize free block header/footer and the new epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}

//Here, introducing the coalescing function, which would do the management for unallocated, yet asked blocks of memory.

/* coalesce - Boundary tag coalescing. Return ptr to coalesced block */
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1: Both neighbors allocated */
        return bp;
    }
    else if (prev_alloc && !next_alloc) {      /* Case 2: Next is free */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    else if (!prev_alloc && next_alloc) {      /* Case 3: Previous is free */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    else {                                     /* Case 4: Both neighbors free */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    
    return bp;
}

