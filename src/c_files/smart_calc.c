
#include "smart_calc.h"

/// @brief
/// @param pt_polish input string in polish notation
/// @param result calculation polish notation
/// @return code error, if SUCCESS then all ok
int ft_calcul_polish(char* pt_polish, double* result) {
  node_t* stack_calculation = NULL;
  int code = SUCCESS;
  while (*pt_polish != '\0' && code == SUCCESS) {
    if (ft_is_number(pt_polish)) {
      ft_number_to_stack(&pt_polish, &stack_calculation);
    } else if (ft_is_operation(*pt_polish)) {
      code = ft_calcul_operation(*pt_polish, &stack_calculation);
    } else if (ft_is_function_after_polish(*pt_polish)) {
      code = ft_calcul_function(*pt_polish, &stack_calculation);
    }
    pt_polish++;
  }
  if (code == SUCCESS) {
    if (getSize(stack_calculation) > 1) {
      code = ERROR_STACK_NUMBER;
    }
    if (code == SUCCESS) *result = pop(&stack_calculation);
  }
  stack_free(&stack_calculation);
  return (code);
}

/// @brief
/// @param pt_in input string in infix form
/// @param pt_polish output string in polish notation
/// @return code error, if SUCCESS then all ok
int ft_from_infix_to_postfix(char* pt_in, char* pt_polish) {
  node_t* stack_polish = NULL;
  char symb = 0;
  char first_symbol = 1;
  int code = SUCCESS;
  while (*pt_in != '\0' && code == SUCCESS) {
    symb = *pt_in;
    if (ft_is_unar_minus(pt_in, first_symbol)) {
      ft_handling_unar_minus(&pt_polish, &stack_polish, symb);
    } else if ((symb >= '0' && symb <= '9') || symb == '.') {
      code = ft_polish_number(&pt_in, &pt_polish);
    } else if (symb == '(') {
      push(&stack_polish, symb);
    } else if (ft_is_function_before_polish(symb)) {
      code = ft_func(&pt_in, &stack_polish);
    } else if (ft_is_operation(symb)) {
      ft_polish_operation(symb, &stack_polish, &pt_in, &pt_polish);
    } else if (symb == ')') {
      code = ft_while_begin_bracket(&pt_polish, &stack_polish, &symb);
    } else if (symb == ' ') {
    } else {
      code = ERROR_NOT_CORRECT_SYMBOLE;
    }
    first_symbol = 0;
    pt_in++;
  }
  if (code == SUCCESS)
    code = ft_from_stack_to_polish(&pt_polish, &stack_polish, &symb);
  stack_free(&stack_polish);
  return (code);
}

/// @brief
/// @param symb symbol of input string
/// @param stack_polish stack that store operations
/// @param pt_in input string in infix form
/// @param pt_polish output string in polish notation
void ft_polish_operation(char symb, node_t** stack_polish, char** pt_in,
                         char** pt_polish) {
  char prior_in_symb = ft_get_prior(symb);
  char prior_stack_symb = ft_get_prior(peek(*stack_polish));
  while (prior_stack_symb >= prior_in_symb && symb != '^') {
    if (prior_stack_symb >= prior_in_symb) {
      **pt_polish = pop(stack_polish);
      (*pt_polish)++;
      **pt_polish = ' ';
      (*pt_polish)++;
    }
    prior_in_symb = ft_get_prior(symb);
    prior_stack_symb = ft_get_prior(peek(*stack_polish));
  }
  push(stack_polish, symb);
  if ('m' == symb) {
    (*pt_in)++;
    (*pt_in)++;
  }
}

/// @brief
/// @param symb symbol of input string
/// @param pt_in input string in infix form
/// @param pt_polish output string in polish notation
/// @return code error, if SUCCESS then all ok
int ft_polish_number(char** pt_in, char** pt_polish) {
  int dot = 0;
  if (**pt_in == '.') {
    dot++;
    **pt_polish = '0';
    (*pt_polish)++;
    **pt_polish = '.';
    (*pt_polish)++;
    (*pt_in)++;
  }
  while ((**pt_in >= '0' && **pt_in <= '9') || (**pt_in == '.')) {
    if (**pt_in == '.') dot++;
    **pt_polish = **pt_in;
    (*pt_polish)++;
    (*pt_in)++;
  }
  (*pt_in)--;
  **pt_polish = ' ';
  (*pt_polish)++;
  if (dot > 1) return (ERROR_MANY_DOTS);
  return (SUCCESS);
}

