#include "functions.h"

line_s line_new(float m, float q)
{
    return (line_s){m, q};
}

void line_draw(line_s *line, SDL_Renderer *renderer, SDL_Color color, int32_t w, int32_t h, float zoom, int32_t pan_x, int32_t pan_y, int32_t grid_step)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    int32_t start_x = -pan_x / zoom;
    int32_t start_y = -pan_y / zoom;
    int32_t end_x = (w - pan_x) / zoom;
    int32_t end_y = (h - pan_y) / zoom;

    float adjusted_q = line->q * grid_step;
    int origin_y = h / 2 * zoom + pan_y;
    int origin_x = w / 2 * zoom + pan_x;

    float x_start_virtual = (start_x - origin_x + pan_x) / zoom;
    float x_end_virtual = (end_x - origin_x + pan_x) / zoom;

    float y_start_virtual = line->m * x_start_virtual + adjusted_q;
    float y_end_virtual = line->m * x_end_virtual + adjusted_q;

    int y_start_screen = origin_y - (int)(y_start_virtual * zoom) - pan_y;
    int y_end_screen = origin_y - (int)(y_end_virtual * zoom) - pan_y;

    SDL_RenderDrawLine(renderer, start_x + pan_x, y_start_screen + pan_y, end_x + pan_x, y_end_screen + pan_y);
}
