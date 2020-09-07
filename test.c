#include <stdlib.h>
#include <stdio.h>
#include "creator_methods.h"

void* malloc_method();

int main(int argc, char** argv) {
    queue* q1 = (queue*) make_queue_queueMod(10); 
    void ** a1 = make_array_arrayMod(5);  
    add_queue_queueMod(q1,a1); //Note that as per implementation, you cannot have spaces between argument  
    destroy_queue_queueMod(q1); 
    //make_array_arrayMod allocates memory on the heap, but was not destroyed --> memory error
}

