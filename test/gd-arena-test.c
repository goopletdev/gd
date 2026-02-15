#include "gd-arena.h"
#include "unity.h"

void setUp(void) {
}

void tearDown(void) {
}

void gdArena_new_shouldInitArenaPointers(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    struct gd_arena arena_copy = arena;
    int err = gd_arena_new(&arena, buffer, 0);

    TEST_ASSERT_EQUAL(-1, err);
    TEST_ASSERT_EQUAL_PTR(arena.buffer, arena_copy.buffer);
    TEST_ASSERT_EQUAL(arena.max, arena_copy.max);
    TEST_ASSERT_EQUAL(arena.current, arena_copy.current);
    TEST_ASSERT_EQUAL(arena.next, arena_copy.next);

    err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_FALSE(arena.buffer == arena_copy.buffer);
    TEST_ASSERT_FALSE(arena.max == arena_copy.max);

    TEST_ASSERT_EQUAL(15, arena.max);
    TEST_ASSERT_EQUAL_PTR(arena.buffer, arena.buffer + arena.current);
    TEST_ASSERT_EQUAL_PTR(arena.buffer, arena.buffer + arena.next);

}

void gdArena_alloc_shouldReturnNegativeIfFull(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    err = gd_arena_alloc(&arena, 10);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_PTR(buffer, arena.buffer);
    TEST_ASSERT_EQUAL(15, arena.max);
    TEST_ASSERT_EQUAL(0, arena.current);
    TEST_ASSERT_EQUAL(10, arena.next);

    err = gd_arena_alloc(&arena, 2);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(10, arena.current);
    TEST_ASSERT_EQUAL(12, arena.next);

    err = gd_arena_alloc(&arena, 5);

    TEST_ASSERT_EQUAL(-1, err);
    TEST_ASSERT_EQUAL(10, arena.current);
    TEST_ASSERT_EQUAL(12, arena.next);

    err = gd_arena_alloc(&arena, 4);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(12, arena.current);
    TEST_ASSERT_EQUAL(16, arena.next);
    TEST_ASSERT_EQUAL(arena.next, arena.size);
    TEST_ASSERT_EQUAL(arena.next, arena.size);

    err = gd_arena_alloc(&arena, 4);

    TEST_ASSERT_EQUAL(-2, err);

    struct gd_pointer p;
    err = gd_arena_read_last_pointer(&arena, &p);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(4, p.size);
}

void gdArena_reallocCurrent_shouldReturnNegativeIfInsufficientSpace(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    err = gd_arena_realloc_current(&arena, 5);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(5, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current); 

    err = gd_arena_realloc_current(&arena, 3);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(3, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current); 

    err = gd_arena_realloc_current(&arena, 25);

    TEST_ASSERT_EQUAL(-1, err);
    TEST_ASSERT_EQUAL(3, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current); 

    err = gd_arena_realloc_current(&arena, 8);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(8, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current); 

    err = gd_arena_alloc(&arena, 0);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(8, arena.current);
    TEST_ASSERT_EQUAL(8, arena.next);

    err = gd_arena_realloc_current(&arena, 2);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(8, arena.current);
    TEST_ASSERT_EQUAL(10, arena.next);
}

void gdArena_appendc_shouldIncrementNextPointer(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    err = gd_arena_appendc(&arena, 'h');

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL('h', arena.buffer[0]);
    TEST_ASSERT_EQUAL(1, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current);

    err = gd_arena_appendc(&arena, 'e');

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL('e', arena.buffer[1]);
    TEST_ASSERT_EQUAL(2, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current);

    err = gd_arena_alloc(&arena, 13);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(15, arena.next);
    TEST_ASSERT_EQUAL(arena.max, arena.next);
    TEST_ASSERT_EQUAL(2, arena.current);

    err = gd_arena_appendc(&arena, 'l');

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(arena.size, arena.next);
    TEST_ASSERT_EQUAL(16, arena.next);
    TEST_ASSERT_EQUAL(2, arena.current);

    err = gd_arena_appendc(&arena, 'p');

    TEST_ASSERT_EQUAL(-2, err);
    TEST_ASSERT_EQUAL(arena.size, arena.next);
    TEST_ASSERT_EQUAL(16, arena.next);
    TEST_ASSERT_EQUAL(2, arena.current);

    struct gd_pointer p;
    err = gd_arena_read_last_pointer(&arena, &p);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(14, p.size);
}

