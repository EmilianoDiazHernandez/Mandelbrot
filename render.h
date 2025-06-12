#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

typedef struct {
    double min_real, max_real;
    double min_imag, max_imag;
    int max_iter;
} Viewport;

void draw_mandelbrot(SDL_Renderer *renderer, int width, int height, Viewport *vp);
void zoom_viewport(Viewport *vp, double mouse_x, double mouse_y, int width, int height, double zoom_factor);

#endif
