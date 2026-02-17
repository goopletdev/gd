#include "gd-trie.h"

#define SUBTREES_LEN 256

typedef struct gd__trie_nodeT gd_trie_node;
struct gd__trie_nodeT {
    const void* value;
    gd_trie_node** subtrees;
};

void gd_trie_free(gd_trie* root) {
    if (!root) {
        return;
    }
    for (int i = 0; i < SUBTREES_LEN; i++) {
        gd_trie_free(root->subtrees[i]);
    }
    free(root->subtrees);
    free(root);
}

gd_trie* gd_trie_init(void) {
    gd_trie_node* node = (gd_trie_node*)malloc(sizeof(gd_trie_node));
    node->value = NULL;
    node->subtrees = (gd_trie_node**)calloc(SUBTREES_LEN, sizeof(gd_trie_node*));
    return node;
}

const void* gd_trie_get_cs(gd_trie* root, const char* key) {
    gd_trie* current = root;
    char c;
    while ((c = *(key++))) {
        current = current->subtrees[(int)c];
        if (current == NULL) {
            return NULL;
        }
    }
    return current->value;
}

const void* gd_trie_get_ci(gd_trie* root, const char* key) {
    gd_trie* current = root;
    char c;
    while ((c = *(key++))) {
        if (c >= 'a' && c <= 'z') {
            c -= 32;
        }
        current = current->subtrees[(int)c];
        if (current == NULL) {
            return NULL;
        }
    }
    return current->value;
}

void gd_trie_insert_cs(gd_trie* root, const char* key, const void* value) {
    gd_trie* current = root;
    char c;
    while ((c = *(key++))) {
        if (!current->subtrees[(int)c]) {
            current->subtrees[(int)c] = gd_trie_init();
        }
        current = current->subtrees[(int)c];
    }
    current->value = value;
}

void gd_trie_insert_ci(gd_trie* root, const char* key, const void* value) {
    gd_trie* current = root;
    char c;
    while ((c = *(key++))) {
        if (c >= 'a' && c <= 'z') {
            c -= 32;
        }
        if (!current->subtrees[(int)c]) {
            current->subtrees[(int)c] = gd_trie_init();
        }
        current = current->subtrees[(int)c];
    }
    current->value = value;
}

const void* gd_trie_get_ci_from_cs(gd_trie* root, const char* key) {
    char c = *(key++);
    if (!c) {
        return root->value;
    }
    gd_trie* current = root->subtrees[(int)c];
    if (!current) {
        if (c >= 'A' && c <= 'Z') {
            current = root->subtrees[c + 32];
            if (!current) {
                return NULL;
            } else {
                return gd_trie_get_ci_from_cs(current, key);
            }
        } else if (c >= 'a' && c <= 'z') {
            current = root->subtrees[c - 32];
            if (!current) {
                return NULL;
            } else {
                return gd_trie_get_ci_from_cs(current, key);
            }
        } else {
            return NULL;
        }
    }
    const void* value = gd_trie_get_ci_from_cs(current, key);
    if (value) {
        return value;
    } else if (c >= 'A' && c <= 'Z') {
        current = root->subtrees[c + 32];
        if (!current) {
            return NULL;
        } else {
            return gd_trie_get_ci_from_cs(current, key);
        }
    } else if (c >= 'a' && c <= 'z') {
        current = root->subtrees[c - 32];
        if (!current) {
            return NULL;
        } else {
            return gd_trie_get_ci_from_cs(current, key);
        }
    } else {
        return NULL;
    }
}

