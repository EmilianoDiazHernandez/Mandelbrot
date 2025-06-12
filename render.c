#include "render.h"
#include "mandelbrot.h"

void draw_mandelbrot(SDL_Renderer *renderer, int width, int height, Viewport *vp) {
    for (int y = 0; y < height; y++) {
        double imag = vp->min_imag + (vp->max_imag - vp->min_imag) * y / height;
        for (int x = 0; x < width; x++) {
            double real = vp->min_real + (vp->max_real - vp->min_real) * x / width;
            int iter = mandelbrot(real, imag, vp->max_iter);

            // Mapeo de iteración a color
            int color = (iter * 255) / vp->max_iter;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void zoom_viewport(Viewport *vp, double mouse_x, double mouse_y, int width, int height, double zoom_factor) {
    double real_range = vp->max_real - vp->min_real;
    double imag_range = vp->max_imag - vp->min_imag;

    double real_center = vp->min_real + real_range * mouse_x / width;
    double imag_center = vp->min_imag + imag_range * mouse_y / height;

    real_range /= zoom_factor;
    imag_range /= zoom_factor;

    vp->min_real = real_center - real_range / 2;
    vp->max_real = real_center + real_range / 2;
    vp->min_imag = imag_center - imag_range / 2;
    vp->max_imag = imag_center + imag_range / 2;
}
