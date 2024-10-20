#ifndef CARTESIAN_GRAPH_H
#define CARTESIAN_GRAPH_H

#include <SDL2/SDL.h>
#include "functions.h"

typedef struct
{
    int32_t grid_distance;
    SDL_Color grid_color;
    SDL_Color axis_color;
} cartesian_graph_s;

cartesian_graph_s cartesian_graph_new(int32_t grid_distance, SDL_Color grid_color, SDL_Color axis_color);
void cartesian_graph_draw(cartesian_graph_s *cartesian_graph, SDL_Renderer *renderer, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y);

#endif // CARTESIAN_GRAPH_H