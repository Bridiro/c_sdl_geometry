#include "text.h"

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