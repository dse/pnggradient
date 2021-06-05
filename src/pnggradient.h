#ifndef PNGGRADIENT_H
#define PNGGRADIENT_H

#include <png.h>

typedef struct pnggradient_options {
    int width;
    int height;
    unsigned long from_color;
    unsigned long to_color;
    int dither;
    int direction;
    float grain;
} pnggradient_options_s;

int main(int argc, char **argv);

void usage(void);

int pnggradient_generate(pnggradient_options_s pnggradient_options,
                         char *filename);

int random_value(int from, int to);

unsigned long parse_color(char *arg, int *status);

#define pnggradient_r(color) ((signed int)(((color) & (0xffu << 24)) >> 24))
#define pnggradient_g(color) ((signed int)(((color) & (0xffu << 16)) >> 16))
#define pnggradient_b(color) ((signed int)(((color) & (0xffu << 8))  >> 8))
#define pnggradient_a(color) ((signed int)(((color) & (0xffu << 0))  >> 0))
#define pnggradient_rgba(r, g, b, a)                                    \
    (((r) & 0xffu) << 24 | ((g) & 0xffu) << 16 | ((b) & 0xffu) << 8 | ((a) & 0xffu))
#define pnggradient_component(i)                \
    (((i) < 0) ? 0 : ((i) > 255) ? 255 : (i))
#define pnggradient_float_component(from, to, y, height)                \
    (                                                                   \
        ((from) == (to)) ? (from) :                                     \
        ((height) < 2) ? (0.5 * ((from) + (to))) :                      \
        ((0.0 + (from) + 1.0 * ((to) - (from)) * (y) / ((height) - 1))) \
    )

#define PNGGRADIENT_DIRECTION_HORIZONTAL 0
#define PNGGRADIENT_DIRECTION_VERTICAL 1

#endif
