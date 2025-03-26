#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "array_list.h"


ArrayList* arrayListInit(size_t size_of_type) {
    ArrayList *arrayList = (ArrayList*)malloc(sizeof(ArrayList));
    arrayList->ptr = malloc(0);
    arrayList->len = 0;
    arrayList->capacity = 0;
    arrayList->size_of_type = size_of_type;
    return arrayList;
}

void arrayListDeinit(ArrayList *arrayList) {
    free(arrayList->ptr);
    free(arrayList);
}

void arrayListGet(ArrayList *arrayList, void *element, size_t index) {
    if (index >= arrayList->len) {
        element = NULL;
        printf("Error: Access out of bounds.\n");
        return;
    }
    memcpy(element, (char*)arrayList->ptr + arrayList->size_of_type * index, arrayList->size_of_type);
}

void arrayListAppend(ArrayList *arrayList, void *element) {
    if (arrayList->len + 1 > arrayList->capacity) {
        arrayList->capacity = arrayList->capacity * 2 + 1;
        arrayList->ptr = realloc(arrayList->ptr, arrayList->capacity * arrayList->size_of_type);
    }
    memcpy((char*)arrayList->ptr + arrayList->size_of_type * arrayList->len, element, arrayList->size_of_type);
    arrayList->len += 1;
}
