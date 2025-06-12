#include <SDL2/SDL.h>
#include <stdbool.h>
#include "render.h"

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int width = 800, height = 600;
    SDL_Window *window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Viewport vp = { .min_real = -2.0, .max_real = 1.0, .min_imag = -1.5, .max_imag = 1.5, .max_iter = 500 };

    bool needs_redraw = true;
    bool running = true;

    SDL_Event event;
    while (running) {
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            running = false;

        if (event.type == SDL_KEYDOWN) {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            
            if (event.key.keysym.sym == SDLK_PLUS) {
                zoom_viewport(&vp, mouse_x, mouse_y, width, height, 1.2);
                needs_redraw = true;
            }
            if (event.key.keysym.sym == SDLK_MINUS) {
                zoom_viewport(&vp, mouse_x, mouse_y, width, height, 0.8);
                needs_redraw = true;
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