/// @brief
/// @param symb symbol of operations
/// @return priority
char ft_get_prior(char symb) {
  char res;
  if (ft_is_function_before_polish(symb) == 1 ||
      ft_is_function_after_polish(symb) == 1) {
    res = 5;
  } else if (symb == '^') {
    res = 4;
  } else if (symb == '*' || symb == '/' || symb == 'm') {
    res = 3;
  } else if (symb == '+' || symb == '-') {
    res = 1;
  } else {
    res = -1;
  }
  return (res);
}

/// @brief
/// @param symb symbol input string
/// @return 1 if operation, 0 if not
int ft_is_operation(char symb) {
  int res = 0;
  if (symb == '+' || symb == '-' || symb == '*' || symb == '/' || symb == '^' ||
      symb == 'm') {  // ?? symb == 'm'
    res = 1;
  } else {
    res = 0;
  }
  return (res);
}

/// @brief compare with begin funtion
/// @param symb symbol input string
/// @return 1 if this function
char ft_is_function_before_polish(char symb) {
  char res = 0;
  if (symb == 'c' || symb == 's' || symb == 't' || symb == 'a' || symb == 'l') {
    res = 1;
  }
  return (res);
}

/// @brief compare with begin funtion after replace macros
/// @param symb symbol after converting in macros
/// @return 1 if this function
char ft_is_function_after_polish(char symb) {
  char res = 0;
  if (symb == COS || symb == SIN || symb == TAN || symb == ACOS ||
      symb == ASIN || symb == ATAN || symb == SQRT || symb == LN ||
      symb == LOG) {
    res = 1;
  } else {
    res = 0;
  }
  return (res);
}

/// @brief function that add function in stack in macros view
/// @param pt_in string in infix form,
/// @param stack_polish
/// @return code error, if SUCCESS then all ok
int ft_func(char** pt_in, node_t** stack_polish) {
  char code_func_and_move[3] = {0};

  if (strncmp(*pt_in, "cos", 3) == 0) {
    code_func_and_move[0] = COS;
    code_func_and_move[1] = 3;
  } else if (strncmp(*pt_in, "sin", 3) == 0) {
    code_func_and_move[0] = SIN;
    code_func_and_move[1] = 3;
  } else if (strncmp(*pt_in, "tan", 3) == 0) {
    code_func_and_move[0] = TAN;
    code_func_and_move[1] = 3;
  } else if (strncmp(*pt_in, "acos", 4) == 0) {
    code_func_and_move[0] = ACOS;
    code_func_and_move[1] = 4;
  } else if (strncmp(*pt_in, "asin", 4) == 0) {
    code_func_and_move[0] = ASIN;
    code_func_and_move[1] = 4;
  } else if (strncmp(*pt_in, "atan", 4) == 0) {
    code_func_and_move[0] = ATAN;
    code_func_and_move[1] = 4;
  } else if (strncmp(*pt_in, "sqrt", 4) == 0) {
    code_func_and_move[0] = SQRT;
    code_func_and_move[1] = 4;
  } else if (strncmp(*pt_in, "log", 3) == 0) {
    code_func_and_move[0] = LOG;
    code_func_and_move[1] = 3;
  } else if (strncmp(*pt_in, "ln", 2) == 0) {
    code_func_and_move[0] = LN;
    code_func_and_move[1] = 2;
  } else {
    return (ERROR_NOT_FUNCTION);
  }
  push(stack_polish, code_func_and_move[0]);
  while (code_func_and_move[1] != 0) {
    (*pt_in)++;
    code_func_and_move[1]--;
  }
  (*pt_in)--;

  return (SUCCESS);
}

/// @brief
/// @param operation function operation
/// @param operand number
/// @return result of operation
double ft_calcul_result_function(char operation, double operand) {
  double res = 0;
  switch (operation) {
    case COS:
      res = cos(operand);
      break;
    case SIN:
      res = sin(operand);
      break;
    case TAN:
      res = tan(operand);
      break;
    case ACOS:
      res = acos(operand);
      break;
    case ASIN:
      res = asin(operand);
      break;
    case ATAN:
      res = atan(operand);
      break;
    case SQRT:
      res = sqrt(operand);
      break;
    case LN:
      res = log(operand);
      break;
    case LOG:
      res = log10(operand);
      break;
    default:
      break;
  }
  return (res);
}

