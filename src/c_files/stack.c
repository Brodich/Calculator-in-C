#include "smart_calc.h"

/// @brief
/// @param head stack
/// @param value value, that push to stack
void push(node_t **head, double value) {
  node_t *tmp = (node_t *)malloc(sizeof(node_t));
  tmp->next = *head;
  tmp->value = value;
  *head = tmp;
}

/// @brief
/// @param head stack
/// @return value, that get from stack
double pop(node_t **head) {
  node_t *out;
  double value;
  if (*head == NULL) {
    return (STACK_UNDERFLOW);
  }
  out = *head;
  *head = (*head)->next;
  value = out->value;
  free(out);
  return value;
}

/// @brief
/// @param head stack
/// @return top value store in stack
double peek(const node_t *head) {
  if (head == NULL) {
    return (STACK_UNDERFLOW);
  }
  return head->value;
}

// void printStack(const node_t *head) {
//   printf("stack >");
//   while (head) {
//     printf("%f ", head->value);
//     head = head->next;
//   }
//   printf("\n");
// }

/// @brief
/// @param head stack
/// @return size of stack
size_t getSize(const node_t *head) {
  size_t size = 0;
  while (head) {
    size++;
    head = head->next;
  }
  return size;
}

/// @brief free all memory that store in stack
/// @param head stack
void stack_free(node_t **head) {
  node_t *out;
  while (*head) {
    out = *head;
    *head = (*head)->next;

    free(out);
  }
}
