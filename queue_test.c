//Simple driver to test my queue struct

#include<stdio.h>
#include<stdlib.h>

#include "queue.h"

int main() {

	_queue* testQueue = malloc(sizeof(_queue));
	init_queue(testQueue);

	_node* nodeToPush;

	int i;
	for(i = 0; i < 10; i++) {

		nodeToPush = malloc(sizeof(_node));
		nodeToPush->taskFunction = (void*) i;
		enqueue(testQueue, nodeToPush);

	}
	nodeToPush = dequeue(testQueue);

	printf("Dequeued %d", (int) nodeToPush->taskFunction);

	queue_empty(testQueue);
	free(testQueue);

	return 0;
}
