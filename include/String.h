#ifndef STRING_H
#define STRING_H

// inspired from Dr Jonas Birch : https://www.youtube.com/watch?v=WhCbTFBB5Qc

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

struct s_string {
    unsigned int count;
    char data[0];
};

typedef struct s_string* String;

static inline const char* _bool_to_string(bool x) { return (x) ? "True" : "False"; }

static inline const char* _string(struct s_string* x) { return (x)->data; }

static inline unsigned int length_of_String(struct s_string* x) { return (x)-> count; }

#define to_string(x) _Generic((x), String:_string, bool: _bool_to_string) (x)

void copy(void* dst, const void* src, const unsigned int n){
    const char* psrc;
    char* pdst;
    unsigned int _x = n;
    for(psrc = (char*)src, pdst = (char*)dst; _x--; psrc++, pdst++){
        *pdst = *psrc;
    }
}

unsigned int length_of_string(const char* str){
    unsigned int _n;
    const char* temp;
    for(_n = 0, temp = str; *temp; temp++, _n++);
    return _n;
}

struct s_string* from_string(const char* str){
    unsigned int size = length_of_string(str) + 1 + sizeof(struct s_string), n = length_of_string(str);
    struct s_string* p = (struct s_string*) calloc(1, size);
    assert(p);
    p->count = n;
    copy(p->data, str, n);
    return p;
}

bool concat_string(struct s_string* dst, const char* src){
    unsigned int n = length_of_string(src), size;
    struct s_string* p;
    size = (dst->count + 1) + sizeof(struct s_string) + n;
    p = (struct s_string*)realloc(dst, size);
    if (!p){
        return false;
    }
    copy(p->data + p->count, src, n);
    p->count += n;
    return true;
}

unsigned long apply_hash(struct s_string* str, HASH_API_STRING _hash){
    assert((str)->data[(str)->count] == '\0' && "[ERR]: Non null terminating data found");
    return _hash((str)->data);
}

bool equals_string(struct s_string* a, const char* b){
    unsigned int n = length_of_string(b);
    if (n != a->count){
        return false;
    }
    for(unsigned int i = 0; i < n; i++){
        if (a->data[i] != b[i]){
            return false;
        }
    }
    return true;
}

static inline bool equals(struct s_string* a, struct s_string* b){
    if (a->count != b->count){
        return false;
    }
    return equals_string((a), (b)->data);
}

static inline bool is_single(struct s_string* string){
    return (string)->count == 1;
}

static inline bool is_printable_ascii(const char c){
    return ((c >= 32) && (c <= 127));
}

static inline bool is_alphabetic(struct s_string* string){
    unsigned int n = (string)->count;
    for(unsigned int i = 0; i < n; i++){
        const char predicate = (string)->data[i];
        assert(is_printable_ascii(predicate) && "[ERR] Non printable character encountered");
        if (((predicate < 'A') || (predicate > 'Z')) && ((predicate < 'a') || (predicate > 'z'))){
            return false;
        }
    }
    return true;
}

static inline bool is_numeric(struct s_string* string){
    unsigned int n = (string)->count;
    for(unsigned int i = 0; i < n; i++){
        const char predicate = (string)->data[i];
        assert(is_printable_ascii(predicate) && "[ERR] Non printable character encountered");
        if ((predicate < '0') || (predicate > '9')){
            return false;
        }
    }
    return true;
}

static inline bool is_alphanumeric(struct s_string* string){
    unsigned int n = (string)->count;
    for(unsigned int i = 0; i < n; i++){
        const char predicate = (string)->data[i];
        assert(is_printable_ascii(predicate) && "[ERR] Non printable character encountered");
        if ((predicate < '0') || ((predicate > '9') && (predicate < 'A')) || ((predicate > 'Z') && (predicate < 'a')) || (predicate > 'z')){
            return false;
        }
    }
    return true;
}

static inline void convert_to_uppercase(struct s_string* string){
    unsigned int i = 0, gap = 'a' - 'A';
    while((string)->data[i] != '\0'){
        const char current = (string)->data[i];
        assert(is_printable_ascii(current) && "[ERR] Non printable character encountered");
        if ((current >= 'a') && (current <= 'z')){
            (string)->data[i] = (char)((unsigned int)(string)->data[i] - gap);
        }
        i++;
    }
}

static inline void convert_to_lowercase(struct s_string* string){
    unsigned int i = 0, gap = 'a' - 'A';
    while((string)->data[i] != '\0'){
        const char current = (string)->data[i];
        assert(is_printable_ascii(current) && "[ERR] Non printable character encountered");
        if ((current >= 'A') && (current <= 'Z')){
            (string)->data[i] = (char)((unsigned int)(string)->data[i] + gap);
        }
        i++;
    }
}

static inline void convert_to_capitalcase(struct s_string* string){
    unsigned int gap = 'a' - 'A', i = 1, n = (string)->count;
    const char start = (string)->data[0];
    if ((start >= 'A') && (start <= 'Z')) {
        (string)->data[0] = (char)((unsigned int)(string)->data[0] + gap);
    } else if ((start >= 'a') && (start <= 'z')) {
        (string)->data[0] = (char)((unsigned int)(string)->data[0] - gap);
    }
    for (;i < n; i++){
        const char current = (string)->data[i];
        const char previous = (string)->data[i-1];
        if (((previous < 'A') || (previous > 'Z')) && ((previous < 'a') || (previous > 'z'))){
            if ((current >= 'A') && (current <= 'Z')){
                (string)->data[i] = (char)((unsigned int)(string)->data[i] + gap);
            } else if ((current >= 'a') && (current <= 'z')) {
                (string)->data[i] = (char)((unsigned int)(string)->data[i] - gap);
            }
        }
    }
}

void trim_char_from_left(struct s_string* string, const char delimiter){
    unsigned int i = -1;
    while(((string)->data[++i] == delimiter) && (string)->count){
        (string)->count--;
    }
    char* dst = (char*)(string)->data + i;
    copy((string)->data, dst, (string)->count);
    (string)->data[(string)->count] = '\0';
}

void trim_char_from_right(struct s_string* string, const char delimiter){
    unsigned int i = (string)->count;
    while(((string)->data[--i] == delimiter) && (string)->count){
        (string)->count--;
    }
    (string)->data[(string)->count] = '\0';
}

static inline void trim_char(struct s_string* string, const char delimiter){
    trim_char_from_right(string, delimiter);
    trim_char_from_left(string, delimiter);
}

#endif //STRING_H
