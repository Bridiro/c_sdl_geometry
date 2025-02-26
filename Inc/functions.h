#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL.h>

typedef struct
{
    float m;
    float q;
} line_s;

typedef struct
{
    float a;
    float b;
    float c;
} parabola_s;

line_s line_new(float m, float q);
void line_draw(line_s *line, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y, int32_t grid_step);
parabola_s parabola_new(float a, float b, float c);
void parabola_draw(parabola_s *parabola, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y, int32_t grid_step);

#endif // FUNCTIONS_H
