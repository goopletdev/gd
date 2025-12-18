#include "gd-ordered-map.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>

gd_omap* map;

void setUp(void) {
    map = NULL;
}

void tearDown(void) {
}

void gdOmapTest_add_shouldInsertAndAppendValue(void) {
    map = gd_omap_init(2);

    char* string1 = (char*)malloc(sizeof(char) * 6);
    char* string2 = (char*)malloc(sizeof(char) * 6);
    char* string3 = (char*)malloc(sizeof(char) * 5);
    strcpy(string1, "Hello");
    strcpy(string2, "world");
    strcpy(string3, "test");

    TEST_ASSERT_EQUAL(0, map->size);
    TEST_ASSERT_EQUAL(2, map->capacity);
    TEST_ASSERT_EQUAL(0, map->bucket_list->length);

    gd_omap_add(map, 1, (gd__omap_valueT)string1);
    TEST_ASSERT_EQUAL(1, map->size);
    TEST_ASSERT_EQUAL(2, map->capacity);
    TEST_ASSERT_EQUAL(1, map->bucket_list->length);

    gd_omap_add(map, 2, (gd__omap_valueT)string2);
    TEST_ASSERT_EQUAL(2, map->size);
    TEST_ASSERT_EQUAL(4, map->capacity);
    TEST_ASSERT_EQUAL(2, map->bucket_list->length);

    gd_omap_add(map, 3, (gd__omap_valueT)string3);
    TEST_ASSERT_EQUAL(3, map->size);
    TEST_ASSERT_EQUAL(3, map->bucket_list->length);
    TEST_ASSERT_EQUAL(8, map->capacity);

    gd_omap_free(map, free);
}

void gdOmapTest_get_shouldRetreiveValue(void) {
    map = gd_omap_init(8);

    char* string1 = (char*)malloc(sizeof(char) * 6);
    char* string2 = (char*)malloc(sizeof(char) * 6);
    char* string3 = (char*)malloc(sizeof(char) * 5);
    strcpy(string1, "Hello");
    strcpy(string2, "world");
    strcpy(string3, "test");

    gd_omap_add(map, 1, (gd__omap_valueT)string1);
    gd_omap_add(map, 2, (gd__omap_valueT)string2);
    gd_omap_add(map, 3, (gd__omap_valueT)string3);

    TEST_ASSERT_NULL((void*)gd_omap_get(map, 0));
    TEST_ASSERT_NULL((void*)gd_omap_get(map, 4));
    TEST_ASSERT_EQUAL_PTR(string1, (void*)gd_omap_get(map, 1));
    TEST_ASSERT_EQUAL_PTR(string2, (void*)gd_omap_get(map, 2));
    TEST_ASSERT_EQUAL_PTR(string3, (void*)gd_omap_get(map, 3));

    gd_omap_free(map, free);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(gdOmapTest_add_shouldInsertAndAppendValue);
    RUN_TEST(gdOmapTest_get_shouldRetreiveValue);

    return UNITY_END();
}
