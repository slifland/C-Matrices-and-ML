#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

#include <time.h>

double time_matrix_multiplication(matrix *(*matrix_func)(matrix *, matrix *),
                                  matrix *m1,
                                  matrix *m2) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    matrix *result = matrix_func(m1, m2);

    clock_gettime(CLOCK_MONOTONIC, &end);

    if (result) {
        free_matrix(result);
    }

    double elapsed_sec  = (double)(end.tv_sec - start.tv_sec);
    double elapsed_nsec = (double)(end.tv_nsec - start.tv_nsec);
    return elapsed_sec + elapsed_nsec * 1e-9;
}

void time_test(int start, int end, int inc) {
    matrix* (*matrix_func)(matrix*, matrix*) = &multiply_matrices;
    matrix* (*matrix_func_parallel)(matrix*, matrix*) = &multiply_matrices_parallel;

    for (int i = start; i <= end; i += inc) {
        matrix* m1 = generate_matrix(i, i, -1.0f, 1.0f);

        matrix* m2 = generate_matrix(i, i, -1.0f, 1.0f);
    
        float time = time_matrix_multiplication(matrix_func, m1, m2);
        printf("The time taken for this multiplication of (%d x %d) and (%d x %d) was: %f\n", i, i, i, i, time);

        time = time_matrix_multiplication(matrix_func_parallel, m1, m2);
        printf("The time taken for this parallel multiplication of (%d x %d) and (%d x %d) was: %f\n", i, i, i, i, time);

        free_matrix(m1);
        free_matrix(m2);
    }
}


int main() {
    srand(time(NULL));

    time_test(1000, 1001, 100);

    return 0;
}