/*
 * title: queue.h
 * content: test datastructure queue;
 * date: 25 Oct, 2011
 */

#ifndef QUEUE
#define QUEUE

#define elem_t char

#define T queue_t
typedef struct T T;

/* define the status */
#define status int
#define OK 1
#define ERROR -1 
#define TRUE 1
#define FALSE 0

T* init_queue();
status destroy_queue(T* queue);
status queue_empty(T* queue);
status empty_queue(T* queue);
status enqueue(T*queue, elem_t elem);
status dequeue(T*queue, elem_t *elem);
status get_top(T*queue, elem_t *elem);
status free_queue(T *queue);

#undef T

#endif
