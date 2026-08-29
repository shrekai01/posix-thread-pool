# POSIX Thread Pool in C

A lightweight, robust thread pool implementation written in C using the POSIX threads (pthreads) API. This project demonstrates concurrent task execution, thread lifecycle management, and safe memory handling using operating system synchronization primitives.

## 🚀 Features
* **Dynamic Task Queue**: A thread-safe linked-list based queue for submitting tasks dynamically.
* **Concurrency Control**: Utilizes Mutex locks to prevent race conditions during queue operations (enqueue/dequeue).
* **Efficient Sleep/Wake Mechanism**: Implements Counting Semaphores to avoid busy-waiting. Worker threads sleep efficiently when the queue is empty and are awakened instantly when new tasks arrive.
* **Graceful Shutdown**: Properly cancels and joins all worker threads while preventing memory leaks and cleaning up synchronization resources.

## 🧠 System Architecture & Synchronization
This implementation highlights several key system programming concepts:

1. **The Worker Routine (`sem_wait`)**: 
   Workers run in an infinite loop waiting for tasks. Instead of polling the queue, they block on a counting semaphore (`sem_wait`). This yields the CPU back to the OS scheduler, making the thread pool highly resource-efficient.
   
2. **Critical Sections (`pthread_mutex`)**: 
   The `worktodo_head` and `worktodo_tail` pointers are shared resources. Both `enqueue()` and `dequeue()` are wrapped in `pthread_mutex_lock` and `pthread_mutex_unlock` to guarantee thread-safe task distribution.

3. **Memory Management**:
   Task arguments and function pointers are dynamically allocated via `malloc` in `pool_submit()`. The worker thread safely `free()`s this memory immediately after task execution to ensure zero memory leaks during prolonged runtime.

4. **Thread Cancellation**:
   The `pool_shutdown()` function utilizes `pthread_cancel()` followed by `pthread_join()`. Because `sem_wait()` is a valid POSIX cancellation point, idling workers are safely terminated without deadlocking.

## 📂 Project Structure

```text
.
├── client.c          # Example client program that generates and submits tasks
├── Makefile          # Build configuration for compiling the project
├── README.md         # Project documentation (this file)
├── threadpool.c      # Core thread pool implementation and synchronization logic
└── threadpool.h      # Header file containing function prototypes and structures
```

## 🛠️ Build & Run

### Prerequisites
* GCC Compiler
* POSIX-compliant operating system (Linux, macOS)

### Installation
```bash
git clone git@github.com:shrekai01/posix-thread-pool.git
```

### Compilation
A `Makefile` is provided for easy compilation. Simply run:
```bash
make
```
### Run
To run the example program, Simply enter
```bash
./example
```
