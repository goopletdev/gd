#include "gd-arena.h"

int gd_arena_new(struct gd_arena *a, uint8_t *buffer, size_t size) {
    if (size < 1) {
        return -1;
    }
    struct gd_arena arena = { 
        buffer, 
        buffer + size - 1, 
        buffer,
        buffer
    };
    *a = arena;
    return 0;
}

int gd_arena_alloc(struct gd_arena *a, size_t size) {
    struct gd_arena arena = *a;
    arena.current = arena.next;
    if (arena.current == (arena.max + 1)) {
        return -2;
    }
    arena.next += size;
    if (arena.next > (arena.max + 1)) {
        return -1;
    }
    *a = arena;
    return 0;
}

int gd_arena_realloc_current(struct gd_arena *a, size_t new_size) {
    struct gd_arena arena = *a;
    arena.next = arena.current + new_size;
    if (arena.next > (arena.max + 1)) {
        return -1;
    }
    *a = arena;
    return 0;
} 

int gd_arena_appendc(struct gd_arena *a, uint8_t c) {
    if (a->next > a->max) {
        return -2;
    }
    *(a->next++) = c;
    return 0;
}

int gd_arena_appends(struct gd_arena *a, void *ptr, size_t s) {
    if ((a->next + s) > (a->max + 1)) {
        return -1;
    }
    memcpy(a->next, ptr, s);
    a->next += s;
    return 0;
}

int gd_arena_read_last_pointer(struct gd_arena *a, struct gd_pointer *p) {
    size_t size = (size_t)(a->next - a->current);
    if (size == 0) {
        return -1;
    }
    p->buffer = a->current;
    p->size = size;
    return 0;
}

int gd_arena_read_last_string(struct gd_arena *a, struct gd_string *s) {
    size_t size = (size_t)(a->next - a->current);
    if (size == 0) {
        s->str = NULL;
        s->length = 0;
    } else {
        s->str = a->current;
        s->length = size;
    }

    return 0;
}
