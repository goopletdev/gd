#ifndef GD_ORDERED_MAP_H
#define GD_ORDERED_MAP_H

#include "gd-doubly-linked-list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset

typedef uintptr_t gd__omap_keyT;
typedef uintptr_t gd__omap_valueT;

typedef struct gd__omap_bucketT gd_omap_bucket;
struct gd__omap_bucketT {
    gd__omap_keyT key;
    gd__omap_valueT value;
};

typedef struct gd__omapT gd_omap;
struct gd__omapT {
    size_t size;
    size_t capacity;
    gd_omap_bucket** buckets;
    gd_dll* bucket_list;
};

void gd_omap_free(gd_omap* m, void(*callback)(void* value));
void gd_omap_free_bucket(gd_omap_bucket* b);
gd_omap* gd_omap_init(size_t capacity);
gd_omap_bucket* gd_omap_init_bucket(gd__omap_keyT key, gd__omap_valueT value);

void gd_omap_add(gd_omap* m, gd__omap_keyT key, gd__omap_valueT value);
gd__omap_valueT gd_omap_get(gd_omap* m, gd__omap_keyT key);
// gd_omap_bucket* gd_omap_remove(gd_omap t, gd__omap_keyT key);

#endif // GD_ORDERED_MAP_H
