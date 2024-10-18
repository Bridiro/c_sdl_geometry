#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "bezier.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *Poppins = TTF_OpenFont("Misc/Poppins-Regular.ttf", 15);
    SDL_Color White = {255, 255, 255};
    SDL_Window *window = SDL_CreateWindow("Bezier Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    float zoom = 1;
    int pan_x = 0;
    int pan_y = 0;

    bezier_s bez = new_bezier();

    int lines_on = 1;
    int points_on = 1;
    int selected_point = -1;

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {

        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetRenderTarget(renderer, texture);
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
            {
                quit = 1;
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                int mouse_x = e.button.x;
                int mouse_y = e.button.y;

                for (int i = 0; i < bez.num; i++)
                {
                    if (is_near(bez.points[i].x, bez.points[i].y, mouse_x, mouse_y))
                    {
                        selected_point = i;
                        break;
                    }
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                selected_point = -1;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                if (selected_point != -1)
                {
                    bez.points[selected_point].x = e.motion.x;
                    bez.points[selected_point].y = e.motion.y;
                    int w, h;
                    SDL_GetRendererOutputSize(renderer, &w, &h);
                    if (bez.points[selected_point].x > w - 5)
                    {
                        bez.points[selected_point].x = w - 5;
                    }
                    else if (bez.points[selected_point].x < 5)
                    {
                        bez.points[selected_point].x = 5;
                    }
                    if (bez.points[selected_point].y > h - 5)
                    {
                        bez.points[selected_point].y = h - 5;
                    }
                    else if (bez.points[selected_point].y < 5)
                    {
                        bez.points[selected_point].y = 5;
                    }
                }
                else if (e.motion.state & SDL_BUTTON_LMASK)
                {
                    pan_x += e.motion.xrel;
                    pan_y += e.motion.yrel;
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_a:
                {
                    bezier_add(&bez, renderer);
                    break;
                }
                case SDLK_r:
                {
                    bezier_remove(&bez);
                    break;
                }
                case SDLK_l:
                {
                    lines_on = !lines_on;
                    break;
                }
                case SDLK_p:
                {
                    points_on = !points_on;
                    break;
                }
                case SDLK_PLUS:
                {
                    zoom += 0.1;
                    break;
                }
                case SDLK_MINUS:
                {
                    zoom -= 0.1;
                    break;
                }
                case SDLK_ESCAPE:
                {
                    zoom = 1;
                    pan_x = 0;
                    pan_y = 0;
                    bez = new_bezier();
                    break;
                }
                break;
                }
            }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            bezier_draw_curve(&bez, renderer, White, 1);
            if (lines_on)
            {
                bezier_draw_lines(&bez, renderer, White, 1);
            }
            if (points_on)
            {
                bezier_draw_points(&bez, renderer, White, 5, 2);
            }
            int w, h;
            SDL_GetRendererOutputSize(renderer, &w, &h);
            SDL_SetRenderTarget(renderer, NULL);
            SDL_Rect dest_rect;
            dest_rect.w = (int)(w * zoom);
            dest_rect.h = (int)(h * zoom);
            dest_rect.x = (w - dest_rect.w) / 2 + pan_x;
            dest_rect.y = (h - dest_rect.h) / 2 + pan_y;

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
            draw_text(renderer, Poppins, White, "p -> toggle points | l -> toggle lines | a -> add 2 points | r -> remove 2 points", 10, h - 45);
            draw_text(renderer, Poppins, White, "esc -> reset | +/- -> zoom", 10, h - 30);
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(bez.points);

    return 0;
}
