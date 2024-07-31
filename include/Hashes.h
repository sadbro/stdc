#ifndef HASHES_H
#define HASHES_H

typedef unsigned long (*HASH_API_STRING) (const char*);

typedef struct {
    HASH_API_STRING djb2;
    HASH_API_STRING sdbm;
    HASH_API_STRING jenkins;
} HASH;

unsigned long djb2(const char *str){
    unsigned long hash = 5381;
    char c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

unsigned long sdbm(const char *str){
    unsigned long hash = 0;
    char c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

unsigned long jenkins(const char *str)
{
    unsigned long hash, i;
    for (hash = i = 0; str[i] != '\0'; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

const HASH Hash = {
        .djb2 = djb2,
        .sdbm = sdbm,
        .jenkins = jenkins
};

#endif //HASHES_H
