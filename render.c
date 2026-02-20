#include <math.h>
#include <omp.h>
#include "render.h"
#include "mandelbrot.h"

static uint32_t get_smooth_color(double iter, int max_iter) {
    if (iter >= max_iter) return 0xFF000000; 

    double t = iter * 0.05; 

    double r = 0.5 + 0.5 * cos(3.0 + t + 0.0);       
    double g = 0.5 + 0.5 * cos(3.0 + t + 0.6);       
    double b = 0.5 + 0.5 * cos(3.0 + t + 1.0);      

    uint8_t R = (uint8_t)(r * 255);
    uint8_t G = (uint8_t)(g * 255);
    uint8_t B = (uint8_t)(b * 255);

    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        return (R << 24) | (G << 16) | (B << 8) | 255;
    #else
        return (255 << 24) | (B << 16) | (G << 8) | R;
    #endif
}

void draw_mandelbrot(uint32_t *pixels, int pitch, int width, int height, Viewport *vp) {
    double real_factor = (vp->max_real - vp->min_real) / width;
    double imag_factor = (vp->max_imag - vp->min_imag) / height;
    double min_r = vp->min_real;
    double min_i = vp->min_imag;

    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; y++) {
        uint32_t *row = (uint32_t*)((uint8_t*)pixels + y * pitch);
        
        double imag = min_i + imag_factor * y;
        
        for (int x = 0; x < width; x++) {
            double real = min_r + real_factor * x;
            
            double val = mandelbrot(real, imag, vp->max_iter);
            
            row[x] = get_smooth_color(val, vp->max_iter);
        }
    }
}

void zoom_viewport(Viewport *vp, double mouse_x, double mouse_y, int width, int height, double zoom_factor) {
    double real_width = vp->max_real - vp->min_real;
    double imag_height = vp->max_imag - vp->min_imag;

    double center_r = vp->min_real + (mouse_x / width) * real_width;
    double center_i = vp->min_imag + (mouse_y / height) * imag_height;

    double new_real_width = real_width / zoom_factor;
    double new_imag_height = imag_height / zoom_factor;

    vp->min_real = center_r - (mouse_x / width) * new_real_width;
    vp->max_real = vp->min_real + new_real_width;
    vp->min_imag = center_i - (mouse_y / height) * new_imag_height;
    vp->max_imag = vp->min_imag + new_imag_height;

    if (zoom_factor > 1.0) {
       vp->max_iter += 5; 
    }
}
