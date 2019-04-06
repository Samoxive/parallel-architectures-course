#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

const i32 REPEAT_COUNT = 2048;

typedef struct {
    i32 x;
    i32 y;
    i32 temp;
} heat_source;

typedef struct {
    i32 dim_x;
    i32 dim_y;
    i32 heat_count;
    heat_source* heat_sources;
} heat_data;

heat_data* get_data_from_file() {
    FILE* file = fopen("input2.txt", "r");
    heat_data* data = malloc(sizeof(heat_data));
    i32 dim_x, dim_y;
    fscanf(file, "Dimensions:%d X %d\n", &dim_x, &dim_y);
    data->dim_x = dim_x;
    data->dim_y = dim_y;
    i32 heat_count;
    fscanf(file, "Number of Heat Source: %d\n", &heat_count);
    data->heat_count = heat_count;
    fscanf(file, "%*[^\n]\n");
    data->heat_sources = malloc(sizeof(heat_source) * heat_count);
    for (i32 i = 0; i < heat_count; i++) {
        heat_source source;
        fscanf(file, "(%d, %d, %d)\n", &source.x, &source.y, &source.temp);
        data->heat_sources[i] = source;
    }

    fclose(file);
    return data;
}

void write_output_to_file(i32** output, heat_data* data) {
    FILE* file = fopen("output2.txt", "w");

    for (i32 y = 0; y < data->dim_y; y++) {
        for (i32 x = 0; x < data->dim_x; x++) {
            if (x == data->dim_x - 1) {
                fprintf(file, "%d", output[y][x]);
            } else {
                fprintf(file, "%d ", output[y][x]);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

f32 get_heat_multiplier(i32 x1, i32 y1, i32 x2, i32 y2) {
    i32 dist_squared =
        (abs(x1 - x2) * abs(x1 - x2)) + (abs(y1 - y2) * abs(y1 - y2));
    if (dist_squared == 0) {
        return 1;
    } else if (dist_squared > 0 && dist_squared <= 5) {
        return 0.85;
    } else if (dist_squared > 5 && dist_squared <= 25) {
        return 0.75;
    } else if (dist_squared > 25 && dist_squared <= 49) {
        return 0.65;
    } else if (dist_squared > 49 && dist_squared <= 61) {
        return 0.60;
    } else {
        return 0;
    }
}

f64 do_it(i32** output, heat_data* data) {
    f64 before = omp_get_wtime();
#pragma omp parallel for collapse(2)
    for (i32 y = 0; y < data->dim_y; y++) {
        for (i32 x = 0; x < data->dim_x; x++) {
            i32 max_heat = -1;
            for (i32 i = 0; i < data->heat_count; i++) {
                heat_source* source = &(data->heat_sources[i]);
                i32 heat =
                    (i32)(((f32)source->temp) *
                          get_heat_multiplier(x, y, source->x, source->y));
                if (heat > max_heat) {
                    max_heat = heat;
                }
            }

            output[y][x] = max_heat;
        }
    }
    f64 after = omp_get_wtime();
    return after - before;
}

i32 main(i32 argc, char** argv) {
    if (argc > 1) {
        i32 thread_count = atoi(argv[1]);
        omp_set_num_threads(thread_count);
    }
    heat_data* data = get_data_from_file();
    i32** output = malloc(sizeof(i32*) * data->dim_y);
    for (i32 i = 0; i < data->dim_y; i++) {
        output[i] = malloc(sizeof(i32) * data->dim_x);
    }

    f64 time_sum = 0;
    for (i32 i = 0; i < REPEAT_COUNT; i++) {
        time_sum += do_it(output, data);
    }

    printf("%fns\n", (time_sum * 1000000) / REPEAT_COUNT);
    write_output_to_file(output, data);

    for (i32 i = 0; i < data->dim_y; i++) {
        free(output[i]);
    }
    free(output);
    free(data->heat_sources);
    free(data);
}