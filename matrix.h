/*
This header file defines the outline for libmat.a, which contains the matrix struct as well as matrix functions.
*/

#ifndef MATRIX_H
#define MATRIX_H

#include <stdarg.h>

//matrix consisting of rows and columns, stored in data
typedef struct {
    int rows;
    int cols;
    float* data;
} matrix;

//struct for dot product thread creation, containing the necessary arguments
typedef struct {
    matrix *m1;
    matrix *m2;
    matrix *result;
    int row_start;
    int row_end;
} row_worker_args;

float random_float(float min, float max);

//multiplies two matrices and returns the resulting matrix
matrix* multiply_matrices(matrix* m1, matrix* m2);

//multiplies two matrices and returns the resulting matrix, using threads for parallelization
matrix* multiply_matrices_parallel(matrix* m1, matrix* m2);

//does the dot product calculations for a set of rows
void* row_calcs(void* args);

//forms a matrix given a number of rows and columns, and the numbers arranged in order
matrix* form_matrix(int rows, int cols, ...);

//forms a matrix given a number of rows and columns, and a min and max value for the entries
matrix* generate_matrix(int rows, int cols, float min, float max);

//returns a newly allocated transposed version of the given matrix
matrix* transpose_matrix(matrix* m);

//calculates dot product of row i of matrix m1 and column j of matrix m2
float dot_product(matrix* m1, int i, matrix* m2, int j);

//returns the 1d index associated with a 2d i, j
int get_index(matrix* m, int i, int j);

//frees a matrix and its associated data
void free_matrix(matrix* m);

//prints the matrix in a human readable format
void print_matrix(matrix* m);

//adds a vector to the matrix, adding each element in the vector to every element in the corresponding matrix row
void extended_add(matrix* m, float* vector);

//generates a vector of dimension 1xsize with values between min and max
float* generate_vector(int size, float min, float max);

//copies the values in a matrix to another matrix. deep copy. resizes dest to be same as src.
void matrix_copy(matrix* src, matrix* dest);

//copies the values from one vector to another. deep copy. resizes dest to be same as src.
void vector_copy(float* src, float* dest, int len);

#endif /* MATRIX_H */