/// @brief
/// @param operation arithmetic operation
/// @param operand1 operand1
/// @param operand2 operand2
/// @return result calculated
double ft_calcul_arithmetic(char operation, double operand1, double operand2) {
  double res = 0;
  switch (operation) {
    case '+':
      res = operand1 + operand2;
      break;
    case '-':
      res = operand1 - operand2;
      break;
    case '*':
      res = operand1 * operand2;
      break;
    case '/':
      res = operand1 / operand2;
      break;
    case '^':
      res = pow(operand1, operand2);
      break;
    case 'm':
      res = fmod(operand1, operand2);
      break;
    default:
      break;
  }
  return (res);
}

/// @brief
/// @param symb
/// @return code error, if SUCCESS then all ok
int ft_is_number(char* symb) {
  if ((*symb >= '0' && *symb <= '9') ||
      (*symb == '-' && *(symb + 1) >= '0' && *(symb + 1) <= '9')) {
    return (SUCCESS);
  }
  return (ERROR);
}

int ft_is_unar_minus(char* pt_in, char first_symbol) {
  if (ft_is_operation(*(pt_in - 1)) || *(pt_in - 1) == '(' ||
      first_symbol == 1) {
    if ((*pt_in == '-' && *(pt_in + 1) >= '0' && *(pt_in + 1) <= '9')) {
      return (SUCCESS);
    }
  }
  return (ERROR);
}

/// @brief
/// @param pt_polish string in polish notation
/// @param stack_polish stack, store operation
/// @param symb minus
void ft_handling_unar_minus(char** pt_polish, node_t** stack_polish,
                            char symb) {
  **pt_polish = '0';
  (*pt_polish)++;
  **pt_polish = ' ';
  (*pt_polish)++;
  push(stack_polish, symb);
}

/// @brief
/// @param pt_polish string in polish notation
/// @param stack_polish stack, store operation
/// @param symb operation, from stack
/// @return code error, if SUCCESS then all ok
int ft_from_stack_to_polish(char** pt_polish, node_t** stack_polish,
                            char* symb) {
  while (*stack_polish != NULL) {
    *symb = pop(stack_polish);
    if (*symb == '(') {
      return (ERROR_BRACKET);
    }
    **pt_polish = *symb;
    (*pt_polish)++;
    **pt_polish = ' ';
    (*pt_polish)++;
  }
  return (SUCCESS);
}

/// @brief
/// @param pt_polish string in polish notation
/// @param stack_polish stack, store operation
/// @param symb operation, from stack
/// @return code error, if SUCCESS then all ok
int ft_while_begin_bracket(char** pt_polish, node_t** stack_polish,
                           char* symb) {
  int code = SUCCESS;
  *symb = pop(stack_polish);
  while (*symb != '(' && *stack_polish != NULL) {
    **pt_polish = *symb;
    (*pt_polish)++;
    **pt_polish = ' ';
    (*pt_polish)++;
    *symb = pop(stack_polish);
  }
  if (*symb != '(') code = ERROR_BRACKET;

  return (code);
}

/// @brief
/// @param pt_polish string in polish notation
/// @param stack_calculation stack, store numbers
void ft_number_to_stack(char** pt_polish, node_t** stack_calculation) {
  // char sign = -1;
  double number = 0;
  number = strtod(*pt_polish, pt_polish);
  // if (*((*pt_polish) - 1) == '-') {
  //   number *= sign;
  // }
  push(stack_calculation, number);
  (*pt_polish)--;
}

/// @brief
/// @param operation operation
/// @param stack_calculation stack, store numbers
/// @return code error, if SUCCESS then all ok
int ft_calcul_operation(char operation, node_t** stack_calculation) {
  double operand1 = 0;
  double operand2 = 0;
  double result = 0;

  if (getSize(*stack_calculation) <= 1) {
    return (ERROR);
  }
  operand2 = pop(stack_calculation);
  operand1 = pop(stack_calculation);
  if (operation == '/' && operand2 == 0.0) {
    return (ERROR_DIV_BY_ZERO);
  }
  result = ft_calcul_arithmetic(operation, operand1, operand2);
  push(stack_calculation, result);
  return (SUCCESS);
}

/// @brief
/// @param operation_function operation function
/// @param stack_calculation stack, store numbers
/// @return code error, if SUCCESS then all ok
char ft_calcul_function(char operation_function, node_t** stack_calculation) {
  double operand = 0;
  double result = 0;

  if (getSize(*stack_calculation) < 1) {
    return (ERROR_SMALL_STACK);
  }
  operand = pop(stack_calculation);
  result = ft_calcul_result_function(operation_function, operand);
  if (isnan(result)) {
    return (ERROR_NAN);
  }
  push(stack_calculation, result);
  return (SUCCESS);
}
