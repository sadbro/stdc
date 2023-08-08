#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdarg.h>

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

void tree_config(Tree* tree, int* values, int n){

    for(int i=0; i<n; i++){ tree->children[i]->value = values[i]; }
}

#endif // TREE_H
