#include "gd-queue.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void gdQueueTest_gdQInit_shouldInitializeEmptyQueue(void) {
    struct gd_q q;
    int buffer[16];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(16, q.capacity);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(0, q.rear);
    TEST_ASSERT_EQUAL(0, q.length);
}

void gdQueueTest_gdQEnqueue_shouldAddElement(void) {
    struct gd_q q;
    int buffer[16];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);

    err = gd_q_enqueue(&q, 2);
    TEST_ASSERT_EQUAL(0, err);
    err = gd_q_enqueue(&q, 3);
    TEST_ASSERT_EQUAL(0, err);
    err = gd_q_enqueue(&q, 5);
    TEST_ASSERT_EQUAL(0, err);
    err = gd_q_enqueue(&q, 7);
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(2, q.buffer[0]);
    TEST_ASSERT_EQUAL(3, q.buffer[1]);
    TEST_ASSERT_EQUAL(5, q.buffer[2]);
    TEST_ASSERT_EQUAL(7, q.buffer[3]);

    TEST_ASSERT_EQUAL(4, q.length);
    TEST_ASSERT_EQUAL(4, q.rear);
    TEST_ASSERT_EQUAL(0, q.front);
}

void gdQueueTest_gdQEnqueue_shouldReturnNegativeWhenFull(void) {
    struct gd_q q;
    int buffer[4];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);

    err = gd_q_enqueue(&q, 2);
    err = gd_q_enqueue(&q, 3);
    err = gd_q_enqueue(&q, 5);
    err = gd_q_enqueue(&q, 7);
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(0, q.rear);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(4, q.length);
    TEST_ASSERT_EQUAL(4, q.capacity);
    err = gd_q_enqueue(&q, 11);
    TEST_ASSERT_EQUAL(-1, err);
    TEST_ASSERT_EQUAL(0, q.rear);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(4, q.length);
    TEST_ASSERT_EQUAL(4, q.capacity);
}

void gdQueueTest_gdQDequeue_shouldRemoveAndSetValue(void) {
    struct gd_q q;
    int buffer[8];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);

    err = gd_q_enqueue(&q, 2);
    err = gd_q_enqueue(&q, 3);
    err = gd_q_enqueue(&q, 5);
    err = gd_q_enqueue(&q, 7);
    TEST_ASSERT_EQUAL(0, err);

    TEST_ASSERT_EQUAL(4, q.rear);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(4, q.length);
    TEST_ASSERT_EQUAL(8, q.capacity);

    int val;
    err = gd_q_dequeue(&q, &val);

    TEST_ASSERT_EQUAL(4, q.rear);
    TEST_ASSERT_EQUAL(1, q.front);
    TEST_ASSERT_EQUAL(3, q.length);
    TEST_ASSERT_EQUAL(8, q.capacity);
    TEST_ASSERT_EQUAL(2, val);
    TEST_ASSERT_EQUAL(0, err);

    err = gd_q_dequeue(&q, &val);

    TEST_ASSERT_EQUAL(4, q.rear);
    TEST_ASSERT_EQUAL(2, q.front);
    TEST_ASSERT_EQUAL(2, q.length);
    TEST_ASSERT_EQUAL(8, q.capacity);
    TEST_ASSERT_EQUAL(3, val);
    TEST_ASSERT_EQUAL(0, err);
}

void gdQueueTest_gdQBuffer_shouldWrapAround(void) {
    struct gd_q q;
    int buffer[4];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);

    int val;
    err = gd_q_enqueue(&q, 2);
    err = gd_q_enqueue(&q, 3);
    err = gd_q_enqueue(&q, 5);
    err = gd_q_dequeue(&q, &val); 
    err = gd_q_dequeue(&q, &val); 
    TEST_ASSERT_EQUAL(1, q.length);
    TEST_ASSERT_EQUAL(4, q.capacity);
    TEST_ASSERT_EQUAL(2, q.front);
    TEST_ASSERT_EQUAL(3, q.rear);
    TEST_ASSERT_EQUAL(3, val);

    err = gd_q_enqueue(&q, 7);
    TEST_ASSERT_EQUAL(0, q.rear);
    TEST_ASSERT_EQUAL(2, q.front);
    TEST_ASSERT_EQUAL(2, q.length);

    err = gd_q_enqueue(&q, 11);
    TEST_ASSERT_EQUAL(1, q.rear);
    TEST_ASSERT_EQUAL(2, q.front);
    TEST_ASSERT_EQUAL(3, q.length);

    err = gd_q_dequeue(&q, &val); 
    TEST_ASSERT_EQUAL(1, q.rear);
    TEST_ASSERT_EQUAL(3, q.front);
    TEST_ASSERT_EQUAL(2, q.length);
    TEST_ASSERT_EQUAL(5, val);

    err = gd_q_dequeue(&q, &val); 
    TEST_ASSERT_EQUAL(1, q.rear);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(1, q.length);
    TEST_ASSERT_EQUAL(7, val);
}

void gdQueueTest_gdQResize_shouldMaintainInsertionOrder(void) {
    struct gd_q q;
    int buffer[4];
    int err = gd_q_init(&q, buffer, sizeof(buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(0, err);

    err = gd_q_enqueue(&q, 2);
    err = gd_q_enqueue(&q, 3);
    err = gd_q_enqueue(&q, 5);
    TEST_ASSERT_EQUAL(0, err);

    int val;
    err = gd_q_dequeue(&q, &val);
    TEST_ASSERT_EQUAL(2, val);

    int new_buffer[8];
    err = gd_q_resize(&q, new_buffer, sizeof(new_buffer) / sizeof(int));
    TEST_ASSERT_EQUAL(3, new_buffer[0]);
    TEST_ASSERT_EQUAL(0, q.front);
    TEST_ASSERT_EQUAL(2, q.length);
    TEST_ASSERT_EQUAL(2, q.rear);
    TEST_ASSERT_EQUAL(8, q.capacity);
    err = gd_q_dequeue(&q, &val);
    TEST_ASSERT_EQUAL(3, val);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(gdQueueTest_gdQInit_shouldInitializeEmptyQueue);
    RUN_TEST(gdQueueTest_gdQEnqueue_shouldAddElement);
    RUN_TEST(gdQueueTest_gdQEnqueue_shouldReturnNegativeWhenFull);
    RUN_TEST(gdQueueTest_gdQDequeue_shouldRemoveAndSetValue);
    RUN_TEST(gdQueueTest_gdQBuffer_shouldWrapAround);
    RUN_TEST(gdQueueTest_gdQResize_shouldMaintainInsertionOrder);

    return UNITY_END();
}
