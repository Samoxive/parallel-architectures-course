#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

const i32 REPEAT_COUNT = 512;
const i32 ARRAY_SIZE = 8192;
const i32 RECURSION_SIZE_LIMIT = 32;
const i32 FILE_ARRAY_SIZE = 100000;

i32* get_random_array(i32 size) {
    i32* arr = malloc(sizeof(i32) * size);
    for (i32 i = 0; i < size; i++) {
        arr[i] = rand() % 500;
    }

    return arr;
}

i32* get_array_from_file() {
    i32* arr = malloc(sizeof(i32) * FILE_ARRAY_SIZE);
    FILE* file = fopen("input1.txt", "r");
    for (i32 i = 0; i < FILE_ARRAY_SIZE; i++) {
        i32 elem;
        fscanf(file, "%d", &elem);
        arr[i] = elem;
    }
    fclose(file);
    return arr;
}

void output_array_to_file(i32* arr) {
    FILE* file = fopen("output1.txt", "w");
    for (i32 i = 0; i < FILE_ARRAY_SIZE; i++) {
        fprintf(file, "%d\n", arr[i]);
    }
    fclose(file);
}

void insertion_sort(i32* arr, i32 l, i32 r) {
    for (i32 i = l + 1; i < r; i++) {
        i32 key = arr[i];
        i32 j = i - 1;

        while (j >= l && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(i32* arr, i32 l, i32 m, i32 r) {
    i32 n1 = m - l;
    i32 n2 = r - m;

    i32* L = malloc(sizeof(i32) * n1);
    i32* R = malloc(sizeof(i32) * n2);

    for (i32 i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }

    for (i32 i = 0; i < n2; i++) {
        R[i] = arr[m + i];
    }

    i32 i = 0, j = 0;
    i32 k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_sort(i32* arr, i32 l, i32 r) {
    if ((r - l) > RECURSION_SIZE_LIMIT) {
        i32 m = l + (r - l) / 2;

#pragma omp task
        merge_sort(arr, l, m);
#pragma omp task
        merge_sort(arr, m, r);
#pragma omp taskwait
        merge(arr, l, m, r);
    } else if (r - l > 1) {
        insertion_sort(arr, l, r);
    }
}

f64 do_it(i32* arr) {
    f64 before = omp_get_wtime();
#pragma omp parallel
#pragma omp single
    merge_sort(arr, 0, FILE_ARRAY_SIZE);
    f64 after = omp_get_wtime();
    return after - before;
}

i32 main(i32 argc, char** argv) {
    omp_set_nested(1);
    if (argc > 1) {
        i32 thread_count = atoi(argv[1]);
        omp_set_num_threads(thread_count);
    }

    // i32* arr = get_random_array(ARRAY_SIZE);
    i32* arr = malloc(sizeof(i32) * FILE_ARRAY_SIZE);
    i32* orig_arr = get_array_from_file();
    f64 time_sum = 0;
    for (i32 i = 0; i < REPEAT_COUNT; i++) {
        for (i32 k = 0; k < FILE_ARRAY_SIZE; k++) {
            arr[k] = orig_arr[k];
        }
        time_sum += do_it(arr);
    }

    output_array_to_file(arr);
    printf("%fus\n", (time_sum * 1000000) / REPEAT_COUNT);
    return 0;
}