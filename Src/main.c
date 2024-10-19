#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "bezier.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *Poppins20 = TTF_OpenFont("Misc/Poppins-Regular.ttf", 20);
    TTF_Font *Poppins15 = TTF_OpenFont("Misc/Poppins-Regular.ttf", 15);
    SDL_Color White = {255, 255, 255};
    SDL_Window *window = SDL_CreateWindow("SDL Geometry", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480);

    float zoom = 1;
    int pan_x = 0;
    int pan_y = 0;

    int width = 640;
    int height = 480;

    typedef enum
    {
        BEZIER,
        STRAIGHT
    } mode_e;
    mode_e mode = BEZIER;

    bezier_s bez = bezier_new();

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        if (w != width || h != height)
        {
            width = w;
            height = h;
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
        }
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
                mouse_world_coordinates(mouse_x, mouse_y, &mouse_x, &mouse_y, pan_x, pan_y, zoom, w, h);
                bezier_select_point(&bez, mouse_x, mouse_y);
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                bez.selected_point = -1;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                if (bez.selected_point != -1)
                {
                    int mouse_x = e.motion.x;
                    int mouse_y = e.motion.y;
                    mouse_world_coordinates(mouse_x, mouse_y, &mouse_x, &mouse_y, pan_x, pan_y, zoom, w, h);
                    bezier_move_point(&bez, mouse_x, mouse_y, w, h);
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
                    if (mode == BEZIER)
                    {
                        bezier_add(&bez, renderer);
                    }
                    break;
                }
                case SDLK_r:
                {
                    if (mode == BEZIER)
                    {
                        bezier_remove(&bez);
                    }
                    break;
                }
                case SDLK_l:
                {
                    if (mode == BEZIER)
                    {
                        bez.lines_on = !bez.lines_on;
                    }
                    break;
                }
                case SDLK_p:
                {
                    if (mode == BEZIER)
                    {
                        bez.points_on = !bez.points_on;
                    }
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
                    free(bez.points);
                    bez = bezier_new();
                    break;
                }
                case SDLK_RIGHT:
                {
                    pan_x += 10;
                    break;
                }
                break;
                }
            }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (mode == BEZIER)
            {
                bezier_draw(&bez, renderer, White, 1, 1, 5, 2);
            }

            SDL_SetRenderTarget(renderer, NULL);
            SDL_Rect dest_rect = {
                .w = (int)(w * zoom),
                .h = (int)(h * zoom),
                .x = (w - dest_rect.w) / 2 + pan_x,
                .y = (h - dest_rect.h) / 2 + pan_y};
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &dest_rect);
            draw_text(renderer, Poppins20, White, "Bezier Curves", 10, 10);
            draw_text(renderer, Poppins15, White, "p -> toggle points | l -> toggle lines | a -> add 2 points | r -> remove 2 points", 10, h - 45);
            draw_text(renderer, Poppins15, White, "esc -> reset | +/- -> zoom", 10, h - 30);
            SDL_RenderPresent(renderer);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(bez.points);

    return 0;
}
