/**
 * pnggradient
 *     generates pnggradient.png
 * pnggradient foo.png
 *     generates foo.png
 *
 * Thanks http://www.labbookpages.co.uk/software/imgProc/libPNG.html
 * for the example.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "pnggradient.h"

static pnggradient_options_s pnggradient_defaults = {
    .width      = 256,
    .height     = 256,
    .from_color = 0x990000ff,
    .to_color   = 0xff0000ff,
    .dither     = 0,
    .direction  = PNGGRADIENT_DIRECTION_VERTICAL,
    .grain      = 0
};

int main(int argc, char **argv) {
    pnggradient_options_s pnggradient_options = pnggradient_defaults;
    char *filename = "pnggradient.png";
    int ch;
    int status;
    while ((ch = getopt(argc, argv, "W:H:c:C:dhog:?")) != -1) {
        switch (ch) {
        case 'W':
            pnggradient_options.width = atoi(optarg);
            break;
        case 'H':
            pnggradient_options.height = atoi(optarg);
            break;
        case 'c':
            pnggradient_options.from_color = parse_color(optarg, &status);
            if (!status) {
                fprintf(stderr, "invalid color: %s", optarg);
                exit(1);
            }
            break;
        case 'C':
            pnggradient_options.to_color = parse_color(optarg, &status);
            if (!status) {
                fprintf(stderr, "invalid color: %s", optarg);
                exit(1);
            }
            break;
        case 'd':
            pnggradient_options.dither = 1;
            break;
        case 'o':
            pnggradient_options.direction = PNGGRADIENT_DIRECTION_HORIZONTAL;
            break;
        case 'g':
            pnggradient_options.grain = atof(optarg);
            break;
        case 'h':
        case '?':
        default:
            usage();
            exit(0);
        }
    }
    argc -= optind;
    argv += optind;
    if (argc) {
        filename = argv[0];
    }
    pnggradient_generate(pnggradient_options, filename);
}

int pnggradient_generate(pnggradient_options_s pnggradient_options,
                         char *filename) {
    int code = 0;
    FILE *fp = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;

    if (!strcmp(filename, "-")) {
        fp = stdout;
    } else {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            fprintf(stderr, "Could not open file %s for writing\n", filename);
            code = 1;
            goto finalise;
        }
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fprintf(stderr, "Could not allocate write struct\n");
        code = 1;
        goto finalise;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fprintf(stderr, "Could not allocate info struct\n");
        code = 1;
        goto finalise;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        code = 1;
        goto finalise;
    }

    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr,
                 info_ptr,
                 pnggradient_options.width,
                 pnggradient_options.height,
                 8,             /* bit depth of each channel */
                 PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_BASE);
    png_set_compression_strategy(png_ptr, Z_DEFAULT_STRATEGY);
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    png_text title_text;
    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    title_text.key = "Title";
    title_text.text = "image generated by pnggradient";
    png_set_text(png_ptr, info_ptr, &title_text, 1);

    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(4 * pnggradient_options.width * sizeof(png_byte));

    srand(time(NULL) + getpid() * 23);

    int x, y;
    float rf, gf, bf, af;
    int r, g, b, a;
    float rp, gp, bp, ap;
    int r1, g1, b1, a1;
    int r2, g2, b2, a2;
    int w, h;
    int d;

    r1 = pnggradient_r(pnggradient_options.from_color);
    g1 = pnggradient_g(pnggradient_options.from_color);
    b1 = pnggradient_b(pnggradient_options.from_color);
    a1 = pnggradient_a(pnggradient_options.from_color);
    r2 = pnggradient_r(pnggradient_options.to_color);
    g2 = pnggradient_g(pnggradient_options.to_color);
    b2 = pnggradient_b(pnggradient_options.to_color);
    a2 = pnggradient_a(pnggradient_options.to_color);
    w = pnggradient_options.width;
    h = pnggradient_options.height;
    d = pnggradient_options.direction;

    for (y = 0; y < h; y += 1) {
        for (x = 0; x < w; x += 1) {
            switch (d) {
            case PNGGRADIENT_DIRECTION_HORIZONTAL:
                rf = pnggradient_float_component(r1, r2, x, w);
                gf = pnggradient_float_component(g1, g2, x, w);
                bf = pnggradient_float_component(b1, b2, x, w);
                af = pnggradient_float_component(a1, a2, x, w);
                break;
            default:
                rf = pnggradient_float_component(r1, r2, y, h);
                gf = pnggradient_float_component(g1, g2, y, h);
                bf = pnggradient_float_component(b1, b2, y, h);
                af = pnggradient_float_component(a1, a2, y, h);
                break;
            }
            if (pnggradient_options.dither) {
                if (pnggradient_options.dither > 1) {
                    rf /= pnggradient_options.dither;
                    gf /= pnggradient_options.dither;
                    bf /= pnggradient_options.dither;
                    af /= pnggradient_options.dither;
                }
                rp = rf - floor(rf); /* [0.0, 1.0) */
                gp = gf - floor(gf);
                bp = bf - floor(bf);
                ap = af - floor(af);
                r = pnggradient_component((int)rf + ((1.0 * rand() / RAND_MAX) < rp ? 1 : 0));
                g = pnggradient_component((int)gf + ((1.0 * rand() / RAND_MAX) < gp ? 1 : 0));
                b = pnggradient_component((int)bf + ((1.0 * rand() / RAND_MAX) < bp ? 1 : 0));
                a = pnggradient_component((int)af + ((1.0 * rand() / RAND_MAX) < ap ? 1 : 0));
                if (pnggradient_options.dither > 1) {
                    r *= pnggradient_options.dither;
                    g *= pnggradient_options.dither;
                    b *= pnggradient_options.dither;
                    a *= pnggradient_options.dither;
                }
            } else {
                r = pnggradient_component((int)(rf + 0.5));
                g = pnggradient_component((int)(gf + 0.5));
                b = pnggradient_component((int)(bf + 0.5));
                a = pnggradient_component((int)(af + 0.5));
            }

            if (pnggradient_options.grain) {
                float amount = 1 + pnggradient_options.grain * (2.0 * rand() / RAND_MAX - 1.0);
                r = (int)(0.5 + 1.0 * r * amount);
                g = (int)(0.5 + 1.0 * g * amount);
                b = (int)(0.5 + 1.0 * b * amount);
            }

            row[x * 4] = r;
            row[x * 4 + 1] = g;
            row[x * 4 + 2] = b;
            row[x * 4 + 3] = a;
        }
        png_write_row(png_ptr, row);
    }
    png_write_end(png_ptr, NULL);

 finalise:
    if (fp != NULL) {
        fclose(fp);
    }
    if (info_ptr != NULL) {
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    }
    if (png_ptr != NULL) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    }
    if (row != NULL) {
        free(row);
    }
    return code;
}

