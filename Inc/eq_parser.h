#ifndef EQ_PARSER_H
#define EQ_PARSER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
    NONE,
    STRAIGHT,
    PARABOLA
} cartesian_graph_type_e;

cartesian_graph_type_e eq_parser_get_type(char *equation);
void eq_parser_get_straight_coefficients(char *equation, float *m, float *q);
void eq_parser_get_parabola_coefficients(char *equation, float *a, float *b, float *c);

#endif // EQ_PARSER_H