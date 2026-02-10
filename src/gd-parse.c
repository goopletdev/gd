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
}

int gd_parse_csv_headers(const char *headers_line, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    int err;
    int escaped = 0;
    int cols = 0;
    char c;
    for (int i = 0;; i++) {
        c = headers_line[i];
        if (escaped) {
            escaped = 0;
            if (err = gd_arena_appendc(char_arena, gd_get_escaped_ascii(c))) return err;
        } else if (c == '\\') {
            escaped = 1;
        } else if (c == GD_PARSE_CSV_DELIMITER || c == '\n' || c == '\0') {
            cols++;
            if (err = gd_arena_read_last_string(char_arena, &s)) return err;
            if (err = gd_arena_alloc(char_arena, 0)) return err;
            if (err = gd_arena_appends(str_arena, &s, sizeof(s))) return err;
            if (c == '\n' || c == '\0') break;
        } else {
            if (err = gd_arena_appendc(char_arena, c)) return err;
        }
    }
    struct gd_string p;
    if (err = gd_arena_read_last_pointer(str_arena, &p)) return err;
    csv->cols = cols;
    if (err = gd_arena_appends(entries_arena, &p, sizeof(p))) return err;
    csv->entries = (struct gd_pointer*)entries_arena->buffer;

    return 0;
}

int gd_parse_csv_line(const char *line, struct gd_csv *csv) {
    return 1;
}

int gd_parse_csv(const char* file_path, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    return 1;
}

