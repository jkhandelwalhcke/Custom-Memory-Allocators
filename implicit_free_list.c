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