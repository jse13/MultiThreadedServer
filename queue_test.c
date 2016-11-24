//Simple driver to test my queue struct

#include<stdio.h>
#include<stdlib.h>

#include "queue.h"

int main() {

	_queue* testQueue = malloc(sizeof(_queue));
	init_queue(testQueue);

	_node* nodeToPush;

	long i;


	printf("Enqueueing 1\n");
	nodeToPush = malloc(sizeof(_node));
	nodeToPush->taskFunction = (void*) 1;
	enqueue(testQueue, nodeToPush);

	printf("Enqueueing 3\n");
	nodeToPush = malloc(sizeof(_node));
	nodeToPush->taskFunction = (void*) 3;
	enqueue(testQueue, nodeToPush);

	printf("Enqueueing 7\n");
	nodeToPush = malloc(sizeof(_node));
	nodeToPush->taskFunction = (void*) 7;
	enqueue(testQueue, nodeToPush);


	printf("Full queue is size %d\n\n.", get_size(testQueue));


	nodeToPush = dequeue(testQueue);
	printf("Dequeued %d\n", (int) nodeToPush->taskFunction);
	printf("Size is now %d\n\n.", get_size(testQueue));

	nodeToPush = dequeue(testQueue);
	printf("Dequeued %d\n", (int) nodeToPush->taskFunction);
	printf("Size is now %d\n\n.", get_size(testQueue));

	nodeToPush = dequeue(testQueue);
	printf("Dequeued %d\n", (int) nodeToPush->taskFunction);
	printf("Size is now %d\n\n.", get_size(testQueue));


	printf("Size of queue is now %d\n\n", get_size(testQueue));
	queue_empty(testQueue);
	free(testQueue);

	return 0;
}
