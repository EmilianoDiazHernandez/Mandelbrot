#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "render.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);

    const int width = 800, height = 600;

    SDL_Window *window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

    if (window == NULL)
    {
        printf("\nNo se pudo crear la ventana\n%s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        printf("\nNo se pudo crear el render\n%s", SDL_GetError());
        SDL_DestroyWindow(window);
        return EXIT_FAILURE;
    }

    Viewport vp = { .min_real = -2.0, .max_real = 1.0, .min_imag = -1.5, .max_imag = 1.5, .max_iter = 500 };

    bool needs_redraw = true;
    bool running = true;

    int mouse_x = 0, mouse_y = 0;

    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    break;

                case SDL_KEYDOWN:
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_ESCAPE:
                                running = false;
                                break;

                            case SDLK_PLUS:
                                zoom_viewport(&vp, mouse_x, mouse_y, width, height, 1.2);
                                needs_redraw = true;
                                break;

                            case SDLK_MINUS:
                                zoom_viewport(&vp, mouse_x, mouse_y, width, height, 0.8);
                                needs_redraw = true;
                                break;

                            default:
                                break;
                        }
                    }
                    break;

                default:
                    break;
            }

            if (needs_redraw && running) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                draw_mandelbrot(renderer, width, height, &vp);
                SDL_RenderPresent(renderer);
                needs_redraw = false;
            }
        }
        if (needs_redraw && running) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                draw_mandelbrot(renderer, width, height, &vp);
                SDL_RenderPresent(renderer);
                needs_redraw = false;
            }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
