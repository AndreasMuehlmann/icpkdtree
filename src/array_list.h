#ifndef __ARRAY_LIST__
#define __ARRAY_LIST__

#include <stddef.h>

typedef struct {
    void* ptr;
    size_t len;
    size_t capacity;
    size_t size_of_type;
} ArrayList;

ArrayList* arrayListInit(size_t size_of_type);
void arrayListDeinit(ArrayList *arrayList);
void arrayListAppend(ArrayList *arrayList, void *element);
void arrayListGet(ArrayList *arrayList, void *element, size_t index);

#endif 
