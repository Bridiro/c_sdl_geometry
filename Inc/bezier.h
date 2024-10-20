#ifndef BEZIER_H
#define BEZIER_H

#include "utilities.h"
#include <stdbool.h>

typedef struct
{
    int32_t id;
    float x;
    float y;
} bezier_point_s;

typedef struct
{
    int32_t num;
    bezier_point_s *points;
    int32_t selected_point;
    bool lines_on;
    bool points_on;
} bezier_s;

bezier_s bezier_new();
void bezier_add(bezier_s *bez, SDL_Renderer *renderer);
void bezier_remove(bezier_s *bez);
void bezier_draw(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t c_thickness, int8_t l_thickness, int8_t p_radius, int8_t p_thickness, float zoom, int32_t pan_x, int32_t pan_y);
void bezier_draw_curve(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t thickness, float zoom, int32_t pan_x, int32_t pan_y);
void bezier_draw_lines(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t thickness, float zoom, int32_t pan_x, int32_t pan_y);
void bezier_draw_points(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t radius, int8_t thickness, float zoom, int32_t pan_x, int32_t pan_y);
bezier_point_s bezier_calculate(bezier_point_s P0, bezier_point_s P1, bezier_point_s P2, float t, float zoom, int32_t pan_x, int32_t pan_y);
void bezier_select_point(bezier_s *bez, int32_t mouse_x, int32_t mouse_y, float zoom, int32_t pan_x, int32_t pan_y);
void bezier_move_point(bezier_s *bez, int32_t mouse_x, int32_t mouse_y, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y);

#endif // BEZIER_H