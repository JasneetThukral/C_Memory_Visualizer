#include <stdio.h>
#include <stdlib.h>
#include "stack_mod.h"

/**
 * A stack.
 *
 * Pop, push, peek, and maybe is_empty.
 *
 * Resize as necessary (define initial size of stack? yes if it's an
 * array, but no if you're dynamically allocating memory, which is probably
 * what you should do - so a singly linked list).
 */

Stack * create_stack(void) {
  Stack *stack = (Stack *) malloc(sizeof(Stack));
  stack->head = NULL;
  stack->next = NULL;

  return stack;
}

BOOL is_empty(Stack *stack) {
  if (size(stack) > 0) {
    return FALSE;
  }
  return TRUE;
}

int size(Stack *stack) {
  int count = 0;
  Node *ptr = stack->next;
  while (ptr != NULL) {
    ptr = ptr->next;
    count++;
  }
  return count;
}

void push(Stack *stack, void* value) {
  Node *node = (Node *) malloc(sizeof(Node));
  node->value = value;
  node->next = NULL;

  if (stack->head == NULL) {
    stack->next = node;
  } else {
    stack->head->next = node;
  }
  stack->head = node;
}

Stack * pop(Stack *stack) {
  Node *node = stack->head;
  if (1 == size(stack)) {
    stack->next = NULL;
    stack->head = NULL;
  } else if (stack->next != NULL) {
    Node *ptr = stack->next;
    while (ptr->next->next != NULL) {
      ptr = ptr->next;
    }
    stack->head = ptr;
    stack->head->next = NULL;
  }
  return stack;
}

void* peek(Stack *stack) {
  if (0 < size(stack)) {
    return stack->head->value;
  }
  return 0;
}

void destroy_stack(Stack *stack) {
  Node *ptr = stack->next;

  while (ptr != NULL) {
    stack->next = ptr->next;
    free(ptr);
    ptr = stack->next;
  }

  free(stack);
}