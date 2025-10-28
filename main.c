#include "matrix.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


float time_matrix_multiplication(matrix* (matrix_func)(matrix*, matrix*), matrix* m1, matrix* m2) {
    clock_t start = clock();

    matrix_func(m1, m2);

    clock_t end = clock();

    float cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;

}

int main() {
    srand(time(NULL));
    matrix* (*matrix_func)(matrix*, matrix*) = &multiply_matrices;

    for (int i = 10; i <= 1000; i += 100) {
        matrix* m1 = generate_matrix(i, i, -1.0f, 1.0f);

        matrix* m2 = generate_matrix(i, i, -1.0f, 1.0f);
    
        matrix* result = multiply_matrices(m1, m2);

        float time = time_matrix_multiplication(matrix_func, m1, m2);
        printf("The time taken for this multiplication of (%d x %d) and (%d x %d) was: %f\n", i, i, i, i, time);

        free_matrix(m1);
        free_matrix(m2);
        free_matrix(result);
    }
    return 0;
}