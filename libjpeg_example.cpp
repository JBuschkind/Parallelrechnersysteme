
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <jpeglib.h>
#include <jerror.h>
#include <string.h>
#include <iostream>
#include <omp.h>

using namespace std;

// Basic structure to hold image data
typedef struct {
    unsigned char* pixels;  // Raw pixel data
    int width;             // Image width
    int height;            // Image height
    int channels;          // Number of color channels
} Image;

// Function to read a JPEG file
Image* read_jpeg_file(const char* filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* file;
    Image* image;

    // Open the file
    if ((file = fopen(filename, "rb")) == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // Initialize JPEG decompression object
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);

    // Read header and start decompression
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    // Allocate memory for the image structure
    image = (Image*)malloc(sizeof(Image));
    image->width = cinfo.output_width;
    image->height = cinfo.output_height;
    image->channels = cinfo.output_components;

    // Allocate memory for pixel data
    image->pixels = (unsigned char*)malloc(
        image->width * image->height * image->channels
    );

    // Read scanlines
    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char* row = &image->pixels[
            cinfo.output_scanline * image->width * image->channels
        ];
        jpeg_read_scanlines(&cinfo, &row, 1);
    }

    // Clean up
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(file);

    return image;
}

// Function to write a JPEG file
int write_jpeg_file(const char* filename, Image* image, int quality) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* file;

    // Open the file
    if ((file = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 0;
    }

    // Initialize JPEG compression object
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, file);

    // Set compression parameters
    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = image->channels;
    cinfo.in_color_space = JCS_RGB;  // Assuming RGB color space

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    // Start compression
    jpeg_start_compress(&cinfo, TRUE);

    // Write scanlines
    while (cinfo.next_scanline < cinfo.image_height) {
        unsigned char* row = &image->pixels[
            cinfo.next_scanline * image->width * image->channels
        ];
        jpeg_write_scanlines(&cinfo, &row, 1);
    }

    // Clean up
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(file);

    return 1;
}

// Example function to adjust brightness
void adjust_brightness(Image* image, int adjustment) {
    int total_pixels = image->width * image->height * image->channels;
    for (int i = 0; i < total_pixels; i++) {
        int new_value = image->pixels[i] + adjustment;
        // Clamp values between 0 and 255
        image->pixels[i] = (new_value < 0) ? 0 :
                          (new_value > 255) ? 255 : new_value;
    }
}

void blur_image(Image* image, int blur_radius) {

    unsigned char* temp_pixels = (unsigned char*)malloc(image->width * image->height * image->channels);
    if (blur_radius < 1) blur_radius = 1;

    #pragma omp parallel for collapse(3)
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            for (int c = 0; c < image->channels; c++) {
                int sum = 0;
                int count = 0;

                for (int dy = -blur_radius; dy <= blur_radius; dy++) {
                    for (int dx = -blur_radius; dx <= blur_radius; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;

                        if (nx >= 0 && nx < image->width && ny >= 0 && ny < image->height) {
                            int neighbor_index = (ny * image->width + nx) * image->channels + c;
                            sum += image->pixels[neighbor_index];
                            count++;
                        }
                    }
                }

                int current_index = (y * image->width + x) * image->channels + c;
                temp_pixels[current_index] = sum / count;
            }
        }
    }

    memcpy(image->pixels, temp_pixels, image->width * image->height * image->channels);
    free(temp_pixels);
}

// Example usage
int main() {
    // Read an image
    Image* image = read_jpeg_file("inputx16.jpg");
    if (!image) {
        fprintf(stderr, "Error reading image\n");
        return 1;
    }

;

    double t = omp_get_wtime ();
  
    // Blursed (increase by 30)
    blur_image(image, 4);

    cout << omp_get_wtime () - t << endl;

    // Save the modified image
    if (!write_jpeg_file("output.jpg", image, 90)) {
        fprintf(stderr, "Error writing image\n");
        return 1;
    }

    // Clean up
    free(image->pixels);
    free(image);

    printf("Image processing completed successfully!\n");
    return 0;
}
