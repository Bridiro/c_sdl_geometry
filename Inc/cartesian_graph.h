#ifndef CARTESIAN_GRAPH_H
#define CARTESIAN_GRAPH_H

#include <SDL.h>
#include "functions.h"
#include "eq_parser.h"

typedef struct
{
    int32_t grid_distance;
    SDL_Color grid_color;
    SDL_Color axis_color;
    char *equation;
    char *partial_equation;
    cartesian_graph_type_e type;
} cartesian_graph_s;

cartesian_graph_s cartesian_graph_new(int32_t grid_distance, SDL_Color grid_color, SDL_Color axis_color);
void cartesian_graph_draw(cartesian_graph_s *cartesian_graph, SDL_Renderer *renderer, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y);
void cartesian_graph_draw_equation_result(cartesian_graph_s *cartesian_graph, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y);

#endif // CARTESIAN_GRAPH_H
