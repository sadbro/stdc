#ifndef STDC_H
#define STDC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include "include/LinkedList.h"
#include "include/Set.h"
#include "include/Matrix.h"
#include "include/Map.h"
#include "include/Stack.h"
#include "include/Tree.h"
#include "include/Vector.h"
#include "include/Pattern.h"
#include "include/String.h"
#include "include/Hashes.h"

#define length(x) _Generic((x),             \
    String: length_of_String,               \
    LinkedList_INT: length_of_linked_list   \
)(x)

#endif // STDC_H
