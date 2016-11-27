Threadpool Structure
The threadpool structure contains internal data for a queue, a pthread mutex, a
pthread condition variable, an array of threads, an integer for the maximum
number of created threads, and an integer that serves as a flag that indicates
whether or not a thread should destroy itself. The queue is my own barebones
implementation that handles the creation, destruction, enqueueing, and
dequeueing of nodes that contain a function pointer and a list of arguments. The
mutex is mostly important for the condition variable, as it allows for thread
waiting.

The create_threadpool() function  does just that - initialize a new threadpool.
This function takes in the number of threads to be created in the pool. After
allocating the internal resources, the function will dispatch each thread it
created to the thread_main() function.
The dispatch() function will create a new node for the task queue with the
supplied function pointer and arguments. Afterwards, the function signals the
condition variable so that a thread will be woken up and will take the task.
The destroy_threadpool() function deallocates the memory created for the
threadpool structure, and tells the threads to terminate. The pool shutdown
flag will be set to true, and then the threads will be woken up via a broadcast
of the condition variable.

The thread_main() function is where the created threads live. This is an
infinite loop in which each thread will attempt for the pool mutex, then check
to see if there is a task available via the condition variable. If not, they
will wait for a signal from either dispatch() or destroy_threadpool(). If the
thread is not shutting down, a node from the task queue will be dequeued, and
the thread will execute the provided function with the given arguments. Note
that because the mutex is given up when a thread is sleeping, the size of the
queue must be checked twice - once before waiting and once after. Once a task
has been dequeued the mutex is safely unlocked.


Critical Sections
There are three functions with critical sections:

dispatch(): because this function adds a new task to the task queue, threads
must not touch the queue structure. The mutex is unlocked before a thread is
woken as that thread will try to re-acquire the lock as soon as possible. Since
dispatch() is not a multi-threaded function, there is no need to acquire the
lock before creating the node, since there will be no situation in which a task
is queued by a separate thread.

destroy_threadpool(): the lock is aquired by this function because the
threadpool structure is being modified. Strictly speaking this isn't necessary,
as no thread will modify the pool shutdown flag. However, any threads that are
iterating through the loop will effectively be "paused," so once the mutex is
released these threads will check the shutdown flag and immediately shut down.

thread_main(): there are technically two critical sections in this code.
Firstly, the mutex is acquired with the intent of safely dequeueing a task and
executing. Initially, a thread will check to see if there are any elements in
the task queue - if the mutex wasn't unlocked then a situation could arise in
which a thread sees a task in the queue, but before that thread can claim the
node, another thread has already dequeued the task. If there are no tasks in the
queue, the thread will wait and give up the mutex via pthread_cond_wait(). Upon
being woken up by dispatch() or destroy_threadpool(), the thread will once again
check the task queue; because pthread_cond_wait() re-acquires the mutex lock,
this is another critical section of code. If the task queue is empty, the mutex
is once again given up and the entire thread will jump to the beginning of the
loop and try again. If a task is available, the encapsulating node is dequeued,
the mutex is released, and the task is executed. There is no need for the thread
to hold the mutex after the node is dequeued, since that task is flushed from
the queue in the process.


Synchronization Specifications
The most important synchronization structure for the threadpool implementation
is the condition variable, as this structure allows threads to wait and be woken
up via signal. A mutex is also required, as condition variable functions like
pthread_cond_wait() require an already-acquired mutex to be passed in. Since
modification of the internal threadpool data is not thread-safe, a condition
variable and mutex are needed to prevent race conditions. Since dequeueing tasks
is mutative, the task queue (and by extension, the threadpool) must be locked
whenever a thread attempts to acquire a new task to perform. Otherwise,
situations would arise in which tasks are stolen from threads that are still
preparing to dequeue the task, the variable inside the queue that tracks how
many nodes are present may fall victim to race conditions (worse case falling
out-of-bounds and segmentation faulting), etc. Thus, threads will block if
another thread is accessing the queue, or, if the thread already has the mutex,
if there are no tasks ready in the queue. In the case of the latter, typically
dispatch() will wake up a sleeping thread when a new task is queued. When the
threadpool is being destroyed, destroy_threadpool() will signal all threads.
Because a thread may have been woken up by either function, the destroy
threadpool flag is used to differentiate the source of the signal. If set to 1,
the thread will exit.
