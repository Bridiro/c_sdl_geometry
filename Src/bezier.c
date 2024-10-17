#include "bezier.h"

/**
 * @brief Create a new bezier curve
 * @return bezier_s New bezier curve
 */
bezier_s new_bezier()
{
    BezierPoint P0 = {0, 100, 400};
    BezierPoint P1 = {1, 320, 50};
    BezierPoint P2 = {2, 540, 400};

    BezierPoint *points = malloc(3 * sizeof(BezierPoint));
    points[0] = P0;
    points[1] = P1;
    points[2] = P2;
    return (bezier_s){
        .num = 3,
        .points = points};
}

/**
 * @brief Add a segment to the bezier curve
 * @param self Bezier curve
 * @param renderer SDL renderer
 */
void bezier_add(bezier_s *self, SDL_Renderer *renderer)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    self->num += 2;
    self->points = realloc(self->points, self->num * sizeof(BezierPoint));
    int diff_x = self->points[self->num - 3].x - self->points[self->num - 4].x;
    int diff_y = self->points[self->num - 3].y - self->points[self->num - 4].y;
    int pos_control_x = self->points[self->num - 3].x + diff_x;
    int pos_control_y = self->points[self->num - 3].y + diff_y;
    while (pos_control_x < 5 || pos_control_x > w - 5 || pos_control_y < 5 || pos_control_y > h - 5)
    {
        diff_x /= 2;
        diff_y /= 2;
        pos_control_x = self->points[self->num - 3].x + diff_x;
        pos_control_y = self->points[self->num - 3].y + diff_y;
    }
    self->points[self->num - 2] = (BezierPoint){self->num - 1, pos_control_x, pos_control_y};
    self->points[self->num - 1] = (BezierPoint){self->num - 2, 370, 240};
}

/**
 * @brief Remove a segment from the bezier curve
 * @param self Bezier curve
 */
void bezier_remove(bezier_s *self)
{
    if (self->num > 4)
    {
        self->num -= 2;
        self->points = realloc(self->points, self->num * sizeof(BezierPoint));
    }
}

/**
 * @brief Draw the bezier curve
 * @param self Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the curve
 * @param thickness Thickness of the curve
 */
void bezier_draw_curve(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t thickness)
{
    for (float t = 0; t <= 1; t += 0.0001)
    {
        for (int i = 0; i < self->num - 2; i += 2)
        {
            BezierPoint p = calculate_bezier(self->points[i], self->points[i + 1], self->points[i + 2], t);
            float dist_x = p.x - trunc(p.x);
            float dist_y = p.y - trunc(p.y);
            int alpha = (int)(255 * (1 - 2 * sqrt(dist_x * dist_x + dist_y * dist_y)));
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
            for (int dx = -thickness / 2; dx <= thickness / 2; dx++)
            {
                for (int dy = -thickness / 2; dy <= thickness / 2; dy++)
                {
                    SDL_RenderDrawPoint(renderer, (int)p.x + dx, (int)p.y + dy);
                }
            }
        }
    }
}

/**
 * @brief Draw the lines between the control points
 * @param self Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the lines
 * @param thickness Thickness of the lines
 */
void bezier_draw_lines(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t thickness)
{
    for (int i = 0; i < self->num - 1; i++)
    {
        draw_dotted_line(renderer, self->points[i].x, self->points[i].y, self->points[i + 1].x, self->points[i + 1].y, thickness);
    }
}

/**
 * @brief Draw the control points
 * @param self Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the points
 * @param radius Radius of the points
 * @param thickness Thickness of the points
 */
void bezier_draw_points(bezier_s *self, SDL_Renderer *renderer, SDL_Color color, int32_t radius, int32_t thickness)
{
    for (int i = 0; i < self->num; i++)
    {
        draw_circle_bresenham(renderer, self->points[i].x, self->points[i].y, radius, thickness);
    }
}

/**
 * @brief Calculate a point in the bezier curve
 * @param P0 First control point
 * @param P1 Second control point
 * @param P2 Third control point
 * @param t Parameter
 * @return BezierPoint Point in the curve
 */
BezierPoint calculate_bezier(BezierPoint P0, BezierPoint P1, BezierPoint P2, float t)
{
    BezierPoint result;
    float u = 1 - t;
    result.x = u * u * P0.x + 2 * u * t * P1.x + t * t * P2.x;
    result.y = u * u * P0.y + 2 * u * t * P1.y + t * t * P2.y;
    return result;
}