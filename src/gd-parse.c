#include "gd-parse.h"

char gd_get_escaped_ascii(char c) {
    #define ESC_ASCII(c, e) case c: return e
    switch (c) {
        ESC_ASCII('a', '\a');
        ESC_ASCII('b', '\b');
        ESC_ASCII('t', '\t');
        ESC_ASCII('n', '\n');
        ESC_ASCII('v', '\v');
        ESC_ASCII('f', '\f');
        ESC_ASCII('r', '\r');
        default: return c;
    }
    /**
    switch (c) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 't':
            return '\t';
        case 'n':
            return '\n';
        case 'v':
            return '\v';
        case 'f':
            return '\f';
        case 'r':
            return '\r';
        default:
            return c;
    };
    */
}

int gd_parse_csv_headers(const char *headers_line, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    return 1;
}

int gd_parse_csv(const char* file_path, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    return 1;
}

