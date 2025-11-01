/*
This header file defines the outline for libnet.a, which contains the neural network struct as well as relevant functions.
*/
#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
//neural net consisting of multiple layers, and relevant metadata
typedef struct {
    int batch_size;
    int hidden_layers;
    int hidden_size;     //for now, all hidden layers are same size
    int input_size;
    int output_size;
    matrix** weights; //stores the weight matrices for this neural network. each layer has its own matrix.
    float** biases; //stores the bias vectors for this neural network. each layer has its own bias vector.
} neural_net;

//initializes a network with weights and biases between -1.0f and 1.0f
neural_net* init_network(int input_size, int output_size, int hidden_size, int hidden_layers, int batch_size);

//does a forward pass, returns the outputs as a vector
matrix* forward_pass(neural_net* net, matrix* inputs);

//calculates loss given predictions and ground truth values
void calculate_loss(float* predictions, float* gt_values);

//applies activation function to all elements of a matrix m
void activation_function(matrix* m);

//frees a neural network and its associated memory
void free_network(neural_net* net);

#endif /* NEURAL_NET_H */
