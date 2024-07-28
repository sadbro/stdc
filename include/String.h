#ifndef STRING_H
#define STRING_H

// inspired from: https://www.youtube.com/watch?v=WhCbTFBB5Qc

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define to_string(x) (x)->data

struct s_string {
    unsigned int count;
    char data[0];
};

typedef struct s_string* String;

void copy(void* dst, const void* src, const unsigned int n){
    const char* psrc;
    char* pdst;
    unsigned int _x = n;
    for(psrc = (char*)src, pdst = (char*)dst; _x; psrc++, pdst++){
        *pdst = *psrc;
        _x--;
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

bool concat_str(struct s_string* dst, const char* src){
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

#endif //STRING_H
