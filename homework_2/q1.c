#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

typedef struct {
    f64 time;
    f64 result;
} integration_result;

const f64 CONST_E = 2.71828;
const f64 CONST_PI = 3.14159;
const i32 REPEAT_COUNT = 512;

f64 graph_function(f64 x) {
    return (3 * CONST_E * x * x * x) + (9 * x * x) - (7.14 * x);
}

f64 calc_rect(f64 x1, f64 x2) {
    f64 x = (x1 + x2) / 2;
    f64 y = graph_function(x);
    f64 d_x = x2 - x1;
    f64 area = d_x * y;

    return area;
}

integration_result do_it() {
    f64 x1 = -20, x2 = 20;
    i32 rect_count = 1000000;
    f64 increment_x = (x2 - x1) / (f64)rect_count;

    f64 sum = 0;
    f64 before = omp_get_wtime();

#pragma omp parallel for reduction(+ : sum)
    for (i32 i = 0; i < rect_count; i++) {
        f64 local_result =
            calc_rect(x1 + (increment_x * i), x1 + (increment_x * (i + 1)));
        sum += local_result;
    }

    f64 after = omp_get_wtime();
    integration_result r;
    r.result = sum;
    r.time = after - before;
    return r;
}

int main(i32 argc, char** argv) {
    if (argc > 1) {
        i32 thread_count = atoi(argv[1]);
        omp_set_num_threads(thread_count);
    }

    integration_result last_result;
    f64 time_sum = 0;
    for (i32 i = 0; i < REPEAT_COUNT; i++) {
        last_result = do_it();
        time_sum += last_result.time;
    }

    printf("%fus\n", (time_sum * 1000000) / REPEAT_COUNT);
}