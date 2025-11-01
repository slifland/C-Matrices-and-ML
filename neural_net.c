#include "neural_net.h"
#include "matrix.h"
#include <stddef.h>
#include <math.h>

//initializes a network with weights and biases between -1.0f and 1.0f
neural_net* init_network(int input_size, int output_size, int hidden_size, int hidden_layers, int batch_size) {
    neural_net* result = (neural_net*) malloc(sizeof(neural_net));
    if (result == NULL) {
        return (neural_net*) NULL;
    }

    result->input_size = input_size;
    result->output_size = output_size;
    result->hidden_size = hidden_size;
    result->hidden_layers = hidden_layers;
    result->batch_size = batch_size;
    
    result->weights = (matrix**) malloc(sizeof(matrix*) * (hidden_layers + 1));
    result->biases = (float**) malloc(sizeof(float*) * (hidden_layers + 1));

    //initialize the weights and biases
    if (hidden_layers > 0) {
        int in = input_size;
        int out = hidden_size;
        for(int i = 0; i < hidden_layers; i++) {
            result->weights[i] = generate_matrix(out, in, -1.0f, 1.0f);
            result->biases[i] = generate_vector(out, -1.0f, 1.0f);
            in = out;
        }
        result->weights[hidden_layers] = generate_matrix(output_size, hidden_size, -1.0f, 1.0f);
        result->biases[hidden_layers] = generate_vector(output_size, -1.0f, 1.0f);
    }
    //no hidden layers
    else {
        result->weights[0] = generate_matrix(output_size, input_size, -1.0f, 1.0f);
        result->biases[0] = generate_vector(output_size, -1.0f, 1.0f);
    }
    return result;
}

//does a forward pass, returns the outputs as a vector
//currently assumes inputs is input_sizex1, no batching for now
matrix* forward_pass(neural_net* net, matrix* inputs) {
    matrix* temp_representation = (matrix*) malloc (sizeof(matrix));
    temp_representation->data = NULL;
    matrix* temp_result = NULL;

    matrix_copy(inputs, temp_representation);
    for(int i = 0; i < net->hidden_layers + 1; i++) {
        if(net->weights[i]->rows * net->weights[i]->cols > 20) {
            temp_result = multiply_matrices_parallel(net->weights[i], temp_representation);
        }
        else {
            temp_result = multiply_matrices(net->weights[i], temp_representation);
        }
        extended_add(temp_result, net->biases[i]);
        activation_function(temp_result);
        matrix_copy(temp_result, temp_representation);
        free_matrix(temp_result);
    }
    matrix* retval = (matrix*) malloc(sizeof(matrix));
    retval->rows = net->output_size;
    retval->cols = net->batch_size;
    retval->data = (float*) malloc(retval->rows * retval->cols * sizeof(float));
    matrix_copy(temp_representation, retval);
    free_matrix(temp_representation);
    return retval;
}

//applies activation function to all elements of a matrix m
void activation_function(matrix* m) {
    int index = 0;
    while (index < m->rows * m->cols) {
        m->data[index] = 1 / (1 + exp(-1 * m->data[index]));
        index += 1;
    }
}

//frees a neural network and its associated memory
void free_network(neural_net* net) {

    for(int i = 0; i < net->hidden_layers + 1; i++) {
        free_matrix(net->weights[i]);
        free(net->biases[i]);
    }

    free(net->weights);
    free(net->biases);
    free(net);
}