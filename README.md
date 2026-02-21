# ThreadPool Engine

High-performance work scheduler with lock-free queues and work-stealing algorithms.

## Project Objective

### The Problem
Enterprise systems need to process thousands of concurrent operations efficiently. Creating a thread for each operation is prohibitively expensive due to memory overhead and context-switching costs. Instead, modern systems use a **ThreadPool** - a fixed number of worker threads that efficiently process any number of tasks.

### What Is a ThreadPool?
A ThreadPool is a design pattern where:
1. **Tasks** (work units) are submitted to a queue, each task knows what function to execute and what data to use
2. **Worker threads** continuously pull tasks from the queue and execute them
3. **Load balancing** ensures work is distributed evenly across workers
4. **Lock-free operations** allow multiple threads to access shared data structures without expensive locks

```
                    ThreadPool
    ┌──────────────────────────────────────┐
    │                                      │
    │  Task Queue: [T1][T2][T3][T4][T5]    │ ← Tasks waiting
    │              ↓   ↓   ↓            │
    │     ┌──────┬──────┬──────┬──────┐    │
    │     │Worker│Worker│Worker│Worker│    │ ← Fixed thread pool
    │     │  #1  │  #2  │  #3  │  #4  │    │
    │     └──────┴──────┴──────┴──────┘    │
    └──────────────────────────────────────┘
```

## Features
- Dynamic thread pool with worker management
- Lock-free task queue using atomic operations (C + Assembly)
- Work-stealing scheduler for load balancing
- Task dependencies and priority scheduling
- Performance monitoring with RDTSC profiling
- CPU affinity and thread pinning

## Components
- `threadpool.c/h` - Core thread pool manager
- `lockfree_queue.c/h` - Lock-free queue with assembly atomics
- `task.c/h` - Task structure and scheduling
- `scheduler.c/h` - Work-stealing scheduler
- `atomic_ops.asm` - x86-64 assembly atomic operations
- `perf_monitor.c/h` - Performance metrics and profiling

## Build
```bash
make
```

This compiles all source files into object files in the `build/` directory and links them into the executable `build/main`.

## Run

```sh
./build/main
```