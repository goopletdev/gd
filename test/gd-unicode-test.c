#include "gd-unicode.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

void gdUnicodeTest_highLeadingBits_shouldCountLeadingBits(void) {
    TEST_ASSERT_EQUAL(1, utf8_high_leading_bits(0x80));
    TEST_ASSERT_EQUAL(1, utf8_high_leading_bits(0xB0));
    TEST_ASSERT_EQUAL(2, utf8_high_leading_bits(0xC0));
}

void gdUnicodeTest_codepointFromStr_shouldNotMutateAscii(void) {
    utf8_codepoint c;
    utf8_codepoint_from_str("a", sizeof("a"), 0, &c);
    TEST_ASSERT_EQUAL('a', c);
}

void gdUnicodeTest_codepointFromStr_shouldGetValidCodepoint(void) {
    utf8_codepoint c;
    utf8_codepoint_from_str("忑", sizeof("忑"), 0, &c);
    TEST_ASSERT_EQUAL(0x5FD1, c);

    utf8_codepoint_from_str("𐌘", sizeof("𐌘"), 0, &c);
    TEST_ASSERT_EQUAL(0x10318, c);
}

void gdUnicodeTest_strFromCodepoint_shouldEncodeCodepoint(void) {
    char buffer[16] = { '\0' };
    int err = utf8_str_from_codepoint(buffer, sizeof(buffer), 0, 0x10318);
    TEST_ASSERT_EQUAL(4, err);
    TEST_ASSERT_EQUAL_STRING("𐌘", buffer);

    memset(buffer, 0, sizeof(buffer));
    err = utf8_str_from_codepoint(buffer, sizeof(buffer), 0, 0xFA);
    TEST_ASSERT_EQUAL(2, err);
    TEST_ASSERT_EQUAL_STRING("ú", buffer);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(gdUnicodeTest_highLeadingBits_shouldCountLeadingBits);
    RUN_TEST(gdUnicodeTest_codepointFromStr_shouldNotMutateAscii);
    RUN_TEST(gdUnicodeTest_codepointFromStr_shouldGetValidCodepoint);
    RUN_TEST(gdUnicodeTest_strFromCodepoint_shouldEncodeCodepoint);

    return UNITY_END();
}
