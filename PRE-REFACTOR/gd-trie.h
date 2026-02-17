#ifndef GD_TRIE_H 
#define GD_TRIE_H

#include <stdio.h>
#include <stdlib.h>

/**
 * Trie data structure
 */
typedef struct gd__trie_nodeT gd_trie;

/**
 * Recursively frees nodes
 */
void gd_trie_free(gd_trie* root);

/**
 * Initialize the root node of a trie
 */
gd_trie* gd_trie_init(void);

/**
 * Insert a case-sensitive [key, val] pair into a trie
 */
void gd_trie_insert_cs(gd_trie* root, const char* key, const void* value);

/**
 * Insert a [key, val] pair for case-insensitive lookup
 * Sets all lowercase letters to uppercase
 */
void gd_trie_insert_ci(gd_trie* root, const char* key, const void* value);

/**
 * Look up value at given case-sensitive key, 
 * or return NULL if key doesn't exist
 */
const void* gd_trie_get_cs(gd_trie* root, const char* key);

/**
 * Look up value at given case-insensitive key;
 */
const void* gd_trie_get_ci(gd_trie* root, const char* key);

/**
 * Using a case-insensitive key,
 * look up value that may have been inserted with a case-sensitive key.
 * or NULL if it doesn't exist
 */
const void* gd_trie_get_ci_from_cs(gd_trie* root, const char* key);

#endif // GD_TRIE_H
