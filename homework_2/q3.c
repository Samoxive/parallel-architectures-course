#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

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
    fscanf(file, "Dimensions:%d X %d", &dim_x, &dim_y);
    data->dim_x = dim_x;
    data->dim_y = dim_y;
    i32 heat_count;
    fscanf(file, "Number of Heat Source: %d", &heat_count);
    data->heat_count = heat_count;
    fscanf(file, "%*[^\n]\n");
    data->heat_sources = malloc(sizeof(heat_source) * heat_count);
    printf("%d %d %d\n", dim_x, dim_y, heat_count);
    for (i32 i = 0; i < heat_count; i++) {
        heat_source source;
        scanf("(%d, %d, %d)", &source.x, &source.y, &source.temp);
        data->heat_sources[i] = source;
        printf("%d %d %d\n", source.x, source.y, source.temp);
    }

    fclose(file);
    return data;
}

i32 main(i32 argc, char** argv) {
    get_data_from_file();
}