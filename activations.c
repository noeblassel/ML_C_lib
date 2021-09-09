#include "activations.h"
#include <math.h>

float sigmoid(Neuron *unit)
{
    if (unit->weighted_input_value > 0)
    {
        return 1. / (1 + (float)exp(-unit->weighted_input_value));
    }
    else
    {
        float s = (float)exp(unit->weighted_input_value);
        return s / (1 + s);
    }
}
float dsigmoid(Neuron *unit)
{
    return (unit->activation_value) * (1 - unit->activation_value);
}

float relu(Neuron *unit)
{
    return (unit->weighted_input_value > 0) ? unit->weighted_input_value : 0.0;
}
float drelu(Neuron *unit)
{
    return (float)(unit->weighted_input_value > 0);
}

float tanh_nn(Neuron *unit)
{
    return tanhf(unit->weighted_input_value);
}
float dtanh_nn(Neuron *unit)
{
    return 1 - (unit->activation_value) * (unit->activation_value);
}
float identity(Neuron *unit)
{
    return unit->weighted_input_value;
}
float didentity(Neuron *unit)
{
    return 1.0;
}
float lk_relu(Neuron *unit)
{
    return (unit->weighted_input_value) * (0.01 + 0.99 * (unit->weighted_input_value > 0));
}
float dlk_relu(Neuron *unit)
{
    return 0.01 + 0.99 * (unit->weighted_input_value > 0);
}