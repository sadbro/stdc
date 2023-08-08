#ifndef STDC_H
#define STDC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

// VECTOR
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

// SET

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

// MAP

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

#define SET_MAP_TYPES(key, value) typedef MAP(key, value) HashMap

// STACK

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

// STACK

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

// MATRIX

typedef struct {

    size_t rows;
    size_t cols;
    size_t stride;
    int* start;

} Matrix;

Matrix MATRIX(size_t rows, size_t cols, float _default){

    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.start = malloc(sizeof(int) * rows * cols);

    for(size_t i=0; i<rows*cols; i++){ m.start[i] = _default; }

    assert(m.start != NULL);
    return m;
}

#define MAT_AT(m, i, j) (m).start[(i)*(m).cols + (j)]

Matrix identity_matrix(size_t order){

    Matrix m = MATRIX(order, order, 0);
    for(size_t i=0; i<m.rows; i++){

        for(size_t j=0; j<m.cols; j++){

            if(i == j){ MAT_AT(m, i, j) += 1; }
        }
    }

    return m;
}

#define MATRIX_PRINT(m) print_mat(m, #m)

#define matrix_fill(matrix, value) randomise(matrix, value, value)

Matrix matrix_row_loc(Matrix m, size_t row_start, size_t row_end){

    assert( row_end <= m.rows );
    assert( row_end > row_start );

    size_t nrows = row_end - row_start;

    return (Matrix){

        .rows = nrows,
        .cols = m.cols,
        .start = &MAT_AT(m, row_start, 0)
    };
}

Matrix matrix_col_loc(Matrix m, size_t col_start, size_t col_end){

    assert( col_end <= m.cols );
    assert( col_end > col_start );

    size_t ncols = col_end - col_start;

    Matrix result = MATRIX(m.rows, ncols, 0.0f);

    for (size_t i=0; i<result.rows; i++){

        for (size_t j=0; j<result.cols; j++){

            MAT_AT(result, i, j) = MAT_AT(m, i, col_start + j);
        }
    }

    return result;
}

void matrix_sum(Matrix dst, Matrix src){

    assert((dst.rows == src.rows) && (dst.cols == src.cols));

    for(size_t i=0; i<dst.rows; i++){

        for(size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) += MAT_AT(src, i, j);
        }
    }
}

void matrix_dot(Matrix dst, Matrix a, Matrix b){

    assert(a.cols == b.rows);
    assert((dst.rows == a.rows) && (dst.cols == b.cols));
    size_t n = a.cols;

    for(size_t i=0; i<dst.rows; i++){

        for(size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) = 0.0f;
            for(size_t k=0; k<n; k++){

                MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
}

void matrix_copy(Matrix dst, Matrix src){

    assert( (dst.cols == src.cols) && (dst.rows == src.rows) );

    for (size_t i=0; i<dst.rows; i++){

        for (size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) = MAT_AT(src, i, j);
        }
    }
}

// LINKED LIST FOR INT

typedef struct LL_PROTOTYPE {

    int value;
    struct LL_PROTOTYPE* next;

} LinkedList_INT;

#define NODE(value) (LinkedList_INT){ .value=value, .next=NULL }

LinkedList_INT* Node(int value){

    LinkedList_INT* node = (LinkedList_INT*)malloc( sizeof(LinkedList_INT) );
    node->value = value;
    node->next = NULL;
    return node;
}

void LL_FREE(LinkedList_INT* head){

    LinkedList_INT* current = {0};
    while( head != NULL ){

        current = head;
        head = head->next;
        free(current);
    }
}

int ll_getLength(LinkedList_INT* current){

    int count = 0;
    while (current != NULL){ count++;current = current->next; }

    return count;
}

void ll_traversefn(LinkedList_INT* _addr, void(*function)(LinkedList_INT*, va_list), ...){

    va_list argv;
    va_start(argv, function);

    LinkedList_INT* intermediate = _addr;
    while (intermediate != NULL){

        function(intermediate, argv);
        intermediate = intermediate->next;
    }

    va_end(argv);
};

void out_prototype(LinkedList_INT* _, va_list argv){(void)argv;printf("{ value = %d }->", _->value);}

#define LL_PRINT out_prototype

void ll_append(LinkedList_INT* current, int value){

    LinkedList_INT* _new = Node(value);

    while (current->next != NULL){current = current->next;}

    current->next = _new;
}

void ll_insert(LinkedList_INT** _addr, int value, int index){

    assert( index <= ll_getLength(*_addr) );

    LinkedList_INT* new_node = Node(value);
    LinkedList_INT* head = *_addr;

    if (index == 0){ new_node->next = head;*_addr = new_node;return; }

    while(--index){head = head->next;}

    LinkedList_INT* RLL = head->next;
    head->next = new_node;
    new_node->next = RLL;
}

int ll_in(int value, LinkedList_INT* head){

    while( head != NULL ){

        if (head->value == value) { return 1;}
        head = head->next;
    }

    return 0;
}

void ll_delete(LinkedList_INT** _addr, int value){

    assert(ll_in(value, *_addr));

    LinkedList_INT* current = *_addr;
    LinkedList_INT* old_node = {0};
    if (current->value == value){

        old_node = current;
        *_addr = current->next;
        free(old_node);
        return;
    }

    while(current->next->value != value){current = current->next;}

    old_node = current->next;
    current->next = current->next->next;
    free(old_node);
}

#endif // STDC_H
