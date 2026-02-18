# ThreadPool Engine

High-performance work scheduler with lock-free queues and work-stealing algorithms.

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