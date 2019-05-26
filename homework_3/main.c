#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int TRY_COUNT = 512;

void fetch_data(char *file_name, float **mat1, float **mat2, int *mat_size) {
    FILE *file = fopen(file_name, "r");

    fscanf(file, "SizeA= %d", mat_size);
    float *matA = (float*) malloc(sizeof(float) * (*mat_size));
    for (int i = 0; i < (*mat_size); i++) {
        fscanf(file, "%f", &(matA[i]));
    }

    fscanf(file, " ");
    
    fscanf(file, "SizeB= %d", mat_size);
    float *matB = (float*) malloc(sizeof(float) * (*mat_size));
    for (int i = 0; i < (*mat_size); i++) {
        fscanf(file, "%f", &(matB[i]));
    }

    *mat1 = matA;
    *mat2 = matB;
    fclose(file);
}

float dot_product(float *mat1, float *mat2, int mat_size) {
    float sum = 0;
    for (int i = 0; i < mat_size; i++) {
        sum += mat1[i] * mat2[i];
    }

    return sum;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide file name!");
        exit(1);
    }

    float *mat1, *mat2;
    int mat_size;
    fetch_data(argv[1], &mat1, &mat2, &mat_size);
    double time_total = 0;
    for (int i = 0; i < TRY_COUNT; i++) {
        double start = omp_get_wtime();
        dot_product(mat1, mat2, mat_size);
        double end = omp_get_wtime();
        time_total += end - start;
    }

    printf("Result: %f\n", dot_product(mat1, mat2, mat_size));
    printf("Time: %fus\n", 1000000 * (time_total / ((double) TRY_COUNT)));
    free(mat1);
    free(mat2);
    return 0;
}
