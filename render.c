#include "render.h"
#include "mandelbrot.h"

void hsv_to_rgb(float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b) {
    int i = (int)(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: *r = (uint8_t)(v * 255); *g = (uint8_t)(t * 255); *b = (uint8_t)(p * 255); break;
        case 1: *r = (uint8_t)(q * 255); *g = (uint8_t)(v * 255); *b = (uint8_t)(p * 255); break;
        case 2: *r = (uint8_t)(p * 255); *g = (uint8_t)(v * 255); *b = (uint8_t)(t * 255); break;
        case 3: *r = (uint8_t)(p * 255); *g = (uint8_t)(q * 255); *b = (uint8_t)(v * 255); break;
        case 4: *r = (uint8_t)(t * 255); *g = (uint8_t)(p * 255); *b = (uint8_t)(v * 255); break;
        case 5: *r = (uint8_t)(v * 255); *g = (uint8_t)(p * 255); *b = (uint8_t)(q * 255); break;
    }
}

void draw_mandelbrot(SDL_Renderer *renderer, int width, int height, Viewport *vp) {
    for (int y = 0; y < height; y++) {
        double imag = vp->min_imag + (vp->max_imag - vp->min_imag) * y / height;
        for (int x = 0; x < width; x++) {
            double real = vp->min_real + (vp->max_real - vp->min_real) * x / width;
            int iter = mandelbrot(real, imag, vp->max_iter);

            uint8_t r, g, b;
            if (iter == vp->max_iter) {
                r = g = b = 0;
            } else {
                float hue = (float)iter / vp->max_iter; 
                hsv_to_rgb(hue, 1.0f, 1.0f, &r, &g, &b);
            }

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
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
