
#pragma once
#include <stdbool.h>
#include "vector_mod.h"
#include "queue_mod.h"
#include "stack_mod.h"
#include "dictionary_mod.h"
#include <stdlib.h>

/*Users must standardize methods for their data structures. This is done by calling methods 
to user-defined data structures. Standardized methods must be named in the following format: 

methodType_dataStructureType_constructingStruct

methodType:
"make" - creates the data structure on the *heap* 
"destroy" - removes the data structure on the *heap* 
"add" - adds an element to the data structure 
"remove" - removes an element from the data structure 

dataStructureType: 
The following data structures are currently supported for visualization: "queue", "stack", "vector", "array".
This list may be appended in the future to expand functionality. 

constructingStruct: 
This refers to the specific name of the datastructure. For example, if a user creates a queue and names the struct 
as "queueMod", constructingStruct will be "queueMod". This is necessary for cases in which the user defines more than 
one implementation of a specific data structure. 
*/
typedef struct queue queue; 
typedef struct stack stack; 
typedef struct vector vector; 
typedef struct dictionary dictionary;

//make methods 
void * make_queue_queueMod(size_t maxSize);
void * make_stack_stackMod(size_t maxSize);
void * make_vector_vectorMod(size_t maxSize);
void * make_array_arrayMod(size_t maxSize);
void * make_dictionary_dictionaryMod(size_t maxSize);

//destroy methods 
void destroy_queue_queueMod(queue* q);
void destroy_stack_stackMod(stack* s);
void destroy_vector_vectorMod(vector_t* v);
void destroy_array_arrayMod(char* a);
void destroy_dictionary_dictionaryMod(dictionary_t * dict);

//add methods 
void add_queue_queueMod(queue* q, void* elem);
void add_stack_stackMod(struct stack* s, void* elem);
void add_vector_vectorMod(vector_t* v, void* elem);
void add_array_arrayMod(char* a);
void add_dictionary_dictionaryMod(dictionary_t *d, char *key, void *value);

//remove methods 
void * remove_queue_queueMod(queue* q);
void * remove_stack_stackMod(struct stack* s, void* elem);
void * remove_vector_vectorMod(vector_t* v);
void * remove_array_arrayMod(char* a);
void * remove_dictionary_dictionaryMod(dictionary_t *d, char *key);
