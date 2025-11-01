#include "matrix.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define THREAD_COUNT 8

float random_float(float min, float max) {
    if (min > max) { // Handle cases where min is greater than max
        float temp = min;
        min = max;
        max = temp;
    }
    return ((float)rand() / RAND_MAX) * (max - min) + min;
}

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

    pthread_t thread_ids[THREAD_COUNT];
    row_worker_args tasks[THREAD_COUNT];
    int rows_per_thread = (result->rows + THREAD_COUNT - 1) / THREAD_COUNT;
    int cur_row = 0;

    for(int thread = 0; thread < THREAD_COUNT; thread++) {
        int start = cur_row;
        int end = start +rows_per_thread;
        if (start >= result->rows) {
            thread_ids[thread] = 0;
            continue;
        }
        else if (end > result->rows) {
            end = result->rows;
        }

        tasks[thread] = (row_worker_args) {
            .m1 = m1,
            .m2 = m2,
            .result = result,
            .row_start = start,
            .row_end = end
        };
        pthread_create(&thread_ids[thread], NULL, row_calcs, &tasks[thread]);
        cur_row = end;
    }

    for(int thread = 0; thread < THREAD_COUNT; thread++) {
        if(thread_ids[thread] != 0) {
            pthread_join(thread_ids[thread], NULL);
        }
    }
    return result;
}

//does the dot product calculations for a set of rows
void* row_calcs(void* args) {
    row_worker_args* row_args = (row_worker_args* ) args;
    for (int i = row_args->row_start; i < row_args->row_end; ++i) {
        for (int j = 0; j < row_args->m2->cols; ++j) {
            row_args->result->data[i * row_args->result->cols + j] =
                dot_product(row_args->m1, i, row_args->m2, j);
        }
    }
    return NULL;
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

//forms a matrix given a number of rows and columns, and a min and max value for the entries
matrix* generate_matrix(int rows, int cols, float min, float max) {
    matrix* m = (matrix*) malloc(sizeof(matrix));
    if (m == NULL) {
        return (matrix*) m;
    }
    m->rows = rows;
    m->cols = cols;
    m->data = (float*) malloc(rows * cols * sizeof(float));
    while (m->data == NULL) {
        m->data = (float*) malloc(rows * cols * sizeof(float));
    }

    for (int index = 0; index < rows * cols; index++) {
        m->data[index] = random_float(min, max);
    }

    return m;
}

//returns a newly allocated transposed version of the given matrix
matrix* transpose_matrix(matrix* m) {
    matrix* result = (matrix*) malloc(sizeof(matrix));
    result->cols = m->rows;
    result->rows = m->cols;
    result->data = (float*) malloc(result->cols * result->rows * sizeof(float));
    int index = 0;
    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            result->data[get_index(result, j, i)] = m->data[index];
            index += 1;
        }
    }
    return result;

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

//adds a vector to the matrix, adding each element in the vector to every element in the corresponding matrix row
void extended_add(matrix* m, float* vector) {
    int index = 0;
    for(int i = 0; i < m->rows; i++) {
        for(int j = 0; j < m->cols; j++) {
            m->data[index] += vector[i];
            index++;
        }
    }
}

//generates a vector of dimension 1xsize with values between min and max
float* generate_vector(int size, float min, float max) {
    float* vector = (float*) malloc(size * sizeof(float));
    for(int i = 0; i < size; i++) {
        vector[i] = random_float(min, max);
    }
    return vector;
}

//copies the values in a matrix to another matrix. deep copy. resizes dest to be same as src.
void matrix_copy(matrix* src, matrix* dest) {
    dest->cols = src->cols;
    dest->rows = src->rows;
    dest->data = (float*) realloc(dest->data, sizeof(float) * dest->cols * dest->rows);
    int index = 0;
    while (index < src->cols * src->rows) {
        dest->data[index] = src->data[index];
        index += 1;
    }
}

//copies the values from one vector to another. deep copy. resizes dest to be same as src.
void vector_copy(float* src, float* dest, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

