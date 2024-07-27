#ifndef MAP_H
#define MAP_H

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define P 53.0

size_t map_hash_str(char* string){

    double _hash = 0;
    for (size_t i=0;i<strlen(string);i++){ _hash += ((double)(string[i] - 'a' + 1)) * (pow(P, (double)i)); }

    return ((int)_hash) % ((int)INT_MAX);
}

size_t map_hash_int(int key){

    return (key < 0) ? (size_t)(-2*key) : (size_t)(key);
}

#define map_outerr_backend(fmt, key) fprintf(stderr, "KeyError: `"fmt"` does not exist as a key.", key);exit(1);
void map_outerr_str(char* key) { map_outerr_backend("%s", key); }
void map_outerr_int(int key) { map_outerr_backend("%d", key); }

#ifndef hash
#define hash(key) _Generic((key), char*:map_hash_str, int: map_hash_int)(key)
#endif

#define MAP(key_type, value_type) struct { key_type keys[INT_MAX];value_type values[INT_MAX];int exists[INT_MAX];size_t size; }

#define map_outerr(key) _Generic((key), char*:map_outerr_str, int: map_outerr_int)(key)

#define map_get(map, key, dst){                         \
    do {                                            \
        size_t index = hash(key);                   \
        if (! map->exists[index]){ map_outerr(key); }   \
        dst = map->values[hash(key)];               \
    } while(0);                                     \
}                                                   \

#define map_set(map, key, value){               \
    do {                                    \
        size_t index = hash(key);           \
        if (!map->exists[index]) {          \
            map->keys[map->size++] = key;   \
            map->exists[index] = 1;         \
        }                                   \
        map->values[index] = value;         \
    } while(0);                             \
}                                           \

#define map_remove(map, key){               \
    do {                                \
        map->exists[hash(key)] = 0;     \
        (void)map->keys[map->size--];   \
    } while(0);                         \
}                                       \

#define map_pair(map, i) map->keys[i], map->values[hash(map->keys[i])]  //does not check for non-existence

// TODO: mapping possible in str->int, int->int yet. make it truly customisable

#define SET_MAP_TYPE(key, value) typedef MAP(key, value)

#endif // MAP_H
