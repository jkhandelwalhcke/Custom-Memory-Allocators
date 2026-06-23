# Custom Memory Allocator: Implicit Free List
## Overview 
This repository contains a custom implementation of a dynamic memory allocator (`malloc`, `free`, and `realloc`) written in C. It bypasses the standard `libc` allocator to directly manage heap memory using `sbrk()`.
The primary goal of this project is to understand low-level systems architecture, memory hierarchies, and the explicit management of heap fragmentation.

## Architecture & Design Choices
The allocator manages memory using an implicit free list with boundary tag coalescing.

### Memory block layout : 
To ensure compatibility and performance, the allocator enforces strict 8-byte double-word alignment for all payloads.
Each memory block consists of:
- *Header (4 bytes / 1 Word)*: Encodes the total block size and an allocated/free status bit in the lowest-order bit.
- *Payload*: The actual memory returned to the user.
- *Padding*: Optional, used to maintain the 8-byte alignment requirement.
- *Footer (4 bytes / 1 Word)*: An exact copy of the header, allowing constant-time bidirectional traversal of the heap.
- *Minimum Block Size*: 16 bytes (4-byte header + 8-byte minimum payload + 4-byte footer).  

### Core Algorithms used :  
- *Placement policy : Next-Fit Search*
- *Coalescing : Immediate*
- *Block Splitting : Dynamic splitting of blocks if requested is lower than the selected block.*


## A gentle note :
Since an implicit free list allocator is not exactly made with the intentions of handling full scale operating systems, at least not in the modern times, it is not expected for it to perform exceptionally high on any benchmark.

## 🚀 How to Run and Test

Because this allocator is built from the ground up to manage heap space directly, it is currently contained within a single source file for easy compilation. You have two ways to test it out:

### 1. The Sandbox Method (Recommended & Safe)

The easiest way to see the allocator in action without replacing your system's memory management is to just copy all the function code from the `implicit_free_list.c` to the `trial_run.c` file, which has been given functions to help visualise the heap state at multiple points within the file. 
Inside `main()` of `trial_run.c`, calls for the custom allocation functions (e.g., `my_malloc`, `my_free`) and print out the block headers or pointer addresses to verify coalescing and splitting logic have been made so that it is easy to see the things happening under the hood.

Compile and run the file using standard GCC:
```
# Compile the file
gcc -o test_allocator allocator.c

# Run the executable
./test_allocator
```
### 2. The LD_PRELOAD method (Advanced)
This is basically a step where you let this allocator run the operating system's execution, basically handing it over the controls.
First we load the program code into a shared object file.
Then, the next step is to use LD_PRELOAD to run a regular system command by calling that specific shared object file. 
```
gcc -shared -fPIC -o mplicit_free_list.so mplicit_free_list.c
LD_PRELOAD=./implicit_free_list.so ls
```
What this does is that it tells the system to utilize the specific program rather than the generally utilized and highly optimized `glibc` allocator.
It is advised that you do not run a lot of commands or specificallt targeting commands using this method, if you wanna save yourself a segmentation fault `SEGSEGV` or two.
Though, oonce again, it is advised that this allocator is not suitable for running operating systems even nearly close to what we have today. 
