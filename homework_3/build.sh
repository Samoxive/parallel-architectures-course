nvcc -Xcompiler -fopenmp par_main.cu -o par_main
nvcc -Xcompiler -fopenmp par_main2.cu -o par_main2
gcc -fopenmp main.c -o main