int random_value(int from, int to) {
    if (from == to) {
        return from;
    }
    if (from > to) {
        return (int)(
            to + 1.0 * (from - to + 1) * rand() / RAND_MAX
        );
    }
    return (int)(
        from + 1.0 * (to - from + 1) * rand() / RAND_MAX
    );
}

void usage(void) {
    puts("usage: pnggradient [<option> ...] [<filename>]");
    puts("options:");
    puts("    -W <int>    specify image width (default 256)");
    puts("    -H <int>    specify image height (default 256)");
    puts("    -c <color>  from color (default '#990000')");
    puts("    -C <color>  to color (default '#ff0000')");
    puts("    -o          horizontal gradient (vertical by default)");
    puts("    -d          dither");
    puts("    -h, -?      display this message");
    puts("<color> arguments can be hex '#rgb', '#rgba', '#rrggbb', or '#rrggbbaa'.");
}

unsigned long parse_color(char *arg, int *status) {
    if (status != NULL) {
        *status = 1;
    }
    if (*arg == '#') {
        arg += 1;
    }
    char *ch;
    for (ch = arg; *ch; ch += 1) {
        if (!isxdigit(*ch)) {
            if (status != NULL) {
                *status = 0;
            }
            return 0;
        }
    }
    char r_string[5];
    char g_string[5];
    char b_string[5];
    char a_string[5];
    memset(r_string, 0, 5);
    memset(g_string, 0, 5);
    memset(b_string, 0, 5);
    memset(a_string, 0, 5);
    int r = 255, g = 255, b = 255, a = 255;
    if (strlen(arg) == 6 || strlen(arg) == 8) {
        r_string[0] = arg[0];
        r_string[1] = arg[1];
        g_string[0] = arg[2];
        g_string[1] = arg[3];
        b_string[0] = arg[4];
        b_string[1] = arg[5];
        if (strlen(arg) == 8) {
            a_string[0] = arg[6];
            a_string[1] = arg[7];
        }
    } else if (strlen(arg) == 3 || strlen(arg) == 4) {
        r_string[0] = arg[0];
        r_string[1] = arg[0];
        g_string[0] = arg[1];
        g_string[1] = arg[1];
        b_string[0] = arg[2];
        b_string[1] = arg[2];
        if (strlen(arg) == 4) {
            a_string[0] = arg[3];
            a_string[1] = arg[3];
        }
    } else {
        if (status != NULL) {
            *status = 0;
        }
        return 0;
    }
    sscanf(r_string, "%x", &r);
    sscanf(g_string, "%x", &g);
    sscanf(b_string, "%x", &b);
    if (*a_string) {
        sscanf(a_string, "%x", &a);
    }
    r = pnggradient_component(r);
    g = pnggradient_component(g);
    b = pnggradient_component(b);
    a = pnggradient_component(a);
    printf("%s => %d %d %d %d\n", arg, r, g, b, a);
    return pnggradient_rgba(r, g, b, a);
}
