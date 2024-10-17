#ifndef UTILITIES_H
#define UTILITIES_H

#include <SDL2/SDL.h>
#include <math.h>

typedef struct
{
    int id;
    float x;
    float y;
} Point;

int is_near(Point p, int32_t mouse_x, int32_t mouse_y);
void draw_circle_bresenham(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);
void draw_dotted_line(SDL_Renderer *renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif // UTILITIES_H