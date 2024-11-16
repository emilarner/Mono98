#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <stdbool.h>
#include <float.h>
#include <math.h>

#include <errno.h>
#include <jpeglib.h>

typedef struct {
    float r;
    float g;
    float b;
} RGBColor;

RGBColor vga_palette[256] = {
    {0, 0, 0}, {0, 0, 170}, {0, 170, 0}, {0, 170, 170}, {170, 0, 0}, {170, 0, 170}, {170, 85, 0}, {170, 170, 170},
    {85, 85, 85}, {85, 85, 255}, {85, 255, 85}, {85, 255, 255}, {255, 85, 85}, {255, 85, 255}, {255, 255, 85}, {255, 255, 255},
    
    // Standard colors (indexes 16-231)
    {0, 0, 0}, {20, 20, 20}, {32, 32, 32}, {44, 44, 44}, {56, 56, 56}, {68, 68, 68}, {80, 80, 80}, {96, 96, 96},
    {112, 112, 112}, {128, 128, 128}, {144, 144, 144}, {160, 160, 160}, {180, 180, 180}, {200, 200, 200}, {224, 224, 224}, {252, 252, 252},
    {0, 0, 252}, {64, 0, 252}, {124, 0, 252}, {188, 0, 252}, {252, 0, 252}, {252, 0, 188}, {252, 0, 124}, {252, 0, 64},
    {252, 0, 0}, {252, 64, 0}, {252, 124, 0}, {252, 188, 0}, {252, 252, 0}, {188, 252, 0}, {124, 252, 0}, {64, 252, 0},
    {0, 252, 0}, {0, 252, 64}, {0, 252, 124}, {0, 252, 188}, {0, 252, 252}, {0, 188, 252}, {0, 124, 252}, {0, 64, 252},
    {124, 124, 252}, {156, 124, 252}, {188, 124, 252}, {220, 124, 252}, {252, 124, 252}, {252, 124, 220}, {252, 124, 188}, {252, 124, 156},
    {252, 124, 124}, {252, 156, 124}, {252, 188, 124}, {252, 220, 124}, {252, 252, 124}, {220, 252, 124}, {188, 252, 124}, {156, 252, 124},
    {124, 252, 124}, {124, 252, 156}, {124, 252, 188}, {124, 252, 220}, {124, 252, 252}, {124, 220, 252}, {124, 188, 252}, {124, 156, 252},
    {180, 180, 252}, {196, 180, 252}, {220, 180, 252}, {236, 180, 252}, {252, 180, 252}, {252, 180, 236}, {252, 180, 220}, {252, 180, 196},
    {252, 180, 180}, {252, 196, 180}, {252, 220, 180}, {252, 236, 180}, {252, 252, 180}, {236, 252, 180}, {220, 252, 180}, {196, 252, 180},
    {180, 252, 180}, {180, 252, 196}, {180, 252, 220}, {180, 252, 236}, {180, 252, 252}, {180, 236, 252}, {180, 220, 252}, {180, 196, 252},
    {0, 0, 112}, {28, 0, 112}, {56, 0, 112}, {84, 0, 112}, {112, 0, 112}, {112, 0, 84}, {112, 0, 56}, {112, 0, 28},
    {112, 0, 0}, {112, 28, 0}, {112, 56, 0}, {112, 84, 0}, {112, 112, 0}, {84, 112, 0}, {56, 112, 0}, {28, 112, 0},
    {0, 112, 0}, {0, 112, 28}, {0, 112, 56}, {0, 112, 84}, {0, 112, 112}, {0, 84, 112}, {0, 56, 112}, {0, 28, 112},
    {56, 56, 112}, {68, 56, 112}, {84, 56, 112}, {96, 56, 112}, {112, 56, 112}, {112, 56, 96}, {112, 56, 84}, {112, 56, 68},
    {112, 56, 56}, {112, 68, 56}, {112, 84, 56}, {112, 96, 56}, {112, 112, 56}, {96, 112, 56}, {84, 112, 56}, {68, 112, 56},
    {56, 112, 56}, {56, 112, 68}, {56, 112, 84}, {56, 112, 96}, {56, 112, 112}, {56, 96, 112}, {56, 84, 112}, {56, 68, 112},
    
    // Last 16 colors (grayscale)
    {0, 0, 0}, {16, 16, 16}, {32, 32, 32}, {48, 48, 48}, {64, 64, 64}, {80, 80, 80}, {96, 96, 96}, {112, 112, 112},
    {128, 128, 128}, {144, 144, 144}, {160, 160, 160}, {176, 176, 176}, {192, 192, 192}, {208, 208, 208}, {224, 224, 224}, {240, 240, 240}
};

float gamma_correct(float c) {
    if (c <= 0.04045f) {
        return c / 12.92f;
    }
    return powf((c + 0.055f) / 1.055f, 2.4f);
}

// Function to convert RGB to LAB
void rgb_to_lab(int R, int G, int B, float* L, float* A, float* Bc) {
    // Normalize RGB to [0, 1]
    float r = gamma_correct(R / 255.0f);
    float g = gamma_correct(G / 255.0f);
    float b = gamma_correct(B / 255.0f);

    // Convert to XYZ
    float X = 0.4124564f * r + 0.3575761f * g + 0.1804375f * b;
    float Y = 0.2126729f * r + 0.7151522f * g + 0.0721750f * b;
    float Z = 0.0193339f * r + 0.1191920f * g + 0.9503041f * b;

    // Normalize for D65 white point
    X /= 0.95047f;
    Z /= 1.08883f;

    // Apply the f(t) function
    float fx = (X > 0.008856f) ? powf(X, 1.0f / 3.0f) : (X / 903.3f) + (16.0f / 116.0f);
    float fy = (Y > 0.008856f) ? powf(Y, 1.0f / 3.0f) : (Y / 903.3f) + (16.0f / 116.0f);
    float fz = (Z > 0.008856f) ? powf(Z, 1.0f / 3.0f) : (Z / 903.3f) + (16.0f / 116.0f);

    // Compute LAB values
    *L = (116.0f * fy) - 16.0f;
    *A = 500.0f * (fx - fy);
    *Bc = 200.0f * (fy - fz);
}

