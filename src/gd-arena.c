#include "gd-arena.h"

void* gd_arena_malloc(struct gd_arena *a, size_t size) {
    if ((a->next + size) > a->size) {
        a->err = GDERR_A_INSUFFICIENT_SPACE;
        return NULL;
    }
    void* ptr = a->buffer + a->next;
    a->next += size;
    return ptr;
}

struct gd_arena gd_arena_new(uint8_t *buffer, size_t size) {
    struct gd_arena a = { buffer, size, 0, GDERR_A_OK };
    if (size < 1) {
        a.buffer = NULL;
        a.size = 0;
        a.err = GDERR_A_INAPPROPRIATE_BUFFER_SIZE;
    }
    return a;
}

struct gd_string gd_arena_str(struct gd_arena *a, char *s, size_t len) {
    struct gd_string gs;
    if ((gs.str = (char*)gd_arena_malloc(a, len)) == NULL) {
        gs.length = 0;
        return gs;
    }
    gs.length = len;
    for (int i = 0; i < len; i++) {
        gs.str[i] = s[i];
    }
    return gs;
}


