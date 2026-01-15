#ifndef GD_ARENA_H
#define GD_ARENA_H

#include <stddef.h>
#include <stdint.h>

enum gd_arena_err {
    GDERR_A_OK,
    GDERR_A_INSUFFICIENT_SPACE,
    GDERR_A_INAPPROPRIATE_BUFFER_SIZE,
};

struct gd_string {
    char *str;
    int length;
};

struct gd_arena {
    uint8_t *buffer;
    size_t size;
    size_t next;
    enum gd_arena_err err;
};

/**
 * @return Pointer if allocated, NULL if insufficient space
 */
void* gd_arena_malloc(struct gd_arena *arena, size_t size);

/**
 * Sets err to GDERR_A_OK on success
 */
struct gd_arena gd_arena_new(uint8_t *buffer, size_t size);

struct gd_string gd_arena_str(struct gd_arena *arena, char *str, size_t len);

#endif // GD_ARENA_H
