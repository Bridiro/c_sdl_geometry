#ifndef CARTESIAN_GRAPH_H
#define CARTESIAN_GRAPH_H

#include <SDL2/SDL.h>

typedef struct
{
    int32_t grid_distance;
    int32_t grid_thickness;
    SDL_Color grid_color;
    int32_t axis_thickness;
    SDL_Color axis_color;
} CartesianGraph;

#endif // CARTESIAN_GRAPH_H