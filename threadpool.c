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

  // Start initializing all the threads via thread_main()

  return (threadpool) pool;
}


void dispatch(threadpool from_me, dispatch_fn dispatch_to_here,
	      void *arg) {
  _threadpool *pool = (_threadpool *) from_me;

  // add your code here to dispatch a thread
  // TODO: allocate a new task
  // TODO: lock the mutex
  // TODO: add the node to the task queue
  // TODO: signal the condition variable
  // TODO: release the mutex
}

void destroy_threadpool(threadpool destroyme) {
  _threadpool *pool = (_threadpool *) destroyme;

  // add your code here to kill a threadpool
  // TODO: lock the mutex
  // TODO: set the destroy flag
  // TODO: broadcast the condition variable
  // TODO: release mutex
  // TODO: wait for all threads to exit

  // TODO: kill task queue
  // TODO: kill mutex
  // TODO: kill cond var
  // TODO: kill the array
}

void thread_main(threadpool* threadpool) {

  _threadpool* pool = (_threadpool*) threadpool;
  _node* taskToDispatchTo;
  
  for(;;) {
    // lock mutex
    pthread_mutex_lock(&pool->poolMutex);

    // check task queue and shutdown signal
    if(pool->poolShutdownFlag == 1) {
      // TODO: shutdown the thread
    }
    else if(get_size(pool->taskQueue) == 0) {
      // TODO: wait for condition variable when queue is empty
    }
    // TODO: dequeue a task
    taskToDispatchTo = dequeue(pool->taskQueue);
    (taskToDispatchTo->taskFunction) (taskToDispatchTo->taskArgs);
    // release mutex
    pthread_mutex_unlock(&pool->poolMutex);
    // TODO: execute the task function
  }
}
