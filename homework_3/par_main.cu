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

__global__
void dot_product(float *mat1, float *mat2, float *tmp, int *mat_size, float *result) {
    tmp[threadIdx.x] = mat1[threadIdx.x] * mat2[threadIdx.x];
    __syncthreads();

    if(threadIdx.x == 0) {
        float sum = 0;
        for(int i = 0; i < (*mat_size); i++) {
            sum += tmp[i];
        }
        *result = sum;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Please provide file name!");
        exit(1);
    }

    float *mat1, *mat2;
    int mat_size;
    fetch_data(argv[1], &mat1, &mat2, &mat_size);
    
    float *cuda_mat1, *cuda_mat2, *cuda_tmp, *cuda_result;
    int *cuda_mat_size;
    cudaMalloc(&cuda_mat1, sizeof(float) * mat_size);
    cudaMalloc(&cuda_mat2, sizeof(float) * mat_size);
    cudaMalloc(&cuda_tmp, sizeof(float) * mat_size);
    cudaMalloc(&cuda_mat_size, sizeof(int));
    cudaMalloc(&cuda_result, sizeof(float));
    cudaMemcpy(cuda_mat1, mat1, sizeof(float) * mat_size, cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_mat2, mat2, sizeof(float) * mat_size, cudaMemcpyHostToDevice);
    cudaMemcpy(cuda_mat_size, &mat_size, sizeof(int), cudaMemcpyHostToDevice);

    double time_total = 0;
    for (int i = 0; i < TRY_COUNT; i++) {
        double start = omp_get_wtime();
        dot_product<<< 1, mat_size >>>(cuda_mat1, cuda_mat2, cuda_tmp, cuda_mat_size, cuda_result);
        cudaDeviceSynchronize();
        double end = omp_get_wtime();
        time_total += end - start;
    }

    float result;
    cudaMemcpy(&result, cuda_result, sizeof(float), cudaMemcpyDeviceToHost);
    printf("Result: %f\n", result);
    printf("Time: %fus\n", 1000000 * (time_total / ((double) TRY_COUNT)));
    cudaFree(cuda_mat1);
    cudaFree(cuda_mat2);
    cudaFree(cuda_tmp);
    cudaFree(cuda_mat_size);
    cudaFree(cuda_result);
    return 0;
}
