#include "utilities.h"

/**
 * @brief Check if a point is near the mouse pointer
 * @param p bezier_point_s to check
 * @param mouse_x X coordinate of the mouse pointer
 * @param mouse_y Y coordinate of the mouse pointer
 * @return 1 if the point is near the mouse pointer, 0 otherwise
 */
int is_near(int32_t p_x, int32_t p_y, int32_t mouse_x, int32_t mouse_y)
{
    int8_t threshold = 15;
    return (abs(p_x - mouse_x) < threshold && abs(p_y - mouse_y) < threshold);
}

/**
 * @brief Draw a circle using Bresenham's algorithm.
 * @param renderer Renderer to draw the circle
 * @param centreX X coordinate of the centre of the circle
 * @param centreY Y coordinate of the centre of the circle
 * @param radius Radius of the circle
 * @param thickness Thickness of the circle line
 */
void draw_circle_bresenham(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius, int32_t thickness)
{
    int32_t x = radius;
    int32_t y = 0;
    int32_t p = 1 - radius;

    while (x >= y)
    {
        for (int i = -thickness / 2; i <= thickness / 2; i++)
        {
            for (int j = -thickness / 2; j <= thickness / 2; j++)
            {
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

        if (p <= 0)
        {
            p = p + 2 * y + 1;
        }
        else
        {
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
 * @param thickness Thickness of the line
 */
void draw_dotted_line(SDL_Renderer *renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t thickness)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float x_inc = (float)dx / steps;
    float y_inc = (float)dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i < steps; i++)
    {
        if (i % 10 < 5)
        {
            for (int j = -thickness / 2; j <= thickness / 2; j++)
            {
                for (int k = -thickness / 2; k <= thickness / 2; k++)
                {
                    float dist_x = (x + j) - trunc(x + j);
                    float dist_y = (y + k) - trunc(y + k);
                    int alpha = (int)(255 * (1 - sqrt(dist_x * dist_x + dist_y * dist_y)));
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
                    SDL_RenderDrawPoint(renderer, (int)x + j, (int)y + k);
                }
            }
        }
        x += x_inc;
        y += y_inc;
    }
}

/**
 * @brief Draw text on the screen
 * @param renderer Renderer to draw the text
 * @param font Font to use
 * @param text Text to draw
 * @param x X coordinate of the text
 * @param y Y coordinate of the text
 */
void draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, const char *text, int x, int y)
{
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    int text_width = surfaceMessage->w;
    int text_height = surfaceMessage->h;
    Message_rect.w = text_width;
    Message_rect.h = text_height;
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}