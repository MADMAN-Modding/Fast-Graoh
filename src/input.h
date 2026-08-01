#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stddef.h>

#define MAX_EQUATIONS 4
#define MAX_EQ_LEN 32

// Gets equations from user input using built in libraries
void input_GetEquations(uint8_t equations[MAX_EQUATIONS][MAX_EQ_LEN],
                         size_t eq_lengths[MAX_EQUATIONS]);

double eval_tokenized_equation(const uint8_t *eq_tokens, size_t eq_len, double x);
#endif