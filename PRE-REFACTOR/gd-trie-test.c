#include "gd-trie.h"
#include "unity.h"

gd_trie* root;

void setUp(void) {
    root = gd_trie_init();
    gd_trie_insert_cs(root, "apple", "fruit");
    gd_trie_insert_cs(root, "tree", "forest is made of ____");
    gd_trie_insert_cs(root, "house", "she lives in a _____");
    gd_trie_insert_cs(root, "apparent", "it's ________ you're a parent when...");
    gd_trie_insert_cs(root, "hOuSE", "HoUse");
    gd_trie_insert_cs(root, "HOUSE", "house");
    gd_trie_insert_cs(root, "it's ____ to see you AGAIN!", "nice");

    gd_trie_insert_ci(root, "capstest_1", "ct1 result 1");
    gd_trie_insert_ci(root, "capstest_2", "ct2 result 2");
}

void tearDown(void) {
    gd_trie_free(root);
}

void gdTrieGetCS_should_getCaseSensitiveKeyValues(void) {
    TEST_ASSERT_EQUAL_STRING("fruit", gd_trie_get_cs(root, "apple"));
    TEST_ASSERT_EQUAL_STRING("forest is made of ____", gd_trie_get_cs(root, "tree"));
    TEST_ASSERT_EQUAL_STRING("HoUse", gd_trie_get_cs(root, "hOuSE"));
    TEST_ASSERT_NULL(gd_trie_get_cs(root, "TREE"));
    TEST_ASSERT_NULL(gd_trie_get_cs(root, "House"));
    TEST_ASSERT_NULL(gd_trie_get_cs(root, "it's ____ to see you again!"));
    TEST_ASSERT_EQUAL_STRING("nice", gd_trie_get_cs(root, "it's ____ to see you AGAIN!"));

    TEST_ASSERT_NULL(gd_trie_get_cs(root, "capstest_1"));
    TEST_ASSERT_NOT_NULL(gd_trie_get_cs(root, "CAPSTEST_1"));
}

void gdTrieGetCIFromCS_should_getCaseInsensitiveKeyValues(void) {
    TEST_ASSERT_EQUAL_STRING("fruit", gd_trie_get_ci_from_cs(root, "apple"));
    TEST_ASSERT_EQUAL_STRING("fruit", gd_trie_get_ci_from_cs(root, "Apple"));
    TEST_ASSERT_EQUAL_STRING("fruit", gd_trie_get_ci_from_cs(root, "APPLE"));
    TEST_ASSERT_EQUAL_STRING("it's ________ you're a parent when...", gd_trie_get_ci_from_cs(root, "ApPaReNT"));
    TEST_ASSERT_EQUAL_STRING("nice", gd_trie_get_ci_from_cs(root, "it's ____ to see you again!"));

    TEST_ASSERT_EQUAL_STRING("ct1 result 1", gd_trie_get_ci_from_cs(root, "capstest_1"));
}

void gdTrieGetCI_should_getCaseInsensitiveKeyValuesFromCapsInsertion(void) {
    TEST_ASSERT_EQUAL_STRING("ct1 result 1", gd_trie_get_ci(root, "capstest_1"));
    TEST_ASSERT_EQUAL_STRING("ct2 result 2", gd_trie_get_ci(root, "capstest_2"));
    TEST_ASSERT_EQUAL_STRING("ct1 result 1", gd_trie_get_ci(root, "CAPSTEST_1"));
    TEST_ASSERT_EQUAL_STRING("ct2 result 2", gd_trie_get_ci(root, "CAPsTeSt_2"));

    TEST_ASSERT_NULL(gd_trie_get_ci(root, "apple"));
}
    

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(gdTrieGetCS_should_getCaseSensitiveKeyValues);
    RUN_TEST(gdTrieGetCIFromCS_should_getCaseInsensitiveKeyValues);
    RUN_TEST(gdTrieGetCI_should_getCaseInsensitiveKeyValuesFromCapsInsertion);
    return UNITY_END();
}

