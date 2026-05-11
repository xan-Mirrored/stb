#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include <stdint.h>
#include <time.h>


const char* input_path = "data/10.4-MB.png";
const char* output_path = "test_image_load_write_runtime.png";
const int attempt = 10;


int main() {
    long double runtime_s = 0;
    int width, height, channels;
    uint8_t* img = NULL;
    for (int i=0; i<attempt; i++) {
        struct timespec start = {0};
        struct timespec end = {0};
        clock_gettime(CLOCK_MONOTONIC, &start);
        img = stbi_load(input_path, &width, &height, &channels, 0);
        clock_gettime(CLOCK_MONOTONIC, &end);
        runtime_s += (end.tv_sec - start.tv_sec) + (long double)(end.tv_nsec - start.tv_nsec) / 10E9;
        if (img == NULL) {
            printf("load fail\n");
            return 1;
        }
    }
    runtime_s /= attempt;
    printf("load: %Lfs\n", runtime_s);

    runtime_s = 0;
    for (int i = 0; i<attempt; i++) {
        struct timespec start = {0};
        struct timespec end = {0};
        clock_gettime(CLOCK_MONOTONIC, &start);
        int ret = stbi_write_png(output_path, width, height, channels, img, width * channels);
        clock_gettime(CLOCK_MONOTONIC, &end);
        runtime_s += (end.tv_sec - start.tv_sec) + (long double)(end.tv_nsec - start.tv_nsec) / 10E9;
        if (!ret) {
            printf("write fail in attempt %d\n", i);
            return 1;
        }
    }
    runtime_s /= attempt;
    printf("write: %Lfs\n", runtime_s);

    free(img);
    printf("done\n");
    return 0;
}