void gdArena_appends_shouldCopyBufferContents(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    char str1[] = "test1";
    char str2[] = "test2";

    err = gd_arena_appends(&arena, str1, 5);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(5, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current);

    err = gd_arena_appends(&arena, str2, 5);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(10, arena.next);
    TEST_ASSERT_EQUAL(0, arena.current);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("test1test2", arena.buffer, 10);

    err = gd_arena_alloc(&arena, 0);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(10, arena.current);
    TEST_ASSERT_EQUAL(10, arena.next);

    char str3[] = "LONGTESTUP";
    char str4[] = "UPPER";

    err = gd_arena_appends(&arena, str3, 10);
    TEST_ASSERT_EQUAL(-1, err);
    TEST_ASSERT_EQUAL(10, arena.current);
    TEST_ASSERT_EQUAL(10, arena.next);

    err = gd_arena_appends(&arena, str4, 5);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(10, arena.current);
    TEST_ASSERT_EQUAL(15, arena.next);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("test1test2UPPER", buffer, 15);
}

void gdArena_readLastPointer_shouldReadAnyDataTypeIntoGdPointerStruct(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    struct gd_pointer p;
    err = gd_arena_read_last_pointer(&arena, &p);

    TEST_ASSERT_EQUAL(-1, err);

    err = gd_arena_alloc(&arena, 4);
    TEST_ASSERT_EQUAL(0, err);

    err = gd_arena_read_last_pointer(&arena, &p);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(4, p.size);
    TEST_ASSERT_EQUAL_PTR(p.buffer, buffer);

    err = gd_arena_alloc(&arena, 0);
    TEST_ASSERT_EQUAL(0, err);

    int nums[2] = { 89, 64 };
    err = gd_arena_appends(&arena, nums, sizeof(nums));

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(4, arena.current);
    TEST_ASSERT_EQUAL(12, arena.next);

    err = gd_arena_read_last_pointer(&arena, &p);

    TEST_ASSERT_EQUAL(p.size, arena.next - arena.current);
    TEST_ASSERT_EQUAL(buffer + 4, p.buffer);
    TEST_ASSERT_EQUAL_INT_ARRAY(arena.buffer + arena.current, p.buffer, 2);
}

void gdArena_readLastString_shouldReadCharsIntoGdStringStruct(void) {
    uint8_t buffer[16];
    struct gd_arena arena;
    int err = gd_arena_new(&arena, buffer, sizeof(buffer));

    TEST_ASSERT_EQUAL(0, err);

    char str1[] = "testedytest";
    err = gd_arena_appends(&arena, str1, sizeof(str1) - 1);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(buffer, str1, sizeof(str1) - 1);

    struct gd_string str;
    err = gd_arena_read_last_string(&arena, &str);

    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(11, str.length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(str.str, buffer, str.length);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(gdArena_new_shouldInitArenaPointers);
    RUN_TEST(gdArena_alloc_shouldReturnNegativeIfFull);
    RUN_TEST(gdArena_reallocCurrent_shouldReturnNegativeIfInsufficientSpace);
    RUN_TEST(gdArena_appendc_shouldIncrementNextPointer);
    RUN_TEST(gdArena_appends_shouldCopyBufferContents);
    RUN_TEST(gdArena_readLastPointer_shouldReadAnyDataTypeIntoGdPointerStruct);
    RUN_TEST(gdArena_readLastString_shouldReadCharsIntoGdStringStruct);

    return UNITY_END();
}
