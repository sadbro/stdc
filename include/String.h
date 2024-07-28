#ifndef STRING_H
#define STRING_H

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define uninit(x) free(x)

struct s_string {
    unsigned int count;
    char data[0];
};

unsigned int length(char* str){
    unsigned int _n;
    char* temp;
    for(_n = 0, temp = str; *temp; temp++, _n++);
    return _n;
}

struct s_string* init(char* str){
    unsigned int size = length(str);
}
bool concat(struct s_string*, const char*);

#endif //STRING_H
