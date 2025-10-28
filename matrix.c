#include "matrix.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//multiplies two matrices and returns the resulting matrix
matrix* multiply_matrices(matrix* m1, matrix* m2) {
    //check if this is a valid multiplication
    if (m1->cols != m2->rows) {
        return (matrix*) NULL;
    }
    matrix* result = (matrix*) malloc(sizeof(matrix));
    if (result == NULL) {
        return (matrix*) result;
    }

    result->rows = m1->rows;
    result->cols = m2->cols;
    result->data = (float*) malloc(result->rows * result->cols * sizeof(float));
    while (result->data == NULL) {
        result->data = (float*) malloc(result->rows * result->cols * sizeof(float));
    }

    int index = 0;
    //organized like this for readability, could be a 1d loop
    for (int i = 0; i < m1->rows; i++) {
        for (int j = 0; j < m2->cols; j++) {
            //entry at this index equals dot product of m1 row i and m2 col j
            result->data[index] = dot_product(m1, i, m2, j);
            index += 1;
        }
    }
    return result;
}

//multiplies two matrices and returns the resulting matrix, using threads for parallelization
matrix* multiply_matrices_parallel(matrix* m1, matrix* m2) {
    return (matrix*) NULL;
}

//forms a matrix given a number of rows and columns, and the numbers arranged in order
matrix* form_matrix(int rows, int cols, ...) {
    matrix* m = (matrix*) malloc(sizeof(matrix));
    if (m == NULL) {
        return (matrix*) m;
    }
    m->rows = rows;
    m->cols = cols;
    m->data = (float*) malloc(rows * cols * sizeof(float));

    va_list args;
    va_start(args, cols);
    for (int i = 0; i < rows * cols; i++) {
        m->data[i] = va_arg(args, double);
    }
    va_end(args);

    return m;
}

//calculates dot product of row i of matrix m1 and column j of matrix m2
float dot_product(matrix* m1, int i, matrix* m2, int j) {
    float value = 0;
    for (int entry = 0; entry < m1-> cols; entry++) {
        value += m1->data[entry + i * m1->cols] * m2->data[entry * m2->cols + j];
    }
    return value;
}



//returns the 1d index associated with a 2d i, j
int get_index(matrix* m, int i, int j) {
    return m->cols * i + j;
}

//frees a matrix and its associated data
void free_matrix(matrix* m) {
    free(m->data);
    free(m);
}

//prints the matrix in a human readable format
void print_matrix(matrix* m) {
    int index;
    for (int i = 0; i < m->rows; i++) {
        printf("[");
        for (int j = 0; j < m->cols; j++) {
            index = get_index(m, i, j);
            printf(" %f ", m->data[index]);
        }
        printf("]\n");
    }
}

