/*===============================================================
  threads_notes.c  —  Revision notes: THREADS (pthreads)
  ===============================================================

  BIG PICTURE
  - A thread is a “path of execution” inside one process.
  - Threads share:
      address space (heap), globals, open file descriptors
    Threads have their own:
      stack, registers, thread-local storage

  WHY THREADS?
  - Parallelism on multicore
  - Concurrency: keep app responsive while doing I/O
  - Structure: producer/consumer pipelines

  CORE PTHREADS API
  - pthread_create: start a thread
  - pthread_join: wait for a thread to finish
  - pthread_mutex_*: mutual exclusion (protect shared data)
  - pthread_cond_*: condition variables (wait/signal state changes)
  - pthread_rwlock_*: many readers / one writer
  - pthread_once: run initialization once

  GOLDEN RULE
  - If two threads access the same shared data and at least one writes:
    you need synchronization (mutex/rwlock/atomics), or it's a data race.

  COMMON PITFALLS
  - Passing pointer to stack variable that goes out of scope
  - Forgetting to join/detach threads => resource leak
  - Deadlocks:
      lock A then B in one place, B then A elsewhere
    Fix: consistent lock ordering, minimize lock scope.
  - Holding locks while doing slow I/O (blocks other threads)
  - Assuming printf is “thread-safe enough”: it’s usually internally locked,
    but output lines can interleave unless you serialize.

  MUTEX NOTES
  - pthread_mutex_lock / unlock protect a critical section.
  - Keep critical section short.
  - Always unlock on all control paths.

  CONDITION VARIABLE NOTES
  - Used to wait until a predicate becomes true.
  - Always wait in a loop:
        while (!predicate) pthread_cond_wait(...)
    because spurious wakeups exist and state may change before you run.

  BUILD
    gcc threads_notes.c -o threads_notes -pthread

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    int counter;
    pthread_mutex_t lock;
} Shared;

static void *worker(void *arg) {
    Shared *s = (Shared *)arg;

    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&s->lock);
        s->counter++;
        pthread_mutex_unlock(&s->lock);
    }
    return NULL;
}

int main(void) {
    Shared s;
    s.counter = 0;
    pthread_mutex_init(&s.lock, NULL);

    pthread_t t1, t2;
    if (pthread_create(&t1, NULL, worker, &s) != 0) {
        perror("pthread_create t1");
        return 1;
    }
    if (pthread_create(&t2, NULL, worker, &s) != 0) {
        perror("pthread_create t2");
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter = %d (expected 200000)\n", s.counter);

    pthread_mutex_destroy(&s.lock);
    return 0;
}

/* --------------------------------------------------------------
   QUICK PATTERNS (NOT FULL CODE)

   1) Thread arguments: allocate per-thread or keep storage alive
      BAD:
        for (...) { int x=i; pthread_create(...,&x); } // x changes/out of scope
      GOOD:
        int *x = malloc(sizeof *x); *x=i; pthread_create(..., x);

   2) Detach if you will never join:
        pthread_detach(pthread_self());
      Detached threads cannot be joined.

   3) Condition variable: producer/consumer sketch
      shared queue + mutex + cond
      consumer:
        lock
        while(queue empty) cond_wait
        pop item
        unlock
      producer:
        lock
        push item
        cond_signal or cond_broadcast
        unlock
-------------------------------------------------------------- */
