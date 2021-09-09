#ifndef __LAYER_H__
#define __LAYER_H__

typedef struct layer_t
{
    int dim;
    struct layer_t *prev;
    struct layer_t *next;
    Neuron *units;

    float **weights;
    float *bias;

    float **weights_grad;
    float *bias_grad;

    neural_function_t activation;
    neural_function_t dactivation;
} Layer;

#endif