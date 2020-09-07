#include "creator_methods.h"

//make methods 
void * make_queue_queueMod(size_t maxSize) {
    queue * q = queue_create(maxSize);
    return (void*)q;
}
void * make_stack_stackMod(size_t maxSize) {
    (void) maxSize; 
    struct stack* s = create_stack(); 
    return (void*)s; 
}
void * make_vector_vectorMod(size_t maxSize) {
    vector_t * v = NULL;
    vector_init(v);
    return (void*)v;
}
void * make_array_arrayMod(size_t maxSize) {
    void * a = malloc(maxSize);
    return (void*)a;
}
void * make_array_dictionaryMod(size_t maxSize) {
    dictionary_t * d = NULL;
    dictionary_init(d);
    return (void*)d;
}

//destroy methods 
void destroy_queue_queueMod(queue* q) {
    queue_destroy(q); 
}
void destroy_stack_stackMod(stack* s) {
    destroy_stack(s); 
}
void destroy_vector_vectorMod(vector_t * v) {
    vector_destroy(v);
}
void destroy_array_arrayMod(char* a) {
    free(a); 
}
void destroy_array_dictionaryMod(dictionary_t * d) {
    dictionary_destroy(d);
}

//add methods 
void add_queue_queueMod(queue* q, void* elem) {
    queue_push(q, elem); 
}
void add_stack_stackMod(struct stack* s, void* elem) {
    push(s, elem); 
}
void add_vector_vectorMod(vector_t * v, void* elem) {
    vector_append(v, elem); 
}
void add_array_arrayMod(char* a) {
    //do nothing -- not necessary  
}
void add_dictionary_dictionaryMod(dictionary_t * d, char *key, void *value) {
    dictionary_add(d, key,value);
}

//remove methods 
void * remove_queue_queueMod(queue* q) {
    return queue_pull(q); 
}
void * remove_stack_stackMod(struct stack* s, void* elem) {
    return pop(s); 
}
void * remove_vector_vectorMod(vector_t* v) {
    vector_remove(v); 
    return NULL;;
}
void * remove_array_arrayMod(char* a) {
    //do nothing -- not necessary  
    return NULL;
}
void * remove_dictionart_dictionaryMod(dictionary_t * d,char * key) {
    dictionary_remove(d, key);
    return NULL;;
}
