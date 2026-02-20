#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "render.h"

void update_fps(SDL_Window *window, Uint32 *last_time, int *frames) {
    (*frames)++;
    Uint32 current_time = SDL_GetTicks();
    if (current_time > *last_time + 1000) {
        char title[64];
        snprintf(title, sizeof(title), "Mandelbrot - FPS: %d", *frames);
        SDL_SetWindowTitle(window, title);
        *last_time = current_time;
        *frames = 0;
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error SDL_Init: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    const int width = 800;
    const int height = 600;

    SDL_Window *window = SDL_CreateWindow("Mandelbrot",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          width, height,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width, height);
    if (!texture) {
        fprintf(stderr, "Error SDL_CreateTexture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    Viewport vp = { .min_real = -2.0, .max_real = 1.0, .min_imag = -1.5, .max_imag = 1.5, .max_iter = 100 };

    bool running = true;
    bool needs_redraw = true;

    int mouse_x = 0, mouse_y = 0;
    Uint32 last_fps_time = SDL_GetTicks();
    int fps_frames = 0;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEMOTION:
                    mouse_x = event.motion.x;
                    mouse_y = event.motion.y;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;
                        case SDLK_PLUS:
                        case SDLK_KP_PLUS:
                            zoom_viewport(&vp, mouse_x, mouse_y, width, height, 1.2); // Zoom in
                            needs_redraw = true;
                            break;
                        case SDLK_MINUS:
                        case SDLK_KP_MINUS:
                            zoom_viewport(&vp, mouse_x, mouse_y, width, height, 0.8); // Zoom out
                            needs_redraw = true;
                            break;
                        case SDLK_r: // Reset
                            vp.min_real = -2.0; vp.max_real = 1.0;
                            vp.min_imag = -1.5; vp.max_imag = 1.5;
                            vp.max_iter = 100;
                            needs_redraw = true;
                            break;
                    }
                    break;
            }
        }

        // Renderizado
        if (needs_redraw) {
            void *pixels;
            int pitch;

            // Bloquear textura para escritura directa
            if (SDL_LockTexture(texture, NULL, &pixels, &pitch) == 0) {
                draw_mandelbrot((uint32_t*)pixels, pitch, width, height, &vp);
                
                SDL_UnlockTexture(texture);
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            
            needs_redraw = false; 
            
            update_fps(window, &last_fps_time, &fps_frames);
        } else {
            SDL_Delay(10);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
