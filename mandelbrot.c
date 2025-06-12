#include "mandelbrot.h"

int mandelbrot(double real, double imag, int max_iter) {
    double z_real = 0.0, z_imag = 0.0;
    int iter = 0;

    while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < max_iter) {
        double temp = z_real * z_real - z_imag * z_imag + real;
        z_imag = 2.0 * z_real * z_imag + imag;
        z_real = temp;
        iter++;
    }

    return iter;
}
