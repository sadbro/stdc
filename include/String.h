#ifndef STRING_H
#define STRING_H

// inspired from Dr Jonas Birch : https://www.youtube.com/watch?v=WhCbTFBB5Qc

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include "Hashes.h"

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

struct s_string* get_owned_substring(struct s_string* string, unsigned int start, unsigned int end){
    unsigned int n = length_of_String(string);
    assert(end <= n && "[ERR]: Invalid Substring length.");
    unsigned int substring_length = end - start;
    struct s_string* p = (struct s_string*)calloc(1, substring_length + 1 + sizeof(struct s_string));
    assert(p);
    p->count = substring_length;
    copy(p->data, (string)->data + start, substring_length);
    p->data[p->count] = '\0';
    return p;
}

static inline struct s_string* clone(struct s_string* information){
    unsigned int n = length_of_String(information);
    return get_owned_substring(information, 0, n);
}

int* get_lps_table(const char* pattern){
    unsigned int n = length_of_string(pattern);
    int* lps_table = (int*)calloc(n, sizeof(int));
    int len = 0;
    lps_table[0] = 0;
    int i = 1;
    while (i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps_table[i] = len;
            i++;
        }
        else {
            if (len != 0) {
                len = lps_table[len - 1];
            }
            else {
                lps_table[i] = 0;
                i++;
            }
        }
    }
    return lps_table;
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

static inline bool equals_for_n(struct s_string* a, const char* b, unsigned int n){
    for(unsigned int i = 0; i < n; i++){
        if (a->data[i] != b[i]){
            return false;
        }
    }
    return true;
}

bool contains_single(struct s_string* haystack, const char needle){
    unsigned int n = (haystack)->count;
    assert((haystack)->data[(haystack)->count] == '\0' && ((haystack)->count >= 0) && "[ERR]: Non null terminating data found");
    for (unsigned int i = 0; i < n; i++){
        if ((haystack)->data[i] == needle){
            return true;
        }
    }
    return false;
}

unsigned int count_single(struct s_string* haystack, const char needle){
    unsigned int n = (haystack)->count, c = 0;
    assert((haystack)->data[(haystack)->count] == '\0' && ((haystack)->count >= 0) && "[ERR]: Non null terminating data found");
    for (unsigned int i = 0; i < n; i++){
        if ((haystack)->data[i] == needle){
            c++;
        }
    }
    return c;
}

bool contains_string(struct s_string* haystack, const char* needle){
    unsigned int n = length_of_String(haystack), l = length_of_string(needle);
    assert(n >= l && "[ERR]: Invalid needle string length.");
    if (n == l){
        return equals_string(haystack, needle);
    } else {
        int* lps = get_lps_table(needle);
        int i = 0, j = 0;
        while ((n - i) >= (l - j)) {
            if (needle[j] == (haystack)->data[i]) {
                j++;
                i++;
            }
            if (j == l) {
                free(lps);
                return true;
            }
            else if ((i < n) && (needle[j] != (haystack)->data[i])) {
                if (j != 0){
                    j = lps[j - 1];
                }
                else {
                    i++;
                }
            }
        }
        free(lps);
        return false;
    }
}

int find_single(struct s_string* haystack, const char needle){
    unsigned int n = (haystack)->count;
    assert((haystack)->data[(haystack)->count] == '\0' && ((haystack)->count >= 0) && "[ERR]: Non null terminating data found");
    for (unsigned int i = 0; i < n; i++){
        if ((haystack)->data[i] == needle){
            return (int)i;
        }
    }
    return -1;
}

int find_string(struct s_string* haystack, const char* needle){
    unsigned int n = length_of_String(haystack), l = length_of_string(needle);
    assert(n >= l && "[ERR]: Invalid needle string length.");
    if (n == l){
        return equals_string(haystack, needle);
    } else {
        int* lps = get_lps_table(needle);
        int i = 0, j = 0;
        while ((n - i) >= (l - j)) {
            if (needle[j] == (haystack)->data[i]) {
                j++;
                i++;
            }
            if (j == l) {
                free(lps);
                return i - j;
            }
            else if ((i < n) && (needle[j] != (haystack)->data[i])) {
                if (j != 0)
                    j = lps[j - 1];
                else
                    i++;
            }
        }
        free(lps);
        return -1;
    }
}

unsigned long apply_hash(struct s_string* str, HASH_API_STRING _hash){
    assert((str)->data[(str)->count] == '\0' && ((str)->count >= 0) && "[ERR]: Non null terminating data found");
    return _hash((str)->data);
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

static inline void trim(struct s_string* string){
    trim_char(string, ' ');
}

static inline struct s_string* yield_owned_token(struct s_string* string, const char delimiter){
    unsigned int n = length_of_String(string);
    assert(n && n > 0 && "[ERR]: String exhausted.");
    if (!contains_single(string, delimiter)){
        return clone(string);
    }
    unsigned int i = 0;
    char c;
    for (; i < n; i++){
        c = (string)->data[i];
        if (c == delimiter){
            char* dst = (char*)(string)->data + i;
            struct s_string* substr = get_owned_substring(string, 0, i);
            copy((string)->data, dst, (string)->count);
            trim_char_from_left(string, delimiter);
            (string)->data[(string)->count] = '\0';
            return substr;
        } else {
            continue;
        }
    }
    return NULL;
}

static inline struct s_string* get_owned_last_token(struct s_string* string, const char delimiter){
    unsigned int n = length_of_String(string), i = n;
    assert(n && n > 0 && "[ERR]: String exhausted.");
    while(i > 0){
        if ((string)->data[i] == delimiter){
            return get_owned_substring(string, i + 1, n);
        }
        i--;
    }
    return string;
}

static inline bool has_prefix_string(struct s_string* string, const char* predicate){
    unsigned int check_length = length_of_string(predicate);
    assert((string)->count >= check_length && "[ERR]: Invalid Prefix length.");
    return equals_for_n(string, predicate, check_length);
}

static inline bool has_suffix_string(struct s_string* string, const char* predicate){
    assert((string)->data[(string)->count] == '\0' && ((string)->count >= 0) && "[ERR]: Non null terminating data found");
    unsigned int check_length = length_of_string(predicate), n = length_of_String(string);
    assert((string)->count >= check_length && "[ERR]: Invalid Suffix length.");
    unsigned int check_start = n - check_length, i = 0;
    for(; check_start < n; check_start++, i++){
        if ((string)->data[check_start] != predicate[i]){
            return false;
        }
    }
    return true;
}

#endif //STRING_H
