#include "utilities.h"

/**
 * @brief Check if a point is near the mouse pointer
 * @param p Point to check
 * @param mouse_x X coordinate of the mouse pointer
 * @param mouse_y Y coordinate of the mouse pointer
 * @return 1 if the point is near the mouse pointer, 0 otherwise
 */
int is_near(Point p, int32_t mouse_x, int32_t mouse_y) {
    int8_t threshold = 10;
    return (fabs(p.x - mouse_x) < threshold && fabs(p.y - mouse_y) < threshold);
}

/**
 * @brief Draw a circle using Bresenham's algorithm.
 * @param renderer Renderer to draw the circle
 * @param centreX X coordinate of the centre of the circle
 * @param centreY Y coordinate of the centre of the circle
 * @param radius Radius of the circle
 */
void draw_circle_bresenham(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    int32_t x = radius;
    int32_t y = 0;
    int32_t p = 1 - radius;

    while (x >= y) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                float dist_x = (centreX + x + i) - trunc(centreX + x + i);
                float dist_y = (centreY + y + j) - trunc(centreY + y + j);
                int alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX + x + i, centreY + y + j);

                dist_x = (centreX - x + i) - trunc(centreX - x + i);
                dist_y = (centreY + y + j) - trunc(centreY + y + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX - x + i, centreY + y + j);

                dist_x = (centreX + x + i) - trunc(centreX + x + i);
                dist_y = (centreY - y + j) - trunc(centreY - y + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX + x + i, centreY - y + j);

                dist_x = (centreX - x + i) - trunc(centreX - x + i);
                dist_y = (centreY - y + j) - trunc(centreY - y + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX - x + i, centreY - y + j);

                dist_x = (centreX + y + i) - trunc(centreX + y + i);
                dist_y = (centreY + x + j) - trunc(centreY + x + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX + y + i, centreY + x + j);

                dist_x = (centreX - y + i) - trunc(centreX - y + i);
                dist_y = (centreY + x + j) - trunc(centreY + x + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX - y + i, centreY + x + j);

                dist_x = (centreX + y + i) - trunc(centreX + y + i);
                dist_y = (centreY - x + j) - trunc(centreY - x + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX + y + i, centreY - x + j);

                dist_x = (centreX - y + i) - trunc(centreX - y + i);
                dist_y = (centreY - x + j) - trunc(centreY - x + j);
                alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                SDL_RenderDrawPoint(renderer, centreX - y + i, centreY - x + j);
            }
        }

        y++;

        if (p <= 0) {
            p = p + 2 * y + 1;
        } else {
            x--;
            p = p + 2 * y - 2 * x + 1;
        }
    }
}

/**
 * @brief Draw a dotted line between two points.
 * @param renderer Renderer to draw the line
 * @param x1 X coordinate of the first point
 * @param y1 Y coordinate of the first point
 * @param x2 X coordinate of the second point
 * @param y2 Y coordinate of the second point
 */
void draw_dotted_line(SDL_Renderer* renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float x_inc = (float)dx / steps;
    float y_inc = (float)dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i < steps; i++) {
        if (i % 10 < 5) {
            float dist_x = x - trunc(x);
            float dist_y = y - trunc(y);
            int alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
        }
        x += x_inc;
        y += y_inc;
    }
}