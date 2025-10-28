/*
This header file defines the outline for matrix.h, which contains the matrix struct as well as matrix functions.
*/

#include <stdarg.h>

//matrix consisting of rows and columns, stored in data
typedef struct {
    int rows;
    int cols;
    float* data;
} matrix;

//multiplies two matrices and returns the resulting matrix
matrix* multiply_matrices(matrix* m1, matrix* m2);

//multiplies two matrices and returns the resulting matrix, using threads for parallelization
matrix* multiply_matrices_parallel(matrix* m1, matrix* m2);

//forms a matrix given a number of rows and columns, and the numbers arranged in order
matrix* form_matrix(int rows, int cols, ...);

//calculates dot product of row i of matrix m1 and column j of matrix m2
float dot_product(matrix* m1, int i, matrix* m2, int j);

//returns the 1d index associated with a 2d i, j
int get_index(matrix* m, int i, int j);

//frees a matrix and its associated data
void free_matrix(matrix* m);

//prints the matrix in a human readable format
void print_matrix(matrix* m);


