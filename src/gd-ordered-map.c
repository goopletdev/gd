#include "gd-ordered-map.h"

gd_omap* gd_omap_init(size_t capacity) {
    if (__builtin_popcount(capacity) != 1) {
        printf("Disallowed map capacity [%zu]. Must be power of 2\n", capacity);
        exit(1);
    }
    gd_omap* t = (gd_omap*)malloc(sizeof(gd_omap));
    t->buckets = (gd_omap_bucket**)calloc(capacity, sizeof(gd_omap_bucket*));
    t->capacity = capacity;
    t->size = 0;
    t->bucket_list = gd_dll_init();
    return t;
}

gd_omap_bucket* gd_omap_init_bucket(gd__omap_keyT key, gd__omap_valueT value) {
    gd_omap_bucket* b = (gd_omap_bucket*)malloc(sizeof(gd_omap_bucket));
    b->key = key;
    b->value = value;
    return b;
}

void gd_omap_free(gd_omap* m, void (*callback)(void* value)) {
    gd_dll_node* n = m->bucket_list->head;
    for (; n; n = n->next) {
        callback((void*)((gd_omap_bucket*)n->value)->value);
        free((gd_omap_bucket*)n->value);
    }
    gd_dll_cleanup(m->bucket_list, NULL);
    free(m->buckets);
    free(m);
}

size_t gd__omap_hash_function(gd_omap* t, gd__omap_keyT key) {
    size_t hash = 0;
    for (size_t trailing = __builtin_ctz(t->capacity); key; key >>= trailing) {
        hash ^= key;
    }
    // return hash & (t->capacity - 1)
    return hash % t->capacity;
}

size_t gd__omap_probe_null(gd_omap* t, size_t target_index) {
    size_t i = target_index;
    size_t max = t->capacity;
    probe:
    for (; i < max; i++) {
        if (!t->buckets[i]) {
            return i;
        }
    }
    if (target_index && (i > target_index)) {
        i = 0;
        max = target_index;
        goto probe;
    }
    return t->capacity;
}

void gd__omap_insert_bucket(gd_omap* m, gd_omap_bucket* b) {
    size_t target_index = gd__omap_hash_function(m, b->key);
    size_t insertion_index = gd__omap_probe_null(m, target_index);
    m->buckets[insertion_index] = b;
}

void gd_omap_resize(gd_omap* m) {
    m->capacity <<= 1;
    m->buckets = realloc(m->buckets, sizeof(gd_omap_bucket*) * m->capacity);
    memset(m->buckets, 0, sizeof(gd_omap_bucket*) * m->capacity);
    gd_dll_node* n = m->bucket_list->head;
    for (; n; n = n->next) {
        gd__omap_insert_bucket(m, (gd_omap_bucket*)n->value);
    }
}

void gd_omap_add(gd_omap* m, gd__omap_keyT key, gd__omap_valueT value) {
    if (m->size >= (m->capacity >> 1)) {
        gd_omap_resize(m);
    }
    gd_omap_bucket* b = gd_omap_init_bucket(key, value);
    gd_dll_push(m->bucket_list, b);
    gd__omap_insert_bucket(m, b);

    m->size++;
}

gd__omap_valueT gd_omap_get(gd_omap* m, gd__omap_keyT key) {
    size_t target_index = gd__omap_hash_function(m, key);
    size_t i = target_index;
    size_t max = m->capacity;
    probe:
    for (gd_omap_bucket* b; (b = m->buckets[i]) && i < max; i++) {
        if (!b) {
            return (gd__omap_valueT)NULL;
        }
        if (b->key == key) {
            return b->value;
        }
    }
    if (target_index && (i > target_index)) {
        i = 0;
        max = target_index;
        goto probe;
    }
    return (gd__omap_valueT)NULL;
}


