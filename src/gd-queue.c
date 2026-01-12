#include "gd-queue.h"

int gd_q_init(struct gd_q *q, gd_q_nodeT *buffer, int size) {
    if (size < 1) {
        return -1;
    }
    q->buffer = buffer;
    q->capacity = size;
    q->front = 0;
    q->rear = 0;
    q->length = 0;
    return 0;
}

int gd_q_enqueue(struct gd_q *q, gd_q_nodeT val) {
    if (q->length == q->capacity) {
        return -1;
    }

    q->buffer[q->rear] = val;
    q->length++;
    q->rear = (q->rear + 1) % q->capacity;
    return 0;
}

int gd_q_dequeue(struct gd_q *q, gd_q_nodeT *val) {
    if (q->length == 0) {
        return -1;
    }

    *val = q->buffer[q->front];
    q->length--;
    q->front = (q->front + 1) % q->capacity;
    return 0;
}

int gd_q_resize(struct gd_q *q, gd_q_nodeT *new_buffer, int new_capacity) {
    if (new_capacity < q->length) {
        return -1;
    }

    if (((new_buffer > q->buffer) 
            && (new_buffer < (q->buffer + q->capacity)))
                || ((q->buffer > new_buffer) 
                    && (q->buffer < (new_buffer + new_capacity)))) {
        return -2;
    }

    struct gd_q new_q;
    int err = gd_q_init(&new_q, new_buffer, new_capacity);

    if (err < 0) {
        return err;
    }

    for (int i = q->front; i != q->rear; i = (i + 1) % q->capacity) {
        new_buffer[new_q.rear++] = q->buffer[i];
    }

    new_q.length = q->length;
    *q = new_q;
    return 0;
}

