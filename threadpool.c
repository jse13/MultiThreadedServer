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

#define TPOOL_DEBUG 0

// _threadpool is the internal threadpool structure that is
// cast to type "threadpool" before it given out to callers
typedef struct _threadpool_st {
   // you should fill in this structure with whatever you need

   // add task queue
   _queue* taskQueue;
   // add mutex
   pthread_mutex_t poolMutex;
   // add condition variable
   pthread_cond_t poolCondVar;
   // add array for threads
   pthread_t* arrayOfThreads;
   // number of threads
   int numberOfActiveThreads;
   // add flag
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

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Initializing the threadpool.\n");

  // add your code here to initialize the newly created threadpool
  pool->numberOfActiveThreads = num_threads_in_pool;

  pool->taskQueue = malloc(sizeof(_queue));
  init_queue(pool->taskQueue);

  pthread_mutex_init(&pool->poolMutex, NULL);
  pthread_cond_init(&pool->poolCondVar, NULL);

  pool->arrayOfThreads = malloc(sizeof(pthread_t) 
      * pool->numberOfActiveThreads);

  pool->poolShutdownFlag = 0;

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: initialized the threadpool.\n"
                    "\tDo: Sending allocated threads to thread_main().\n");

  // Start initializing all the threads via thread_main()
  int i;
  for(i = 0; i < pool->numberOfActiveThreads; i++) {
          pthread_create(&pool->arrayOfThreads[i], NULL, thread_main, 
              (void*) pool);
  }

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: sent threads to thread_main().\n");

  return (threadpool) pool;
}


void dispatch(threadpool from_me, dispatch_fn dispatch_to_here,
	      void *arg) {
  _threadpool *pool = (_threadpool *) from_me;
  
  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Dispatching thread %ld.\n", (long) arg);
  
  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Allocating new task node.\n");

  // add your code here to dispatch a thread
  // lock the mutex
  pthread_mutex_lock(&pool->poolMutex);
  // allocate a new task
  _node* newTaskForQueue = malloc(sizeof(_node));
  // set task
  newTaskForQueue->taskFunction = dispatch_to_here;
  // set args
  newTaskForQueue->taskArgs = arg;

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: allocated new task node.\n");

  // add the node to the task queue
  enqueue(pool->taskQueue, newTaskForQueue);
  // signal the condition variable
  pthread_cond_signal(&pool->poolCondVar);
  // release the mutex
  pthread_mutex_unlock(&pool->poolMutex);

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: dispatched thread.\n");
}

void destroy_threadpool(threadpool destroyme) {
  _threadpool *pool = (_threadpool *) destroyme;

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Destroying threadpool.\n");
 
  // add your code here to kill a threadpool
  // lock the mutex
  pthread_mutex_lock(&pool->poolMutex);
  // set the destroy flag
  pool->poolShutdownFlag = 1;
  // broadcast the condition variable
  pthread_cond_broadcast(&pool->poolCondVar);
  // release mutex
  pthread_mutex_unlock(&pool->poolMutex);

  // wait for all threads to exit
  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Joining disparate threads.\n");

  int i;
  for(i = 0; i < pool->numberOfActiveThreads; i++) {
    pthread_join(pool->arrayOfThreads[i], NULL);
  }

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: joined all threads.\n");


  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDo: Cleaning up dynamic threadpool allocations.\n");

  // kill task queue
  queue_empty(pool->taskQueue);
  // kill mutex
  pthread_mutex_destroy(&pool->poolMutex);
  // kill cond var
  pthread_cond_destroy(&pool->poolCondVar);
  // kill the array
  free(pool->arrayOfThreads);

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: cleaned up dynamic memory.\n");

  if(TPOOL_DEBUG)
    fprintf(stderr, "\tDone: destroyed threadpool.\n");
}

void* thread_main(void* threadpool) {

  _threadpool* pool = (_threadpool*) threadpool;
  _node* taskToDispatchTo;
  
  if(TPOOL_DEBUG)
    fprintf(stderr, "\tInfo: A thread entered thread_main().\n");


  for(;;) {
    // lock mutex
    pthread_mutex_lock(&pool->poolMutex);

    if(get_size(pool->taskQueue) == 0) {
      // wait for condition variable when queue is empty
      if(TPOOL_DEBUG)
        fprintf(stderr, "\tInfo: A thread is waiting.\n");

      pthread_cond_wait(&pool->poolCondVar, &pool->poolMutex);
    }
    
    // check the shutdown signal
    if(pool->poolShutdownFlag == 1) {
      // shutdown the thread
      if(TPOOL_DEBUG)
        fprintf(stderr, "\tInfo: Exiting a thread.\n");
      pthread_exit(NULL);
    }

    if(TPOOL_DEBUG)
      fprintf(stderr, "\tInfo: A thread is doing a job.\n");

    // dequeue a task
    if(TPOOL_DEBUG)
      fprintf(stderr, "\tInfo: Size of queue is %d.\n", get_size(pool->taskQueue));
    taskToDispatchTo = dequeue(pool->taskQueue);
    // release mutex
    pthread_mutex_unlock(&pool->poolMutex);
    // execute the task function
    (taskToDispatchTo->taskFunction) (taskToDispatchTo->taskArgs);
  }
}
