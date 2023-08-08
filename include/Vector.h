#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

size_t MAX = 1;
void limit(){ MAX++; }
void set_limit(size_t n){ MAX = n; }

short equal_ll(long long a, long long b){ return a == b; }
short equal_str(char* a, char* b){ return !strcmp(a, b); }

#define equal(a, b) _Generic((a), int:equal_ll, long:equal_ll, long long:equal_ll, char*:equal_str)(a, b)

#define VectorCtor(type) struct { type* data;size_t size; }
#define VEC_TYPE(type) typedef VectorCtor(type) Vector

#define VEC_INIT(vv){                                   \
    do {                                                \
        vv = (Vector*)malloc( sizeof(Vector) );         \
        vv->data = calloc( MAX, sizeof(*(vv->data)) );  \
        vv->size = 0;                                   \
    } while(0);                                         \
}                                                       \

#define VEC_INIT_N(vv, n, v){                           \
    do {                                                \
        vv = malloc( sizeof(Vector) );                  \
        set_limit(n);                                   \
        vv->data = calloc( MAX, sizeof(*(vv->data)) );  \
        for(size_t i=0;i<n;i++){ vv->data[i] = v; }     \
        vv->size = n;                                   \
    } while(0);                                         \
}                                                       \

#define vector_insert(vv, value){                                           \
    do {                                                                    \
        if (vv->size == MAX){                                               \
            limit();                                                        \
            vv->data = reallocarray(vv->data, MAX, sizeof(*((vv)->data)));  \
        }                                                                   \
        vv->data[vv->size++] = value;                                       \
    } while(0);                                                             \
}                                                                           \

#define length(vv) vv->size
#define at(vv, i) vv->data[i]

#define vector_erase(vv, value){                                            \
    do{                                                                     \
        size_t count = 0;                                                   \
        for(size_t i=0; i<vv->size; i++){                                   \
            if(!equal(vv->data[i], value)){ count++; }                      \
        }                                                                   \
        typeof((vv)->data) tmp = calloc( count, sizeof(*((vv)->data)) );    \
        size_t j = 0;                                                       \
        for(size_t i=0; i<vv->size; i++){                                   \
            if(!equal(vv->data[i], value)){ tmp[j++] = vv->data[i]; }       \
        }                                                                   \
        vv->size = j; free(vv->data); vv->data = tmp;                       \
    } while(0);                                                             \
}                                                                           \

#define vector_clear(vv) {              \
    do {                                \
        memset(vv->data, 0, vv->size);  \
        vv->size = 0;                   \
    } while(0);                         \
}                                       \

#endif // VECTOR_H
