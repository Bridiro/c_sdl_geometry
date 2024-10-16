#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    int id;
    int x;
    int y;
} Point;

Point bezier(Point P0, Point P1, Point P2, float t) {
    Point result;
    float u = 1 - t;
    result.x = u * u * P0.x + 2 * u * t * P1.x + t * t * P2.x;
    result.y = u * u * P0.y + 2 * u * t * P1.y + t * t * P2.y;
    return result;
}

int is_near(Point p, int mouse_x, int mouse_y) {
    int threshold = 10;
    return (abs(p.x - mouse_x) < threshold && abs(p.y - mouse_y) < threshold);
}

void draw_circle_bresenham(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius) {
    int32_t x = radius;
    int32_t y = 0;
    int32_t p = 1 - radius;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);

        y++;

        if (p <= 0) {
            p = p + 2 * y + 1;
        } else {
            x--;
            p = p + 2 * y - 2 * x + 1;
        }
    }
}

void draw_dotted_line(SDL_Renderer* renderer, int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float x_inc = (float)dx / steps;
    float y_inc = (float)dy / steps;

    float x = x1;
    float y = y1;

    for (int i = 0; i < steps; i++) {
        if (i % 10 < 5) {
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
        }
        x += x_inc;
        y += y_inc;
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font* Poppins = TTF_OpenFont("Poppins-Regular.ttf", 15);
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage =
        TTF_RenderText_Solid(Poppins, "p -> toggle points | l -> toggle lines | + -> add 2 points | - -> remove 2 points", White);
    int text_width, text_height;
    TTF_SizeText(Poppins, "p -> toggle points | l -> toggle lines | + -> add 2 points | - -> remove 2 points", &text_width, &text_height);
    SDL_Window* window = SDL_CreateWindow("Bezier Curve", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Point P0 = {0, 100, 400};
    Point P1 = {1, 320, 50};
    Point P2 = {2, 540, 400};

    Point* points = malloc(3 * sizeof(Point));
    int len = 3;
    points[0] = P0;
    points[1] = P1;
    points[2] = P2;

    int lines_on = 1;
    int points_on = 1;
    int selected_point = -1;

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: {
                    quit = 1;
                    break;
                }
                case SDL_MOUSEBUTTONDOWN: {
                    int mouse_x = e.button.x;
                    int mouse_y = e.button.y;

                    for (int i = 0; i < len; i++) {
                        if (is_near(points[i], mouse_x, mouse_y)) {
                            selected_point = i;
                            break;
                        }
                    }
                    break;
                }
                case SDL_MOUSEBUTTONUP: {
                    selected_point = -1;
                    break;
                }
                case SDL_MOUSEMOTION: {
                    if (selected_point != -1) {
                        points[selected_point].x = e.motion.x;
                        points[selected_point].y = e.motion.y;
                        int w, h;
                        SDL_GetRendererOutputSize(renderer, &w, &h);
                        if (points[selected_point].x > w - 5) {
                            points[selected_point].x = w - 5;
                        } else if (points[selected_point].x < 5) {
                            points[selected_point].x = 5;
                        }
                        if (points[selected_point].y > h - 5) {
                            points[selected_point].y = h - 5;
                        } else if (points[selected_point].y < 5) {
                            points[selected_point].y = 5;
                        }
                    }
                    break;
                } 
                case SDL_KEYDOWN: {
                    switch (e.key.keysym.sym) {
                        case SDLK_PLUS: {
                            int w, h;
                            SDL_GetRendererOutputSize(renderer, &w, &h);
                            len += 2;
                            points = realloc(points, len * sizeof(Point));
                            int diff_x = points[len - 3].x - points[len - 4].x;
                            int diff_y = points[len - 3].y - points[len - 4].y;
                            int pos_control_x = points[len - 3].x + diff_x;
                            int pos_control_y = points[len - 3].y + diff_y;
                            while (pos_control_x < 5 || pos_control_x > w - 5 || pos_control_y < 5 || pos_control_y > h - 5) {
                                diff_x /= 2;
                                diff_y /= 2;
                                pos_control_x = points[len - 3].x + diff_x;
                                pos_control_y = points[len - 3].y + diff_y;
                            }
                            points[len - 2] = (Point){len - 1, pos_control_x, pos_control_y};
                            points[len - 1] = (Point){len - 2, 370, 240};
                            break;
                        }
                        case SDLK_MINUS: {
                            if (len > 4) {
                                len -= 2;
                                points = realloc(points, len * sizeof(Point));
                            }
                            break;
                        }
                        case SDLK_l: {
                            lines_on = !lines_on;
                            break;
                        }
                        case SDLK_p: {
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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (float t = 0; t <= 1; t += 0.0001) {
            for (int i = 0; i < len - 2; i += 2) {
                Point p = bezier(points[i], points[i + 1], points[i + 2], t);
                SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
            }
        }

        if (lines_on) {
            for (int i = 0; i < len - 1; i++) {
                draw_dotted_line(renderer, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
            }
        }

        if (points_on) {
            for (int i = 0; i < len; i++) {
                draw_circle_bresenham(renderer, points[i].x, points[i].y, 4);
                draw_circle_bresenham(renderer, points[i].x, points[i].y, 5);
                draw_circle_bresenham(renderer, points[i].x, points[i].y, 6);
            }
        }
        int w, h;
        SDL_GetRendererOutputSize(renderer, &w, &h);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect Message_rect;
        Message_rect.x = 10;
        Message_rect.y = h - 30;
        Message_rect.w = text_width;
        Message_rect.h = text_height;
        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free(points);

    return 0;
}
