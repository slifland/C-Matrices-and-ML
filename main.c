#include "matrix.h"

int main() {
    matrix* m1 = form_matrix(2, 2, 2.0f, 1.5f, 2.0f, 2.0f);

    matrix* m2 = form_matrix(2, 3, -2.0f, 2.0f, 4.0f, 2.0f, 3.0f, 0.5f);
    
    matrix* result = multiply_matrices(m1, m2);
    print_matrix(result);

    free_matrix(m1);
    free_matrix(m2);
    free_matrix(result);
}