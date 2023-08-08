#ifndef STDC_H
#define STDC_H

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

size_t VEC_LIMIT = 1;
void limit(){ VEC_LIMIT++; }
void set_limit(size_t n){ VEC_LIMIT = n; }

short equal_ll(long long a, long long b){ return a == b; }
short equal_str(char* a, char* b){ return !strcmp(a, b); }

#define equal(a, b) _Generic((a), int:equal_ll, long:equal_ll, long long:equal_ll, char*:equal_str)(a, b)

#define VectorCtor(type) struct { type* data;size_t size; }
#define VEC_TYPE(type) typedef VectorCtor(type) Vector

#define VEC_INIT(vv){                                   \
    do {                                                \
        vv = (Vector*)malloc( sizeof(Vector) );         \
        vv->data = calloc( VEC_LIMIT, sizeof(*(vv->data)) );  \
        vv->size = 0;                                   \
    } while(0);                                         \
}                                                       \

#define VEC_INIT_N(vv, n, v){                           \
    do {                                                \
        vv = malloc( sizeof(Vector) );                  \
        set_limit(n);                                   \
        vv->data = calloc( VEC_LIMIT, sizeof(*(vv->data)) );  \
        for(size_t i=0;i<n;i++){ vv->data[i] = v; }     \
        vv->size = n;                                   \
    } while(0);                                         \
}                                                       \

#define vector_insert(vv, value){                                                  \
    do {                                                                    \
        if (vv->size == VEC_LIMIT){                                               \
            limit();                                                        \
            vv->data = reallocarray(vv->data, VEC_LIMIT, sizeof(*((vv)->data)));  \
        }                                                                   \
        vv->data[vv->size++] = value;                                       \
    } while(0);                                                             \
}                                                                           \

#define vector_length(vv) vv->size
#define vector_at(vv, i) vv->data[i]

#define vector_erase(vv, value){                                                   \
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

#define vector_clear(vv) {                     \
    do {                                \
        memset(vv->data, 0, vv->size);  \
        vv->size = 0;                   \
    } while(0);                         \
}                                       \

#define P 53.0
#define M ((int)1e5+9)

size_t hash_str(char* string){

    double __hash = 0;
    for (size_t i=0;i<strlen(string);i++){ __hash += ((double)(string[i] - 'a' + 1)) * (pow(P, (double)i)); }

    return ((int)__hash) % ((int)M);
}

size_t hash_int(int key){

    return (key < 0) ? (size_t)(-2*key) : (size_t)(key);
}

#define outerr_backend(fmt, key) fprintf(stderr, "KeyError: `"fmt"` does not exist as a key.", key);exit(1);
void outerr_str(char* key) { outerr_backend("%s", key); }
void outerr_int(int key) { outerr_backend("%d", key); }

#define hash(key) _Generic((key), char*:hash_str, int: hash_int)(key)
#define SET(key_type) struct { key_type keys[M];int exists[M];size_t size; }

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

#define set_remove(set, key){               \
    do {                                \
        set->exists[hash(key)] = 0;     \
        (void)set->keys[set->size--];   \
    } while(0);                         \
}                                       \

#define MAP(key_type, value_type) struct { key_type keys[M];value_type values[M];int exists[M];size_t size; }

#define map_get(map, key, dst){                         \
    do {                                            \
        size_t index = hash(key);                   \
        if (! map->exists[index]){ outerr(key); }   \
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

#define HMAP_TYPES(key, value) typedef MAP(key, value) HashMap

typedef struct {

    int* backend;
    int length;
    int maxLength;

} Stack;

// TODO: make stack ctor type-free

Stack* STACK(int maxLength){

    int* data = (int*)calloc( maxLength, sizeof(int) );
    Stack* stack_addr = (Stack*)malloc( sizeof(Stack) );

    stack_addr->backend = data;
    stack_addr->length = 0;
    stack_addr->maxLength = maxLength;

    return stack_addr;
}

void STACK_FREE(Stack* _addr){

    free(_addr->backend);
    free(_addr);
}

int stack_isEmpty(Stack* _addr){ return (_addr->length == 0); }

void stack_push(Stack* _addr, int value){

    assert( _addr->length < _addr->maxLength );
    _addr->backend[_addr->length++] = value;
}

int stack_pop(Stack* _addr){

    assert( stack_isEmpty(_addr) != 1 );
    return _addr->backend[--_addr->length];
}

int stack_peek(Stack* _addr){

    return _addr->backend[_addr->length-1];
}

typedef struct NODE {

    int value;
    struct NODE** children;
    int numChildren;

} Tree;

Tree* TREE(int val, int num_children){

    Tree* tree = (Tree*)calloc( 1, sizeof(Tree) );

    tree->value = val;
    tree->numChildren = num_children;

    if (num_children > 0){

        tree->children = (Tree**)calloc( num_children , sizeof(Tree) );
        for (int i=0; i<num_children; i++){ tree->children[i] = TREE(0, 0); }
    }

    return tree;
}

void TREE_FREE(Tree* tree){

    if( tree->numChildren == 0 ){ free(tree);return; }

    for(int i=0; i<(tree->numChildren); i++){ TREE_FREE(tree->children[i]); }

    free(tree->children);
    free(tree);
}

void set_tree(Tree* tree, int* values, int n){

    for(int i=0; i<n; i++){ tree->children[i]->value = values[i]; }
}

#endif // STDC_H
