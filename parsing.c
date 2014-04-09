#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

// Fake add_history command
void add_history(char* unused) {}

// Otherwise include the editline headers
#else

#include <editline/readline.h>
#include <editline/history.h>

#endif

int main(int argc, char** argv) {

// Create parsers
mpc_parser_t* Number = mpc.new("number");
mpc_parser_t* Operator = mpc.new("operator");
mpc_parser_t* Expr = mpc.new("expr");
mpc_parser_t* Lispy = mpc.new("lispy");

// Define them
mpca_lang(MPC_LANG_DEFAULT,
    "
      number : /-?[0-9]+/ ;                          \
      operator : '+' | '-' | '*' | '/' ;             \
      expr : <number> | '(' <operator> <expr>+ ')' ; \
      lispy : /^/ <operator> <expr>+ /$/ ;           \

    ",
    Number, Operator, Expr, Lispy);

  // Print version and exit information
  puts("Lispy Version 0.0.0.0.1");
  puts("Press Ctrl+C to exit\n");

  // In a never ending loop
  while (1) {
    // Output prompt and get input
    char* input = readline("darkLisp> ");

    // Add input to history
    add_history(input);

    // Echo input back to user
    printf("wingardium leviosa %s\n", input);

    // Free retrieved input
    free(input);
  }

  return 0;
}
