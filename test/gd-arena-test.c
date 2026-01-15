#include "gd-arena.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void gdArena_new_shouldInitArenaWithErrValue(void) {
    uint8_t buffer[16];
    struct gd_arena arena = gd_arena_new(buffer, 0);
    TEST_ASSERT_EQUAL(NULL, arena.buffer);
    TEST_ASSERT_EQUAL(0, arena.size);
    TEST_ASSERT_EQUAL(GDERR_A_INAPPROPRIATE_BUFFER_SIZE, arena.err);
    arena = gd_arena_new(buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_PTR(buffer, arena.buffer);
    TEST_ASSERT_EQUAL(16, arena.size);
    TEST_ASSERT_EQUAL(GDERR_A_OK, arena.err);
    TEST_ASSERT_EQUAL(0, arena.next);
}

void gdArena_malloc_shouldReturnPointers(void) {
    uint8_t buffer[16];
    struct gd_arena arena = gd_arena_new(buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_PTR(buffer, arena.buffer);
    TEST_ASSERT_EQUAL(0, arena.next);

    void *ptr = gd_arena_malloc(&arena, 5);
    TEST_ASSERT_EQUAL_PTR(arena.buffer, ptr);
    TEST_ASSERT_EQUAL(5, arena.next);

    ptr = gd_arena_malloc(&arena, 5);
    TEST_ASSERT_EQUAL_PTR(buffer + 5, ptr);
    TEST_ASSERT_EQUAL(10, arena.next);

    ptr = gd_arena_malloc(&arena, 5);
    TEST_ASSERT_EQUAL_PTR(buffer + 10, ptr);
    TEST_ASSERT_EQUAL(15, arena.next);

    TEST_ASSERT_EQUAL(GDERR_A_OK, arena.err);

    ptr = gd_arena_malloc(&arena, 7);
    TEST_ASSERT_NULL(ptr);
    TEST_ASSERT_EQUAL(15, arena.next);

    TEST_ASSERT_EQUAL(GDERR_A_INSUFFICIENT_SPACE, arena.err);
}

void gdArena_str_shouldReturnStructs(void) {
    uint8_t buffer[16];
    struct gd_arena arena = gd_arena_new(buffer, sizeof(buffer));
    TEST_ASSERT_EQUAL_PTR(buffer, arena.buffer);
    TEST_ASSERT_EQUAL(0, arena.next);

    char str1[] = "UTEST";
    char str2[] = "ltest";
    char str3[] = "mtEsT";
    char str4[] = "failtest";

    struct gd_string s1 = gd_arena_str(&arena, str1, sizeof(str1) - 1);
    struct gd_string s2 = gd_arena_str(&arena, str2, sizeof(str2) - 1);
    struct gd_string s3 = gd_arena_str(&arena, str3, sizeof(str3) - 1);
    struct gd_string s4 = gd_arena_str(&arena, str4, sizeof(str4) - 1);

    TEST_ASSERT_NOT_NULL(s1.str);
    TEST_ASSERT_EQUAL_MEMORY(str1, s1.str, s1.length);
    TEST_ASSERT_NOT_NULL(s2.str);
    TEST_ASSERT_EQUAL_MEMORY(str2, s2.str, s2.length);
    TEST_ASSERT_NOT_NULL(s3.str);
    TEST_ASSERT_EQUAL_MEMORY(str3, s3.str, s3.length);
    TEST_ASSERT_NULL(s4.str);
    TEST_ASSERT_EQUAL(0, s4.length);

    TEST_ASSERT_EQUAL(15, arena.next);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(gdArena_new_shouldInitArenaWithErrValue);
    RUN_TEST(gdArena_malloc_shouldReturnPointers);
    RUN_TEST(gdArena_str_shouldReturnStructs);

    return UNITY_END();
}
