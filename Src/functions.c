#include "functions.h"

/**
 * @brief Create a new line
 * @param m The slope
 * @param q The y-intercept
 * @return line_s
 */
line_s line_new(float m, float q)
{
    return (line_s){m, q};
}

/**
 * @brief Draw a line
 * @param line The line to draw
 * @param renderer The renderer
 * @param color The color of the line
 * @param w The width of the window
 * @param h The height of the window
 * @param zoom The zoom level
 * @param pan_x The x pan
 * @param pan_y The y pan
 * @param grid_step The distance between the grid lines
 */
void line_draw(line_s *line, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y, int32_t grid_step)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    float origin_x = w / 2.0f * zoom + pan_x;
    float origin_y = h / 2.0f * zoom + pan_y;

    float x_start_virtual = (-origin_x) / zoom;
    float x_end_virtual = (w - origin_x) / zoom;

    float adjusted_q = line->q * grid_step;

    float y_start_virtual = line->m * x_start_virtual + adjusted_q;
    float y_end_virtual = line->m * x_end_virtual + adjusted_q;

    int start_x_screen = 0;
    int end_x_screen = w;

    int y_start_screen = (int)(origin_y - y_start_virtual * zoom);
    int y_end_screen = (int)(origin_y - y_end_virtual * zoom);

    SDL_RenderDrawLine(renderer, start_x_screen, y_start_screen, end_x_screen, y_end_screen);
}
