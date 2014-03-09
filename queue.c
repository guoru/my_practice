/*
 * title: queue.c
 * date: 25 Oct, 2011
 */

#include "queue.h"
#include <stdlib.h>
#include <assert.h>

#define T queue_t

struct NODE{
	elem_t data;
	struct NODE* next;
};
typedef struct NODE NODE;

struct T{
	NODE* front;
	NODE* rear;
}; 

T* init_queue()
{
	queue_t *queue;

	queue = (T*) malloc(sizeof(T) );	
	if (!queue)
		exit(ERROR);

	queue->front = (NODE*)malloc(sizeof(NODE) );
	if (!queue->front)
		exit(ERROR);
	queue->rear = queue->front;

	queue->front->data = '\0';
	queue->rear->next = NULL;
	return(queue);
}

status queue_empty(T* queue)
{
	return(!(queue->front == queue->rear) );
}

status enqueue(T* queue, elem_t elem)
{
	queue->rear->next = (NODE*)malloc(sizeof(NODE) );

	if (!queue->rear->next)
		exit(ERROR);
	
	queue->rear->data = elem;	
	queue->rear = queue->rear->next;
	queue->rear->next = NULL;

	return(OK);
}

status dequeue(T* queue, elem_t *elem)
{
	assert(queue);

	if (!queue_empty(queue) )
		return(FALSE);

	*elem = queue->front->data;
	
	NODE* p;
	
	p = queue->front;
	queue->front = p->next;
	free(p);

	return(OK);
}
status get_top(T* queue, elem_t* elem)
{
	assert(queue);

	if (!queue_empty(queue))
		return(FALSE);

	*elem = queue->front->data;

	return(OK);
}

status destroy_queue(T* queue)
{
	assert(queue);
	
	NODE* p;

	while (!queue_empty(queue) )
	{
		p = queue->front;
		queue->front = p->next;
		free(p);
	}
		
	return(OK);	
}







