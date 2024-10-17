#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include "utilities.h"
#include "bezier.h"
#include "text.h"

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *Poppins = TTF_OpenFont("Misc/Poppins-Regular.ttf", 15);
    SDL_Color White = {255, 255, 255};
    SDL_Window *window = SDL_CreateWindow("Bezier Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    bezier_s bez = new_bezier();

    int lines_on = 1;
    int points_on = 1;
    int selected_point = -1;

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
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
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_PLUS:
                {
                    bezier_add(&bez, renderer);
                    break;
                }
                case SDLK_MINUS:
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
        draw_text(renderer, Poppins, White, "p -> toggle points | l -> toggle lines | + -> add 2 points | - -> remove 2 points", 10, h - 30);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(bez.points);

    return 0;
}
