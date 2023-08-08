#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

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

int ll_length(LinkedList_INT* current){

    int count = 0;
    while (current != NULL){ count++;current = current->next; }

    return count;
}
void ll_append(LinkedList_INT* current, int value){

    LinkedList_INT* _new = Node(value);

    while (current->next != NULL){current = current->next;}

    current->next = _new;
}

void ll_insert(LinkedList_INT** _addr, int value, int index){

    assert( index <= ll_length(*_addr) );

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

void ll_delete_by_value(LinkedList_INT** _addr, int value){

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

#endif // LINKEDLIST_H
