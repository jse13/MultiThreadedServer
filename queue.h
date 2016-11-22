//Barebones queue implementation using a primitive linked list
#include "threadpool.h"

typedef struct _node_st {

	struct _node_st* nextNode;
	dispatch_fn taskFunction;
	void* taskArgs;

} _node;

typedef struct _queue_st {

	_node* firstNode;
	_node* lastNode;
	int sizeOfQueue;

} _queue;

void init_queue(_queue*);

int queue_empty(_queue*);

void enqueue(_queue*, _node*);

_node* dequeue(_queue*);

int get_size(_queue*);
