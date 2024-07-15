// Inspired by tsoding
// TODO: To be self implemented later

#include <stdbool.h>
#include <stdio.h>

#ifndef STDC_PATTERN_H
#define STDC_PATTERN_H

#define BOOL_TO_STR(b) ((b) ? "true" : "false")

bool glob(const char* pattern, const char* text){
    while ((*pattern != '\0') && (*text != '\0')){
        switch (*pattern) {
            case '?': {
                pattern += 1;
                text += 1;
                break;
            }
            case '*': {
                if (glob(pattern + 1, text)) return true;
                text += 1;
                break;
            }
            case '[': {
                pattern += 1;
                bool matched = false;
                while (*pattern != ']' && *pattern != '\0'){
                    matched |= (*pattern == *text);
                    pattern += 1;
                }
                if (*pattern != ']'){
                    fprintf(stderr, "ERR: Closure not found");
                    exit(1);
                }
                if(!matched) return false;

                pattern += 1;
                text += 1;
                break;
            }
            default:{
                if (*pattern == *text){
                    pattern += 1;
                    text += 1;
                } else {
                    return false;
                }
            }
        }
    }
    if (*text == '\0'){
        while (*pattern == '*') pattern += 1;
        return *pattern == '\0';
    }

    return false;
}

void CHECK_GLOB(const char* pattern, const char* text){
    printf("Matching: '%s' <=> '%s'\n", pattern, text);
    printf("Found: %s", BOOL_TO_STR(glob(pattern, text)));
}

#endif //STDC_PATTERN_H