int main(int argc, char *argv[]) {
    bool lab = false;
    bool cosine = false;

    uint16_t res_x = 320;
    uint16_t res_y = 200;
    uint16_t colours = 256;

    char *demo_filename = NULL;

    if (argc < 2) {
        fprintf(stderr, "error: imgconvert needs a file (the first argument)\n");
        return -1;
    }

    if (argc < 3) {
        fprintf(stderr, "error: imgconvert needs an output file (the second argument)\n");
        return -1;
    }

    for (uint8_t i = 3; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "--colours") || !strcmp(argv[i], "-c")) {
                colours = atoi(argv[i + 1]);

                if (colours != 16 && colours != 256) {
                    fprintf(stderr, "error: can only choose between 16 and 256 colours\n");
                    return -1;
                }
            } 

            else if (!strcmp(argv[i], "--res-x"))
                res_x = atoi(argv[i + 1]);

            else if (!strcmp(argv[i], "--res-y"))
                res_y = atoi(argv[i + 1]);

            else if (!strcmp(argv[i], "--demo"))
                demo_filename = argv[i + 1];

            else if (!strcmp(argv[i], "--lab"))
                lab = true;

            else if (!strcmp(argv[i], "--cosine-distance"))
                cosine = true;

            else {
                fprintf(stderr, "error: invalid argument '%s'\n", argv[i]);
                return -1;
            }
        }
    }


    if (lab) {
        float L;
        float A;
        float B;

        for (uint16_t i = 0; i < colours; i++) {
            rgb_to_lab(
                vga_palette[i].r,
                vga_palette[i].g,
                vga_palette[i].b,
                &vga_palette[i].r,
                &vga_palette[i].g,
                &vga_palette[i].b
            );
        }
    }

    FILE *imagefp = fopen(argv[1], "rb");
    FILE *outfp = fopen(argv[2], "wb");
    FILE *demofp = fopen("demo.jpeg", "wb");

    if (imagefp == NULL) {
        fprintf(stderr, "error reading image file: %s\n", strerror(errno));
        return -1;
    }

    if (outfp == NULL) {
        fprintf(stderr, "error opening output file: %s\n", strerror(errno));
        return -1;
    }

    struct jpeg_compress_struct demoinfo;
    struct jpeg_error_mgr demoerr;
    demoinfo.err = jpeg_std_error(&demoerr); 
    jpeg_create_compress(&demoinfo);
    jpeg_stdio_dest(&demoinfo, demofp);

    demoinfo.image_width = 320;
    demoinfo.image_height = 200;
    demoinfo.input_components = 3;
    demoinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&demoinfo);
    jpeg_set_quality(&demoinfo, 100, true);
    jpeg_start_compress(&demoinfo, true);


    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, imagefp);
    

    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK) {
        fprintf(stderr, "error: not a JPEG file\n");
        jpeg_destroy_decompress(&cinfo);
        fclose(imagefp);
        fclose(outfp);
        return -1;
    }

    jpeg_start_decompress(&cinfo);


    uint8_t scanline[res_x * 3];
    uint8_t demoline[res_x * 3];

    while (cinfo.output_scanline < res_y) {
        uint8_t *row_pointer = scanline;

        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
        for (uint16_t i = 0; i < res_x; i++) {
            float red = scanline[i * 3];
            float green = scanline[i * 3 + 1];
            float blue = scanline[i * 3 + 2];

            if (lab) {
                rgb_to_lab(
                    red,
                    green,
                    blue,
                    &red,
                    &green,
                    &blue
                );
            }

            double minimum_distance = DBL_MAX;
            uint16_t minimum_palette = -1;

            for (uint16_t j = 0; j < colours; j++) {
                double tmp_distance = 0.00;

                if (cosine) {
                    double dot_product = 0.00;
                    
                    dot_product += red * vga_palette[j].r;
                    dot_product += green * vga_palette[j].g;
                    dot_product += blue * vga_palette[j].b;

                    double mag1 = sqrt(red*red + green*green + blue*blue);
                    double mag2 = sqrt(pow(vga_palette[j].r, 2) + pow(vga_palette[j].g, 2) + pow(vga_palette[j].b, 2));

                    tmp_distance = 1 - (dot_product / (mag1*mag2));

                } else {
                    tmp_distance = sqrt(
                        pow(red - vga_palette[j].r, 2) +
                        pow(green - vga_palette[j].g, 2) +
                        pow(blue - vga_palette[j].b, 2)
                    );
                }

                if (tmp_distance < minimum_distance) {
                    minimum_distance = tmp_distance;
                    minimum_palette = j;
                }
            }

            demoline[i * 3] = vga_palette[minimum_palette].r;
            demoline[i * 3 + 1] = vga_palette[minimum_palette].g;
            demoline[i * 3 + 2] = vga_palette[minimum_palette].b;

            uint8_t byte_palette = (uint8_t)minimum_palette;
            fwrite(&byte_palette, 1, 1, outfp);
        }

        uint8_t *demoline_ptr = demoline;
        jpeg_write_scanlines(&demoinfo, &demoline_ptr, 1);
    }

    jpeg_finish_compress(&demoinfo);
    jpeg_destroy_compress(&demoinfo);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(imagefp);
    fclose(demofp);
    fclose(outfp);

    return 0;
}