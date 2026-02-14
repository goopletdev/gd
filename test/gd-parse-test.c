#include "gd-parse.h"
#include "unity.h"

const char fpath[] = "test/test-data/CSV1.csv";

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

void gdParse_csvLine_shouldGetNumColumns(void) {
    struct gd_csv csv;
    
    int cols = gd_parse_csv_line("alpha,bravo,charlie,delta,echo", &char_arena, &str_arena);
    TEST_ASSERT_EQUAL(5, cols);
    csv.cols = cols;
    csv.entries = (struct gd_string*)str_arena.buffer;
    TEST_ASSERT_EQUAL(5, csv.cols);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("alpha", csv.entries[0].str, csv.entries[0].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("bravo", csv.entries[1].str, csv.entries[1].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("charlie", csv.entries[2].str, csv.entries[2].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("delta", csv.entries[3].str, csv.entries[3].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("echo", csv.entries[4].str, csv.entries[4].length);
}

void gdParse_csv_shouldParseCsv(void) {
    struct gd_csv csv;
    int err = gd_parse_csv(fpath, &csv, &char_arena, &str_arena);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL_PTR(str_arena.buffer, csv.entries);
    TEST_ASSERT_EQUAL(6, csv.cols);
    TEST_ASSERT_EQUAL(3, csv.rows);

    TEST_ASSERT_EQUAL_CHAR_ARRAY("alpha", csv.entries[0].str, csv.entries[0].length);
    TEST_ASSERT_EQUAL_CHAR_ARRAY("foxtrot", csv.entries[5].str, csv.entries[5].length);

    int line = 1, col = 0;
    int pos = (line * csv.cols) + col;
    TEST_ASSERT_EQUAL_CHAR_ARRAY("line1", csv.entries[pos].str, csv.entries[pos].length);

    line = 2, col = 1;
    pos = (line * csv.cols) + col;
    TEST_ASSERT_EQUAL_CHAR_ARRAY("column 2 (index 1)", csv.entries[pos].str, csv.entries[pos].length);

    pos++;
    TEST_ASSERT_NULL(csv.entries[pos].str);

    pos++;
    TEST_ASSERT_EQUAL_CHAR_ARRAY("previous was blank", csv.entries[pos].str, csv.entries[pos].length);
}

void gdParse_csvGetRowCol_shouldGetEntryCol(void) {
    TEST_IGNORE_MESSAGE("write csvGetRowCol test");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(gdParse_escapedAscii_shouldReturnUnescapedAscii);
    RUN_TEST(gdParse_csvLine_shouldGetNumColumns);
    RUN_TEST(gdParse_csv_shouldParseCsv);
    RUN_TEST(gdParse_csvGetRowCol_shouldGetEntryCol);

    return UNITY_END();
}
