#include "bezier.h"

/**
 * @brief Create a new bezier curve
 * @return bezier_s New bezier curve
 */
bezier_s bezier_new()
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
        .points = points,
        .selected_point = -1,
        .lines_on = true,
        .points_on = true};
}

/**
 * @brief Add a segment to the bezier curve
 * @param bez Bezier curve
 * @param renderer SDL renderer
 */
void bezier_add(bezier_s *bez, SDL_Renderer *renderer)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    bez->num += 2;
    bez->points = realloc(bez->points, bez->num * sizeof(BezierPoint));
    int diff_x = bez->points[bez->num - 3].x - bez->points[bez->num - 4].x;
    int diff_y = bez->points[bez->num - 3].y - bez->points[bez->num - 4].y;
    int diff_x_half = diff_x / 2;
    int diff_y_half = diff_y / 2;
    int pos_control_x = bez->points[bez->num - 3].x + diff_x;
    int pos_control_y = bez->points[bez->num - 3].y + diff_y;
    int pos_control_x_half = bez->points[bez->num - 3].x + diff_x_half;
    int pos_control_y_half = bez->points[bez->num - 3].y + diff_y_half;
    while (pos_control_x < 5 || pos_control_x > w - 5 || pos_control_y < 5 || pos_control_y > h - 5)
    {
        diff_x /= 2;
        diff_y /= 2;
        diff_x_half /= 2;
        diff_y_half /= 2;
        pos_control_x = bez->points[bez->num - 3].x + diff_x;
        pos_control_y = bez->points[bez->num - 3].y + diff_y;
        pos_control_x_half = bez->points[bez->num - 3].x + diff_x_half;
        pos_control_y_half = bez->points[bez->num - 3].y + diff_y_half;
    }
    bez->points[bez->num - 2] = (BezierPoint){bez->num - 1, pos_control_x_half, pos_control_y_half};
    bez->points[bez->num - 1] = (BezierPoint){bez->num - 2, pos_control_x, pos_control_y};
}

/**
 * @brief Remove a segment from the bezier curve
 * @param bez Bezier curve
 */
void bezier_remove(bezier_s *bez)
{
    if (bez->num > 4)
    {
        bez->num -= 2;
        bez->points = realloc(bez->points, bez->num * sizeof(BezierPoint));
    }
}

/**
 * @brief Draw the bezier curve
 * @param bez Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the curve
 * @param c_thickness Thickness of the curve
 * @param l_thickness Thickness of the lines
 * @param p_radius Radius of the points
 * @param p_thickness Thickness of the points
 */
void bezier_draw(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t c_thickness, int8_t l_thickness, int8_t p_radius, int8_t p_thickness)
{
    bezier_draw_curve(bez, renderer, color, c_thickness);
    if (bez->lines_on)
    {
        bezier_draw_lines(bez, renderer, color, l_thickness);
    }
    if (bez->points_on)
    {
        bezier_draw_points(bez, renderer, color, p_radius, p_thickness);
    }
}

/**
 * @brief Draw the bezier curve
 * @param bez Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the curve
 * @param thickness Thickness of the curve
 */
void bezier_draw_curve(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t thickness)
{
    for (float t = 0; t <= 1; t += 0.0001)
    {
        for (int i = 0; i < bez->num - 2; i += 2)
        {
            BezierPoint p = bezier_calculate(bez->points[i], bez->points[i + 1], bez->points[i + 2], t);
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
 * @param bez Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the lines
 * @param thickness Thickness of the lines
 */
void bezier_draw_lines(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t thickness)
{
    for (int i = 0; i < bez->num - 1; i++)
    {
        draw_dotted_line(renderer, bez->points[i].x, bez->points[i].y, bez->points[i + 1].x, bez->points[i + 1].y, thickness);
    }
}

/**
 * @brief Draw the control points
 * @param bez Bezier curve
 * @param renderer SDL renderer
 * @param color Color of the points
 * @param radius Radius of the points
 * @param thickness Thickness of the points
 */
void bezier_draw_points(bezier_s *bez, SDL_Renderer *renderer, SDL_Color color, int8_t radius, int8_t thickness)
{
    for (int i = 0; i < bez->num; i++)
    {
        draw_circle_bresenham(renderer, bez->points[i].x, bez->points[i].y, radius, thickness);
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
BezierPoint bezier_calculate(BezierPoint P0, BezierPoint P1, BezierPoint P2, float t)
{
    BezierPoint result;
    float u = 1 - t;
    result.x = u * u * P0.x + 2 * u * t * P1.x + t * t * P2.x;
    result.y = u * u * P0.y + 2 * u * t * P1.y + t * t * P2.y;
    return result;
}

/**
 * @brief Select a control point
 * @param bez Bezier curve
 * @param mouse_x Mouse x position
 * @param mouse_y Mouse y position
 */
void bezier_select_point(bezier_s *bez, int32_t mouse_x, int32_t mouse_y)
{
    for (int i = 0; i < bez->num; i++)
    {
        if (is_near(bez->points[i].x, bez->points[i].y, mouse_x, mouse_y))
        {
            bez->selected_point = i;
            return;
        }
    }
}

/**
 * @brief Move a control point
 * @param bez Bezier curve
 * @param mouse_x Mouse x position
 * @param mouse_y Mouse y position
 * @param w Window width
 * @param h Window height
 */
void bezier_move_point(bezier_s *bez, int32_t mouse_x, int32_t mouse_y, int32_t w, int32_t h)
{
    bez->points[bez->selected_point].x = mouse_x;
    bez->points[bez->selected_point].y = mouse_y;
    if (bez->points[bez->selected_point].x > w - 5)
    {
        bez->points[bez->selected_point].x = w - 5;
    }
    else if (bez->points[bez->selected_point].x < 5)
    {
        bez->points[bez->selected_point].x = 5;
    }
    if (bez->points[bez->selected_point].y > h - 5)
    {
        bez->points[bez->selected_point].y = h - 5;
    }
    else if (bez->points[bez->selected_point].y < 5)
    {
        bez->points[bez->selected_point].y = 5;
    }
}