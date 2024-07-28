#ifndef STRING_H
#define STRING_H

// inspired from: https://www.youtube.com/watch?v=WhCbTFBB5Qc

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define uninit(x) free(x)
#define data(x) (x)->data

struct s_string {
    unsigned int count;
    char data[0];
};

unsigned int length_of_string(char* str){
    unsigned int _n;
    char* temp;
    for(_n = 0, temp = str; *temp; temp++, _n++);
    return _n;
}

struct s_string* init(char* str){
    unsigned int size = length_of_string(str) + 1 + sizeof(struct s_string), n = length_of_string(str);
    struct s_string* p = (struct s_string*) calloc(1, size);
    assert(p);
    p->count = n;
    memcpy(p->data, str, n);
    return p;
}

bool concat(struct s_string*, const char*);

#endif //STRING_H
