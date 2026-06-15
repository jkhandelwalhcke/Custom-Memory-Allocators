# Custom Memory Allocators 

A collection of custom memory allocators written in C, built to explore heap management, fragmentation reduction, and thread-safe dynamic memory allocation at the system level. 

## 📖 Background & Theory

At the core of every operating system and user-space application is memory management. The standard `libc` implementations of `malloc`, `free`, and `realloc` are highly optimized black boxes. This repository breaks open that black box. 

These projects explore the trade-offs in memory allocator design, from managing block headers and mitigating fragmentation (internal and external) to handling the complexities of lock contention in multi-threaded environments. By taking control of the heap space (via system calls like `sbrk` or `mmap`), these allocators demonstrate the fundamental mechanics of dynamic memory.

## 📂 Implementations

This repository is structured into distinct allocator models, scaling from foundational concepts to highly concurrent designs:

* **[Implicit Free List](./implicit_free_list):** The baseline allocator. It utilizes a simple header/footer architecture to track block sizes and allocation status, implementing basic block splitting and coalescing to manage heap fragmentation.
* **[Multi-Thread Caching](./multi_thread_caching):** *(Work in Progress)* An advanced, thread-safe allocator designed to minimize lock contention. It implements thread-local caching mechanisms and mutexes to efficiently handle concurrent allocation requests without bottlenecking performance.

## ⚙️ Compilation & Usage

*(Add your instructions here on how someone would compile this. For example:)*
To compile the allocators, navigate to the specific directory and run `make`:

```bash
cd implicit_free_list
make
