#ifndef GD_PARSE_H
#define GD_PARSE_H

#include "gd-arena.h"

#ifndef GD_PARSE_MAX_LINE_WIDTH
#define GD_PARSE_MAX_LINE_WIDTH 2048
#endif // GD_PARSE_MAX_LINE_WIDTH

#ifndef GD_PARSE_MAX_CSV_COLUMNS
#define GD_PARSE_MAX_CSV_COLUMNS 64
#endif // GD_PARSE_MAX_CSV_COLUMNS

struct gd_csv {
    size_t cols;
    size_t rows;
    struct gd_string *headers;
    struct gd_string *entries;
};

char gd_get_escaped_ascii(char c);

/**
 * Writes each header into csv->headers, 
 * allocating memory for strings and pointers to arena
 */
int gd_parse_csv_headers(const char *headers_line, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena);

int gd_parse_csv(const char* file_path, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena);

#endif // GD_PARSE_H
