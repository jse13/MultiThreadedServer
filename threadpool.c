/**
 * threadpool.c
 *
 * This file will contain your implementation of a threadpool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "threadpool.h"
#include "queue.h"

// _threadpool is the internal threadpool structure that is
// cast to type "threadpool" before it given out to callers
typedef struct _threadpool_st {
   // you should fill in this structure with whatever you need

   // add task queue
   _queue* taskQueue;
   // TODO: add mutex
   pthread_mutex_t poolMutex;
   // TODO: add condition variable
   pthread_cond_t poolCondVar;
   // TODO: add array for threads
   pthread_t* arrayOfThreads;
   // number of threads
   int numberOfActiveThreads;
   // TODO: add flag
   int poolShutdownFlag;

} _threadpool;

threadpool create_threadpool(int num_threads_in_pool) {
  _threadpool *pool;

  // sanity check the argument
  if ((num_threads_in_pool <= 0) || (num_threads_in_pool > MAXT_IN_POOL))
    return NULL;

  pool = (_threadpool *) malloc(sizeof(_threadpool));
  if (pool == NULL) {
    fprintf(stderr, "Out of memory creating a new threadpool!\n");
    return NULL;
  }

  // add your code here to initialize the newly created threadpool
  init_queue(pool->taskQueue);
  pthread_mutex_init(&pool->poolMutex, NULL);
  pthread_cond_init(&pool->poolCondVar, NULL);
  pool->arrayOfThreads = malloc(sizeof(pthread_t) * MAXT_IN_POOL);
  pool->numberOfActiveThreads = 0;
  pool->poolShutdownFlag = 0;

  return (threadpool) pool;
}


void dispatch(threadpool from_me, dispatch_fn dispatch_to_here,
	      void *arg) {
  _threadpool *pool = (_threadpool *) from_me;

  // add your code here to dispatch a thread
}

void destroy_threadpool(threadpool destroyme) {
  _threadpool *pool = (_threadpool *) destroyme;

  // add your code here to kill a threadpool
}

