#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include <math.h>
#include <string.h>

#define P 53.0

size_t hash_str(char* string){

    double _hash = 0;
    for (size_t i=0;i<strlen(string);i++){ _hash += ((double)(string[i] - 'a' + 1)) * (pow(P, (double)i)); }

    return ((int)_hash) % ((int)INT_MAX);
}

size_t hash_int(int key){

    return (key < 0) ? (size_t)(-2*key) : (size_t)(key);
}

#define outerr_backend(fmt, key) fprintf(stderr, "KeyError: `"fmt"` does not exist as a key.", key);exit(1);
void outerr_str(char* key) { outerr_backend("%s", key); }
void outerr_int(int key) { outerr_backend("%d", key); }

#ifndef hash
#define hash(key) _Generic((key), char*:hash_str, int: hash_int)(key)
#endif

#define SET(key_type) struct { key_type keys[INT_MAX];int exists[INT_MAX];size_t size; }

#define outerr(key) _Generic((key), char*:outerr_str, int: outerr_int)(key)

#define set_exists(set, key) set->exists[hash(key)]

#define set_add(set, key){                      \
    do {                                    \
        size_t index = hash(key);           \
        if (!set->exists[index]) {          \
            set->keys[set->size++] = key;   \
            set->exists[index] = 1;         \
        }                                   \
    } while(0);                             \
}                                           \

#define set_insert(set, key, position){                     \
    do {                                                \
        size_t index = hash(key);                       \
        if (!set->exists[index]) {                      \
            set->size++;                                \
            for(size_t i=set->size; i>position; i--){   \
                set->keys[i] = set->keys[i-1];          \
            }                                           \
            set->keys[position] = key;                  \
        }                                               \
    } while(0);                                         \
}                                                       \

#define set_remove(set, key){                           \
    do {                                            \
        set->exists[hash(key)] = 0;                 \
        size_t position = 0;                        \
        for(size_t i=0; i<set->size; i++){          \
            if(set->keys[i] == key){                \
                position = i;                       \
            }                                       \
        }                                           \
        for(size_t i=position; i<set->size; i++){   \
            set->keys[i] = set->keys[i+1];          \
        }                                           \
        set->size--;                                \
    } while(0);                                     \
}                                                   \

#endif // SET_H
