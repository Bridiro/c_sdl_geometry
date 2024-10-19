#ifndef BEZIER_H
#define BEZIER_H

#include "utilities.h"

typedef struct
{
    int id;
    float x;
    float y;
} BezierPoint;

typedef struct
{
    int num;
    BezierPoint *points;
} bezier_s;

bezier_s new_bezier();
void bezier_add(bezier_s *self, SDL_Renderer *renderer);
void bezier_remove(bezier_s *self);
void bezier_draw_curve(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t thickness);
void bezier_draw_lines(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t thickness);
void bezier_draw_points(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t radius, int32_t thickness);
BezierPoint calculate_bezier(BezierPoint P0, BezierPoint P1, BezierPoint P2, float t);
int select_point(bezier_s *bez, int mouse_x, int mouse_y);
void move_point(bezier_s *bez, int selected_point, int mouse_x, int mouse_y, int w, int h);

#endif // BEZIER_H