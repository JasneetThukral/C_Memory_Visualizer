
#include <stdlib.h>

#include "vector_mod.h"

#define INIT_VEC_SIZE 1024


void vector_init(vector_t* v) {
    v->buffer = (void **) malloc(INIT_VEC_SIZE * sizeof(void *));
    v->size = 0;
    v->allocSize = INIT_VEC_SIZE;
}

void vector_destroy(vector_t* v) {
    free(v->buffer);
}

void vector_append(vector_t* v, void *item) {
    if(v->size == v->allocSize) {
        v->allocSize = v->allocSize*2;
        v->buffer = (void **) realloc(v->buffer, v->allocSize);
    }

    v->buffer[v->size] = item;
    v->size++;
}

void *vector_remove(vector_t* v) {
    if(0 >= v->size) {
        return NULL;
    }

    return v->buffer[0];
}

unsigned int vector_size(vector_t* v) {
    return v->size;
}