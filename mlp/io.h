#ifndef __MLP_IO_H__
#define __MLP_IO_H__
/*
THIS HEADER FILE
*/

#include "../multi_layer_perceptron.h"
#include "../base85.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *write_to_z85(MultiLayerPerceptron *net);
byte *write_to_bytes(MultiLayerPerceptron *net,size_t *buffer_length);

void read_from_z85(MultiLayerPerceptron *net, char *z85_string);
void read_from_bytes(MultiLayerPerceptron *net,byte *byte_array,size_t *end_position);

void save_to_z85_file(MultiLayerPerceptron *net, char *filename);
void load_from_z85_file(MultiLayerPerceptron *net, char *filename);

void save_to_bin_file(MultiLayerPerceptron *net, char *filename);
void load_from_bin_file(MultiLayerPerceptron *net, char *filename);

#endif