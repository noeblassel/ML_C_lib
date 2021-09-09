#include "utils.h"

void setup(MultiLayerPerceptron *net, int n_layers, int *layer_dimensions, int *activation_types)
{
    net->n_layers = n_layers;
    net->layers = (Layer *)malloc(net->n_layers * sizeof(Layer));
    net->layer_dims = (int *)malloc((net->n_layers) * sizeof(int));
    net->activation_types = (int *)malloc((net->n_layers) * sizeof(int));

    memcpy(&(net->layer_dims), layer_dimensions, n_layers * sizeof(int));
    memcpy(&(net->activation_types), activation_types, n_layers * sizeof(int));

    for (int i = 0; i < net->n_layers; ++i)
    {
        net->layers[i].units = (Neuron *)malloc(net->layer_dims[i] * sizeof(Neuron));
        if (i > 0)
        {
            net->layers[i].prev = net->layers + i - 1;
            net->layers[i].weights = (float **)malloc(net->layer_dims[i - 1] * sizeof(float *));
            net->layers[i].weights_grad = (float **)malloc(net->layer_dims[i - 1] * sizeof(float *));

            for (int j = 0; j < net->layer_dims[i - 1]; j++)
            {
                net->layers[i].weights[j] = (float *)malloc(net->layer_dims[i] * sizeof(float));
                net->layers[i].weights_grad[j] = (float *)malloc(net->layer_dims[i] * sizeof(float));
            }

            net->layers[i].bias = (float *)malloc(net->layer_dims[i] * sizeof(float));
            net->layers[i].bias_grad = (float *)malloc(net->layer_dims[i] * sizeof(float));
        }
        net->layers[i].activation = activation_functions[net->activation_types[i]];
        net->layers[i].dactivation = derivatives[net->activation_types[i]];
        if (i < net->n_layers - 1)
            net->layers[i].next = net->layers + i + 1;
    }
}

void copy(MultiLayerPerceptron *net_from, MultiLayerPerceptron *net_to)
{

    setup(net_to, net_from->n_layers, net_from->layer_dims, net_from->activation_types);

    for (int i = 1; i < net_from->n_layers; ++i)
    {
        for (int k = 0; k < net_from->layer_dims[i]; k++)
        {
            for (int j = 0; j < net_from->layer_dims[i - 1]; j++)
            {
                net_to->layers[i].weights[j][k] = net_from->layers[i].weights[j][k];
            }
            net_to->layers[i].bias[k] = net_from->layers[i].bias[k];
        }
    }
}

void cleanup(MultiLayerPerceptron *net)
{
    for (int i = 1; i < net->n_layers; ++i)
    {
        for (int j = 0; j < net->layer_dims[i - 1]; j++)
        {
            free(net->layers[i].weights[j]);
            free(net->layers[i].weights_grad[j]);
        }
        free(net->layers[i].weights);
        free(net->layers[i].weights_grad);

        free(net->layers[i].bias);
        free(net->layers[i].bias_grad);

        free(net->layers[i].units);
    }

    free(net->layers[0].units);
    free(net->layers);
    free(net->layer_dims);
    free(net->activation_types);
}