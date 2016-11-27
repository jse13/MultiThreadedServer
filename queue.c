#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define QUEUE_DEBUG 0

void init_queue(_queue* q) {

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDo: Initialize queue.\n");

	q->lastNode = 0;
	q->firstNode = 0;
	q->sizeOfQueue = 0;

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDone: Initialized queue.\n");
}

int queue_empty(_queue* q) {

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDo: empty queue.\n");

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

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDone: emptied queue.\n");

	return 0;
}

void enqueue(_queue* q, _node* n) {

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDo: Enqueue node.\n");

	if(q->sizeOfQueue != 0) {
		_node* previousFirstNode = q->firstNode;
		q->firstNode = n;
		q->firstNode->nextNode = previousFirstNode;
		q->sizeOfQueue++;
		return;
	}
	else {
		//Special case if no items are in queue
		if(QUEUE_DEBUG)
			fprintf(stderr, "Info: Enqueue special case.\n");
		q->firstNode = n;
		q->lastNode = n;
		q->sizeOfQueue++;
	}

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDone: Enqueued node.\n");
}

_node* dequeue(_queue* q) {

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDo: Dequeue node.\n");

	_node* nodeToReturn = q->lastNode;

	//Get the second-to-last node
	_node* walkThroughNode = q->firstNode;

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tInfo: walking to second to last node.\n");
	if(q->sizeOfQueue == 2) {
		nodeToReturn = q->lastNode;
		q->lastNode = q->firstNode;
		q->sizeOfQueue--;
		return nodeToReturn;
	}
	else if(q->sizeOfQueue == 1) {
		nodeToReturn = q->firstNode;
		q->firstNode = NULL;
		q->lastNode = NULL;
		q->sizeOfQueue--;
		return nodeToReturn;
	}
	else if(q->sizeOfQueue == 0) {
		return NULL;
	}
	else {
		int i;
		for(i = 1; i < (q->sizeOfQueue) - 1; i++)
			walkThroughNode = walkThroughNode->nextNode;
	}
	if(QUEUE_DEBUG)
		fprintf(stderr, "\tInfo: found second to last node.\n");

	q->lastNode = walkThroughNode;
	q->sizeOfQueue--;

	if(QUEUE_DEBUG)
		fprintf(stderr, "\tDone: Dequeued node.\n");

	return nodeToReturn;

}

int get_size(_queue* q) { return q->sizeOfQueue; }
