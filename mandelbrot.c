#include <math.h>
#include "mandelbrot.h"

double mandelbrot(double c_real, double c_imag, int max_iter) {
    double q = (c_real - 0.25) * (c_real - 0.25) + c_imag * c_imag;
    if (q * (q + (c_real - 0.25)) <= 0.25 * c_imag * c_imag) return max_iter;
    if ((c_real + 1.0) * (c_real + 1.0) + c_imag * c_imag <= 0.0625) return max_iter;

    double z_real = 0.0;
    double z_imag = 0.0;
    double z_real2 = 0.0;
    double z_imag2 = 0.0;
    int iter = 0;

    while (z_real2 + z_imag2 <= 4.0 && iter < max_iter) {
        z_imag = 2.0 * z_real * z_imag + c_imag;
        z_real = z_real2 - z_imag2 + c_real;
        z_real2 = z_real * z_real;
        z_imag2 = z_imag * z_imag;
        iter++;
    }

    if (iter == max_iter) {
        return (double)max_iter;
    }
    
    double modulus = sqrt(z_real2 + z_imag2);
    double smooth_val = iter + 1.0 - log2(log2(modulus)); 
    
    return smooth_val;
}
