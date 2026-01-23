#include "gd-parse.h"
#include "unity.h"

struct gd_arena char_arena;
struct gd_arena str_arena;

uint8_t char_buffer[2048];
uint8_t str_buffer[2048];

void setUp(void) {
    gd_arena_new(&char_arena, char_buffer, sizeof(char_buffer));
    gd_arena_new(&str_arena, str_buffer, sizeof(str_buffer));
}
void tearDown(void) {}

void gdParse_escapedAscii_shouldReturnUnescapedAscii(void) {
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('a'), '\a');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('b'), '\b');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('t'), '\t');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('n'), '\n');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('v'), '\v');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('f'), '\f');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('r'), '\r');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('\0'), '\0');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('N'), 'N');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('s'), 's');
    TEST_ASSERT_EQUAL(gd_get_escaped_ascii('-'), '-');
}

void gdParse_csvHeaders_shouldGetNumColumns(void) {
    struct gd_csv csv;
    int err = gd_parse_csv_headers("alpha,bravo,charlie,delta,echo", &csv, &char_arena, &str_arena);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(5, csv.cols);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("alpha", csv.headers[0].str, csv.headers[0].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("bravo", csv.headers[1].str, csv.headers[1].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("charlie", csv.headers[2].str, csv.headers[2].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("delta", csv.headers[3].str, csv.headers[3].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("echo", csv.headers[4].str, csv.headers[4].length);
}

void gdParse_csv_shouldParseCsv(void) {
    TEST_IGNORE_MESSAGE("WRITE CSV TEST");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(gdParse_escapedAscii_shouldReturnUnescapedAscii);
    RUN_TEST(gdParse_csvHeaders_shouldGetNumColumns);
    RUN_TEST(gdParse_csv_shouldParseCsv);

    return UNITY_END();
}
