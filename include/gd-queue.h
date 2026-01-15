#ifndef GD_QUEUE_H
#define GD_QUEUE_H

#include <stddef.h>

#ifndef GD_Q_NODE_T
#define GD_Q_NODE_T
typedef int gd_q_nodeT;
#endif // GD_Q_NODE_T

struct gd_q {
    gd_q_nodeT *buffer;
    int capacity;
    int front;
    int rear; // next available empty position
    int length;
};

/**
 * @brief Initializes an empty queue
 * @return
 *  0 on success
 *  -1 on failure
 * @param q Pointer to queue
 * @param buffer Pointer to allocated queue memory
 * @param size Size of buffer
 */
int gd_q_init(struct gd_q *q, gd_q_nodeT *buffer, size_t size);

/**
 * @brief Adds one element to rear of queue
 * @return
 *  0 on success
 *  -1 if queue is full
 *  Other negative value on failure
 * @param q Pointer to queue
 * @param val Int to add to queue
 */
int gd_q_enqueue(struct gd_q *q, gd_q_nodeT val);

/**
 * @brief Removes int from front of queue and writes it to int pointer
 * @return
 *  0 on success
 *  -1 if queue is empty
 *  Other negative value on failure
 * @param q Pointer to queue
 * @param val Pointer to int; will not be changed when return value is negative
 */
int gd_q_dequeue(struct gd_q *q, gd_q_nodeT *val);

/**
 * @brief Copies queue contents into new buffer
 * @return
 *  0 on success
 *  -1 if new buffer is too small for queue
 *  -2 if new buffer overlaps with old buffer
 *  Other negative value on failure
 * @param q Pointer to queue
 * @param new_buffer Pointer to newly allocated memory
 * @param new_size Size of newly allocated buffer
 * @warning Not for use with overlapping buffers
 */
int gd_q_resize(struct gd_q *q, gd_q_nodeT *new_buffer, size_t new_size);

#endif // GD_QUEUE_H
