#ifndef GD_PARSE_H
#define GD_PARSE_H

#include <stdio.h>

#include "gd-arena.h"

#ifndef GD_PARSE_MAX_LINE_WIDTH
#define GD_PARSE_MAX_LINE_WIDTH 2048
#endif // GD_PARSE_MAX_LINE_WIDTH

#ifndef GD_PARSE_MAX_CSV_COLUMNS
#define GD_PARSE_MAX_CSV_COLUMNS 64
#endif // GD_PARSE_MAX_CSV_COLUMNS

#ifndef GD_PARSE_CSV_DELIMITER
#define GD_PARSE_CSV_DELIMITER ','
#endif // GD_PARSE_CSV_DELIMITER

struct gd_csv {
    int cols;
    int rows; // inclusive of header line
    struct gd_string *entries;

    struct gd_arena *char_arena;
    struct gd_arena *str_arena;
};

char gd_get_escaped_ascii(char c);

/**
 * writes columns from headers_line into char_arena and str_arena
 * the return value should be used to set gd_csv.cols
 * gd_csv.entries should be set to str_arena.buffer
 * @param headers_line null-terminated or newline-terminated string
 * @return number of columns, which is also number of gd_string written to str_arena
 *  0: empty string
 *  negative value: failure
 */
int gd_parse_csv_line(const char *line, struct gd_arena *char_arena, struct gd_arena *str_arena);

/**
 * parses .csv file into *csv and its associated arenas
 * @return
 *  0: success
 *  negative value: failure
 */
int gd_parse_csv(const char* file_path, struct gd_csv *csv, struct gd_arena *char_arena, struct gd_arena *str_arena);

int get_parse_csv_get_row_col(struct gd_csv *csv, int row, int col);

#endif // GD_PARSE_H
