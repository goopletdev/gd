#ifndef GD_ARENA_H
#define GD_ARENA_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct gd_pointer {
    size_t index; // previously pointer
    size_t size;
};

struct gd_string {
    char *str;
    size_t length;
};

struct gd_arena {
    uint8_t *buffer;
    size_t size;
    size_t max;
    size_t current;
    size_t next;
};

/**
 * sets arena pointers
 * @return
 *  0 on success
 *  -1 if insufficient size (less than 1)
 */
int gd_arena_new(struct gd_arena *a, uint8_t *buffer, size_t size);

/**
 * sets a->current to a->next, and increments a->next by size
 * notably, size can be 0, which should only be used with appendc and appends
 * @return 
 *  0 on success
 *  -1 if insufficient space in arena for new allocation
 *  -2 if arena is full
 */
int gd_arena_alloc(struct gd_arena *a, size_t size);

/**
 * resizes current allocation to new size
 * @return
 *  0 on success
 *  -1 if insufficient space in arena for resizing
 */
int gd_arena_realloc_current(struct gd_arena *a, size_t new_size);

/**
 * appends c to current allocation and increments a->next
 * @return
 *  0 on success
 *  -2 if arena is full; doesn't set value
 */
int gd_arena_appendc(struct gd_arena *a, uint8_t c);

/**
 * appends contents of buffer to last arena pointer
 * @return
 *  0 on success
 *  -1 if insufficient space in arena
 */
int gd_arena_appends(struct gd_arena *a, void *ptr, size_t s);

/** 
 * Reads a->current and (a->next - a->current) into p
 * @return
 *  0 on success
 *  -1 if pointer had size 0; does not write to p
 */
int gd_arena_read_last_pointer(struct gd_arena *a, struct gd_pointer *p); 

/**
 * reads a->current and (a->next - a->current) into s
 * can write 0-size strings as well, but will return a gd_str w/ null pointer
 * @return
 *  0 on success
 */
int gd_arena_read_last_string(struct gd_arena *a, struct gd_string *s);

#endif // GD_ARENA_H
