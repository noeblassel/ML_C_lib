#ifndef __ACTIVATIONS_H__
#define __ACTIVATIONS_H__

#include "neuron.h"

#define SIGMOID 0
#define RELU 1
#define TANH 2
#define LINEAR 3
#define LK_RELU 4

typedef float (*neural_function_t)(Neuron *);

float sigmoid(Neuron *unit);
float dsigmoid(Neuron *unit);
float relu(Neuron *unit);
float drelu(Neuron *unit);
float tanh_nn(Neuron *unit);
float dtanh_nn(Neuron *unit);
float identity(Neuron *unit);
float didentity(Neuron *unit);
float lk_relu(Neuron *unit);
float dlk_relu(Neuron *unit);

const neural_function_t activation_functions[5] = {sigmoid, relu, tanh_nn, identity, lk_relu}; //must respect order of macros defined above
const neural_function_t derivatives[5] = {dsigmoid, drelu, dtanh_nn, didentity, dlk_relu};

#endif