#ifndef __NEURON_H__
#define __NEURON_H__

typedef struct neuron_t
{
    float weighted_input_value;
    float activation_value;
    float delta;
} Neuron;

#endif