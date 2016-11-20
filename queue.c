#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue(_queue* q) {

	q->lastNode = 0;
	q->firstNode = 0;
	q->sizeOfQueue = 0;

}

int queue_empty(_queue* q) {

	if(q->sizeOfQueue == 0) {
		return 0;
	}

	_node* currentQueueItem = q->firstNode;
	_node* nextQueueItem = 0;
	int i;
	for(i = 0; i < q->sizeOfQueue; i++) {

		nextQueueItem = currentQueueItem->nextNode;
		free(currentQueueItem);
		currentQueueItem = nextQueueItem;

	}
}

void enqueue(_queue* q, _node* n) {

	if(q->sizeOfQueue != 0) {
		_node* previousFirstNode = q->firstNode;
		q->firstNode = n;
		q->firstNode->nextNode = previousFirstNode;
		q->sizeOfQueue++;
		return;
	}
	else {
		//Special case if no items are in queue
		q->firstNode = n;
		q->lastNode = n;
		q->sizeOfQueue++;
	}

}

_node* dequeue(_queue* q) {

	_node* nodeToReturn = q->lastNode;

	//Get the second-to-last node
	_node* walkThroughNode = q->firstNode;
	while(walkThroughNode->nextNode != q->lastNode)
		walkThroughNode = walkThroughNode->nextNode;

	q->lastNode = walkThroughNode;
	q->sizeOfQueue--;

	return nodeToReturn;

}
