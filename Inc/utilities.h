#ifndef UTILITIES_H
#define UTILITIES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

int is_near(int32_t p_x, int32_t p_y, int32_t mouse_x, int32_t mouse_y);
void draw_circle_bresenham(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, int32_t thickness);
void draw_dotted_line(SDL_Renderer *renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t thickness);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, int x, int y);
void mouse_world_coordinates(int mouse_screen_x, int mouse_screen_y, int *mouse_world_x, int *mouse_world_y, int pan_x, int pan_y, float zoom, int w, int h);

#endif // UTILITIES_H