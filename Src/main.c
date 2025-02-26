#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "bezier.h"
#include "cartesian_graph.h"

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
    SDL_SetRenderTarget(renderer, NULL);
    SDL_StartTextInput();

    float zoom = 1;
    int pan_x = 0;
    int pan_y = 0;

    int width = 640;
    int height = 480;

    typedef enum
    {
        BEZIER,
        CARTESIAN
    } mode_e;
    mode_e mode = BEZIER;

    bezier_s bez = bezier_new();
    cartesian_graph_s cart = cartesian_graph_new(20, White, White);
    cart.equation = NULL;

    SDL_Event e;
    int quit = 0;
    while (!quit)
    {
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
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
                if (mode == BEZIER)
                {
                    bezier_select_point(&bez, e.button.x, e.button.y, zoom, pan_x, pan_y);
                }
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (mode == BEZIER)
                {
                    bez.selected_point = -1;
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                if (bez.selected_point != -1)
                {
                    if (mode == BEZIER)
                    {
                        bezier_move_point(&bez, e.motion.x, e.motion.y, w, h, zoom, pan_x, pan_y);
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
                case SDLK_UP:
                {
                    pan_x -= 30;
                    pan_y -= 30;
                    zoom += 0.1;
                    break;
                }
                case SDLK_DOWN:
                {
                    pan_x += 30;
                    pan_y += 30;
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
                    cart = cartesian_graph_new(20, White, White);
                    break;
                }
                case SDLK_RIGHT:
                {
                    mode = !mode;
                    break;
                }
                case SDLK_LEFT:
                {
                    mode = !mode;
                    break;
                }
                case SDLK_BACKSPACE:
                {
                    if (mode == CARTESIAN && cart.equation != NULL && strlen(cart.equation) > 0)
                    {
                        cart.equation[strlen(cart.equation) - 1] = '\0';
                    }
                    break;
                }
                }
                break;
            }
            case SDL_TEXTINPUT:
            {
                if (mode == CARTESIAN)
                {
                    char *character = e.text.text;
                    if (cart.equation == NULL)
                    {
                        cart.equation = (char *)malloc(strlen(character) + 1);
                        if (cart.equation != NULL) // Controlla se malloc ha successo
                        {
                            strcpy(cart.equation, character);
                        }
                    }
                    else
                    {
                        char *new_equation = (char *)realloc(cart.equation, strlen(cart.equation) + strlen(character) + 1);
                        if (new_equation != NULL) // Controlla se realloc ha successo
                        {
                            cart.equation = new_equation;
                            strcat(cart.equation, character);
                        }
                    }
                }
                break;
            }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (mode == BEZIER)
            {
                bezier_draw(&bez, renderer, White, 1, 1, 5, 2, zoom, pan_x, pan_y);
                draw_text(renderer, Poppins20, White, "Bezier Curves", 10, 10);
                draw_text(renderer, Poppins15, White, "p -> toggle points | l -> toggle lines | a -> add 2 points | r -> remove 2 points", 10, h - 48);
            }
            else
            {
                cartesian_graph_draw(&cart, renderer, w, h, zoom, pan_x, pan_y);
                cartesian_graph_draw_equation_result(&cart, renderer, White, w, h, zoom, pan_x, pan_y);
                draw_text(renderer, Poppins20, White, "Cartesian Graph", 10, 10);
                if (cart.equation != NULL && strlen(cart.equation) > 0)
                {
                    draw_text(renderer, Poppins15, White, cart.equation, 10, h - 48);
                }
            }
            draw_text(renderer, Poppins15, White, "esc -> reset | +/- -> zoom | arrows -> change modes", 10, h - 30);
            SDL_RenderPresent(renderer);
        }
    }
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(bez.points);

    return 0;
}
