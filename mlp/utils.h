#ifndef __MLP_UTILS_H__
#define __MLP_UTILS_H__

#include "multi_layer_perceptron.h"

void setup(MultiLayerPerceptron *net, int n_layers, int *layer_dimensions, int *activation_types);

void copy(MultiLayerPerceptron *net_from, MultiLayerPerceptron *net_to);

void cleanup(MultiLayerPerceptron *net);

#endif