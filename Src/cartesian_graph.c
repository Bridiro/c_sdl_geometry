#include "cartesian_graph.h"

/**
 * @brief Create a new cartesian graph
 * @param grid_distance The distance between the grid lines
 * @param grid_color The color of the grid lines
 * @param axis_color The color of the axis
 * @return cartesian_graph_s
 */
cartesian_graph_s cartesian_graph_new(int32_t grid_distance, SDL_Color grid_color, SDL_Color axis_color)
{
    return (cartesian_graph_s){grid_distance, grid_color, axis_color, NULL, NULL, NONE};
}

/**
 * @brief Draw the cartesian graph
 * @param cartesian_graph The cartesian graph to draw
 * @param renderer The renderer
 * @param w The width of the window
 * @param h The height of the window
 * @param zoom The zoom level
 * @param pan_x The x pan
 * @param pan_y The y pan
 */
void cartesian_graph_draw(cartesian_graph_s *cartesian_graph, SDL_Renderer *renderer, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y)
{
    SDL_SetRenderDrawColor(renderer, cartesian_graph->grid_color.r, cartesian_graph->grid_color.g, cartesian_graph->grid_color.b, 100);

    int32_t start_x = -pan_x / zoom;
    int32_t start_y = -pan_y / zoom;
    int32_t end_x = (w - pan_x) / zoom;
    int32_t end_y = (h - pan_y) / zoom;

    for (int32_t x = start_x; x <= end_x; x++)
    {
        if (x % cartesian_graph->grid_distance == 0)
        {
            int32_t screen_x = (int32_t)(x * zoom + pan_x);
            SDL_RenderDrawLine(renderer, screen_x, 0, screen_x, h);
        }
    }
    for (int32_t y = start_y; y <= end_y; y++)
    {
        if (y % cartesian_graph->grid_distance == 0)
        {
            int32_t screen_y = (int32_t)(y * zoom + pan_y);
            SDL_RenderDrawLine(renderer, 0, screen_y, w, screen_y);
        }
    }

    SDL_SetRenderDrawColor(renderer, cartesian_graph->axis_color.r, cartesian_graph->axis_color.g, cartesian_graph->axis_color.b, 255);
    SDL_RenderDrawLine(renderer, 0, h / 2 * zoom + pan_y, w, h / 2 * zoom + pan_y);
    SDL_RenderDrawLine(renderer, w / 2 * zoom + pan_x, 0, w / 2 * zoom + pan_x, h);
}

/**
 * @brief Draw the cartesian graph equation result
 * @param cartesian_graph The cartesian graph to draw
 * @param renderer The renderer
 * @param w The width of the window
 * @param h The height of the window
 * @param zoom The zoom level
 * @param pan_x The x pan
 * @param pan_y The y pan
 */
void cartesian_graph_draw_equation_result(cartesian_graph_s *cartesian_graph, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y)
{
    cartesian_graph->type = eq_parser_get_type(cartesian_graph->equation);
    switch (cartesian_graph->type)
    {
    case STRAIGHT:
    {
        float m, q;
        eq_parser_get_straight_coefficients(cartesian_graph->equation, &m, &q);
        line_s line = line_new(m, q);
        line_draw(&line, renderer, color, w, h, zoom, pan_x, pan_y, cartesian_graph->grid_distance);
        break;
    }
    case PARABOLA:
    {
        break;
    }
    default:
        break;
    }
}