# High-Performance Multi-Threaded Caching Allocator

A custom TCMalloc-style memory allocator written in C, designed to drastically outperform standard `glibc` `malloc` in highly concurrent, multi-threaded environments. 

## Architecture Overview

This allocator minimizes lock contention by providing each thread with its own lock-free cache of memory blocks. 

* **Thread-Local Caching:** Utilizes `__thread` for lightning-fast allocations on the fast path.
* **Central Fallback:** Safely routes to a mutex-locked central cache when thread-local supplies are exhausted.
* **Segregated Size Classes:** Implements 11 distinct size classes (16B to 16KB) to efficiently manage memory and handle varied request sizes.
* **Direct OS Mapping:** Memory requests exceeding 16KB bypass the cache and are routed directly to the operating system via `mmap`.

## Performance Benchmarks

Tested against standard `glibc` `malloc` with 100,000 allocations per thread. While standard `malloc` maintains an edge in single-threaded micro-benchmarks and memory footprint, this custom allocator completely dominates in multi-threaded throughput.

| Metric | Custom Allocator | Standard `glibc` |
| :--- | :--- | :--- |
| **8-Thread Throughput** | **98.69M ops/sec** | 48.67M ops/sec |
| **16-Thread Throughput** | **93.43M ops/sec** | 76.90M ops/sec |
| **p50 Latency (Single Thread)** | 50 ns | **20 ns** |
| **Fragmentation Waste** | 26.00% | **< 5%** (Coalescing) |

## Build and Usage

This project uses standard GNU Make for compilation. The provided Makefile is configured to compile the testing suite with `-pthread` for concurrency support and `-O3` for maximum performance optimization.

**To compile the allocator and the benchmark suite, simply navigate to the project directory and run:**

```bash
make
```
**To run the benchmarks:**
```bash
./test_file #Where the test file is the file you make for the benchmarking tests
```
**To clean the compiled files**
```bash
make clean
```

## Future Optimizations

* Refine the single-threaded fast-path to close the baseline latency gap with `glibc`.
* Optimize memory block splitting and padding to reduce the 26% fragmentation overhead inherent to segregated size classes.
