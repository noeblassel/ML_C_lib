#ifndef __MULTI_LAYER_PERCEPTRON_H__
#define __MULTI_LAYER_PERCEPTRON_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "neuron.h"
#include "layer.h"
#include "rng.h"
#include "activations.h"

#include "mlp/utils.h"
#include "mlp/io.h"

typedef struct mlp_t
{
    int n_layers;
    int *layer_dims;
    Layer *layers;
    int *activation_types;
    float lr;
} MultiLayerPerceptron;

void initialize_weights(MultiLayerPerceptron *net);

void forward(MultiLayerPerceptron *net, float *input);
void backward(MultiLayerPerceptron *net, float *input, float *output, bool recompute_activations);

void gradient_step(MultiLayerPerceptron *net, float lr, bool descent);
void predict(MultiLayerPerceptron *net, float *input);



#endif