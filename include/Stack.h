#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>

typedef struct {

    int* backend;
    int length;
    int maxLength;

} Stack;

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

int stack_isEmpty(Stack* _addr){ return ((_addr)->length == 0); }

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

#endif // STACK_H
