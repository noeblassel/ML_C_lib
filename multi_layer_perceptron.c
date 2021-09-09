#include "multi_layer_perceptron.h"

void initialize_weights(MultiLayerPerceptron *net)
{
    for (int i = 1; i < net->n_layers; ++i)
    {
        float alpha = (float)sqrt(3. / ((float)net->layer_dims[i - 1]));
        for (int k = 0; k < net->layer_dims[i]; k++)
        {
            for (int j = 0; j < net->layer_dims[i - 1]; j++)
            {
                net->layers[i].weights[j][k] = randf(-alpha, alpha);
            }
            net->layers[i].bias[k] = 0;
        }
    }
}

void forward(MultiLayerPerceptron *net, float *input)
{
    for (int k = 0; k < net->layer_dims[0]; k++)
    {
        net->layers[0].units[k].weighted_input_value = input[k];
        net->layers[0].units[k].activation_value = (*(net->layers[0].activation))(&(net->layers[0].units[k]));
    }
    for (int i = 1; i < net->n_layers; ++i)
    {
        for (int k = 0; k < net->layer_dims[i]; k++)
        {
            net->layers[i].units[k].weighted_input_value = 0;
            for (int j = 0; j < net->layer_dims[i - 1]; j++)
            {
                net->layers[i].units[k].weighted_input_value += (net->layers[i].weights[j][k]) * (net->layers[i - 1].units[j].activation_value);
            }
            net->layers[i].units[k].weighted_input_value += net->layers[i].bias[k];
            net->layers[i].units[k].activation_value = (*(net->layers[i].activation))(&(net->layers[i].units[k]));
        }
    }
}

void backward(MultiLayerPerceptron *net, float *input, float *output, bool recompute_activations)
{
    if (recompute_activations)
        forward(net, input);
    for (int k = 0; k < net->layer_dims[net->n_layers - 1]; ++k)
    {
        net->layers[net->n_layers - 1].units[k].delta = (*(net->layers[net->n_layers - 1].dactivation))(&(net->layers[net->n_layers - 1].units[k])) * (net->layers[net->n_layers - 1].units[k].activation_value - output[k]);
    }

    for (int i = net->n_layers - 2; i > 0; i--)
    {
        for (int j = 0; j < net->layer_dims[i]; j++)
        {
            float _delta = 0;
            for (int k = 0; k < net->layer_dims[i + 1]; k++)
            {
                _delta += (net->layers[i + 1].units[k].delta) * (net->layers[i + 1].weights[j][k]);
            }
            _delta *= (*(net->layers[i].dactivation))(&(net->layers[i].units[j]));
            net->layers[i].units[j].delta = _delta;
        }
    }

    for (int i = net->n_layers - 1; i > 0; i--)
    {
        for (int j = 0; j < net->layer_dims[i - 1]; j++)
        {
            for (int k = 0; k < net->layer_dims[i]; k++)
            {
                net->layers[i].weights_grad[j][k] = (net->layers[i - 1].units[j].activation_value) * (net->layers[i].units[k].delta);
            }
        }

        for (int k = 0; k < net->layer_dims[i]; k++)
        {
            net->layers[i].bias_grad[k] = net->layers[i].units[k].delta;
        }
    }
}

void gradient_step(MultiLayerPerceptron *net, float lr, bool descent)
{
    int dir = (descent) ? -1 : 1;
    for (int i = 1; i < net->n_layers; i++)
    {
        for (int j = 0; j < net->layer_dims[i - 1]; j++)
        {
            for (int k = 0; k < net->layer_dims[i]; k++)
            {
                net->layers[i].weights[j][k] += dir * lr * net->layers[i].weights_grad[j][k];
            }
        }
        for (int k = 0; k < net->layer_dims[i]; k++)
        {
            net->layers[i].bias[k] += dir * lr * net->layers[i].bias_grad[k];
        }
    }
}

void predict(MultiLayerPerceptron *net, float *input)
{
    forward(net, input);
    for (int i = 0; i < net->layer_dims[net->n_layers - 1]; ++i)
    {
        printf("%d", (net->layers[net->n_layers - 1].units[i].activation_value > .5) ? 1 : 0);
    }
    printf("\n");
}