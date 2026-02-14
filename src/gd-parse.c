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

int gd_parse_csv_line(const char *line, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    int err;
    int escaped = 0;
    int cols = 0;
    char c;
    struct gd_string s;
    for (int i = 0;; i++) {
        c = line[i];
        if (escaped) {
            escaped = 0;
            if ((err = gd_arena_appendc(char_arena, gd_get_escaped_ascii(c)))) return err;
        } else if (c == '\\') {
            escaped = 1;
        } else if (c == GD_PARSE_CSV_DELIMITER || c == '\n' || c == '\0') {
            cols++;
            if ((err = gd_arena_read_last_string(char_arena, &s))) return err;
            if ((err = gd_arena_alloc(char_arena, 0))) return err;
            if ((err = gd_arena_appends(str_arena, &s, sizeof(s)))) return err;
            if (c == '\n' || c == '\0') break;
        } else {
            if ((err = gd_arena_appendc(char_arena, c))) return err;
        }
    }

    return cols;
}

int gd_parse_csv(const char* file_path, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena) {
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return -1;
    }

    char line_buffer[GD_PARSE_MAX_LINE_WIDTH];

    // parse header line for number columns
    if (!fgets(line_buffer, GD_PARSE_MAX_LINE_WIDTH, fp)) {
        return -2; // zero lines
    }
    int cols = gd_parse_csv_line(line_buffer, char_arena, str_arena);
    if (cols < 1) {
        if (cols == 0) return -3;
        return -4;
    }
    csv->cols = cols;
    csv->rows = 1;
    csv->entries = (struct gd_string*)str_arena->buffer;

    while (fgets(line_buffer, GD_PARSE_MAX_LINE_WIDTH, fp)) {
        cols = gd_parse_csv_line(line_buffer, char_arena, str_arena);
        if (cols != csv->cols) {
            if (cols > 0) return cols;
            return cols - 5;
        }
        csv->rows++;
    }

    return 0;
}

