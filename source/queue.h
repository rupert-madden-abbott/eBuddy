/**
 * @file   queue.h
 * @author Rupert Madden-Abbott
 *
 * This module is a queue implementation with the following features:
 *   - Generic. The queue can be used to store any data structure
 *   - Thread Safe. The queue can be used in multi-threaded applications
 *   - POSIX compliant.
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

/**
 * @typedef qu_Node
 * Defines a node for the queue
 * 
 * @member data A void pointer to hold any sort of data on the queue
 * @member link A pointer to the next node in the queue
 */
typedef struct qu_node {
    void *data;
    struct qu_node *link;
} qu_Node;

/**
 * @typedef qu_Queue
 * Defines a linked list implementation of a queue
 *
 * @member mutex A mutex lock to make the queue thread safe
 * @member size A count of the number of nodes on the queue
 * @member head The start of the queue
 * @member tail The end of the queue
 */
typedef struct qu_queue {
    pthread_mutex_t mutex;
    int size;
    qu_Node *head;
    qu_Node *tail;
} qu_Queue;

/**
 * Initialises a new queue structure
 *
 * @warning The caller must free allocated resources by using qu_free
 *
 * @return Returns the queue object or NULL on error
 */
qu_Queue *qu_init(void);

/**
 * Destroys the queue itself and any nodes left on the queue.
 *
 * @param queue The queue to be destroyed
 * @return None
 */
void qu_free(qu_Queue *queue);

/**
 * Push new data onto the queue. Data is placed at the end of the queue.
 * 
 * @param queue A pointer to the queue for the data to be added to
 * @param data A pointer to the data to be added
 * @return UT_ERR_NONE on success, UT_ERR_UNKNOWN on failure
 */ 
int qu_push(qu_Queue *queue, void *data);

/**
 * Return the data from the next node on the queue. Nodes are ordered so that
 * the first node to be pushed onto the queue is the first node to be returned.
 * 
 * @param queue A pointer to the queue for the data to be returned from
 * @return The data from the next node or NULL on failure
 */
void *qu_pop(qu_Queue *queue);

/**
 * Provides a thread safe accessor to the size of the queue
 *
 * @param queue A pointer to the queue of which to check the size
 * @return 
 */
int qu_size(qu_Queue *queue);

#endif

