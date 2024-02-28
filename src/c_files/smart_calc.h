#ifndef SMART_CALCH
#define SMART_CALCH

#define SUCCESS 1
#define ERROR 0
#define ERROR_DIV_BY_ZERO -2
#define ERROR_NOT_CORRECT_SYMBOLE -3
#define ERROR_MANY_DOTS -4
#define ERROR_NOT_FUNCTION -5
#define ERROR_BRACKET -6
#define ERROR_NAN -7
#define ERROR_STACK_NUMBER -8
#define ERROR_SMALL_STACK -9

#define STACK_OVERFLOW -100
#define STACK_UNDERFLOW -101
#define OUT_OF_MEMORY -102

#define COS 100
#define SIN 101
#define TAN 102
#define ACOS 103
#define ASIN 104
#define ATAN 105
#define SQRT 106
#define LOG 107
#define LN 108

#define MOD 109

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_tag {
  double value;
  struct Node_tag* next;
} node_t;

void push(node_t** head, double value);
double pop(node_t** head);
double peek(const node_t* head);
void printStack(const node_t* head);
size_t getSize(const node_t* head);
void stack_free(node_t** head);

char ft_get_prior(char symb);
int ft_func(char** pt_in, node_t** stack_polish);
int ft_is_operation(char symb);
char ft_is_function_before_polish(char symb);
char ft_is_function_after_polish(char symb);
int ft_is_number(char* symb);
int ft_is_unar_minus(char* pt_in, char first_symbol);

int ft_polish_number(char** pt_in, char** pt_polish);
void ft_polish_operation(char symb, node_t** stack_polish, char** pt_in,
                         char** pt_polish);
void ft_handling_unar_minus(char** pt_polish, node_t** stack_polish, char symb);
int ft_while_begin_bracket(char** pt_polish, node_t** stack_polish, char* symb);

double ft_calcul_arithmetic(char operation, double operand1, double operand2);
char ft_calcul_function(char operation_function, node_t** stack_calculation);

double ft_calcul_result_function(char operation, double operand);
int ft_calcul_operation(char operation, node_t** stack_calculation);

int ft_from_stack_to_polish(char** pt_polish, node_t** stack_polish,
                            char* symb);
void ft_number_to_stack(char** pt_polish, node_t** stack_calculation);

int ft_from_infix_to_postfix(char* pt_in, char* pt_polish);
int ft_calcul_polish(char* pt_polish, double* result);

int trash();

#endif
