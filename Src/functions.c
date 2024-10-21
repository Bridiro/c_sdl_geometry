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

/**
 * @brief Create a new parabola
 * @param a The coefficient of x^2
 * @param b The coefficient of x
 * @param c The constant term
 * @return parabola_s
 */
parabola_s parabola_new(float a, float b, float c)
{
    return (parabola_s){a, b, c};
}

/**
 * @brief Draw a parabola
 * @param parabola The parabola to draw
 * @param renderer The renderer
 * @param color The color of the parabola
 * @param w The width of the window
 * @param h The height of the window
 * @param zoom The zoom level
 * @param pan_x The x pan
 * @param pan_y The y pan
 * @param grid_step The distance between the grid lines
 */
void parabola_draw(parabola_s *parabola, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y, int32_t grid_step)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

    float origin_x = w / 2.0f * zoom + pan_x;
    float origin_y = h / 2.0f * zoom + pan_y;

    float x_start_virtual = (-origin_x) / zoom;
    float x_end_virtual = (w - origin_x) / zoom;

    // Adjust coefficients for grid step
    float adjusted_a = parabola->a;
    float adjusted_b = parabola->b;
    float adjusted_c = parabola->c * grid_step;

    // Initialize the first points for the line
    float x_prev = x_start_virtual;
    float y_prev = adjusted_a * x_prev * x_prev + adjusted_b * x_prev + adjusted_c;
    int x_screen_prev = (int)(origin_x + x_prev * zoom);
    int y_screen_prev = (int)(origin_y - y_prev * zoom);

    for (float x = x_start_virtual + 0.1f; x < x_end_virtual; x += 0.1f)
    {
        float y = adjusted_a * x * x + adjusted_b * x + adjusted_c;

        int x_screen = (int)(origin_x + x * zoom);
        int y_screen = (int)(origin_y - y * zoom);

        // Draw a line between the previous point and the current point
        SDL_RenderDrawLine(renderer, x_screen_prev, y_screen_prev, x_screen, y_screen);

        // Update the previous point
        x_screen_prev = x_screen;
        y_screen_prev = y_screen;
    }
}
