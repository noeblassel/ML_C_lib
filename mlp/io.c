#include "io.h"

char *write_to_z85(MultiLayerPerceptron *net)
{
    size_t n_bytes;
    byte *data = write_to_bytes(net, &n_bytes);
    char *encoded_data = z85_encode(data, n_bytes);
    free(data);

    return encoded_data;
}

byte *write_to_bytes(MultiLayerPerceptron *net, size_t *buffer_length)
{
    size_t n_bytes = 0;
    n_bytes += sizeof net->n_layers;
    for (int i = 0; i < net->n_layers; ++i)
        n_bytes += sizeof net->layer_dims[i];

    for (int i = 0; i < net->n_layers; ++i)
        n_bytes += sizeof net->activation_types[i];

    for (int i = 1; i < net->n_layers; ++i)
    {
        for (int k = 0; k < net->layer_dims[i]; ++k)
        {
            for (int j = 0; j < net->layer_dims[i - 1]; ++j)
                n_bytes += sizeof net->layers[i].weights[j][k];
            n_bytes += sizeof net->layers[i].bias[k];
        }
    }

    byte *data = malloc(n_bytes);
    byte *copy_ptr = data;

    memcpy(copy_ptr, &(net->n_layers), sizeof net->n_layers);
    copy_ptr += sizeof net->n_layers;

    for (int i = 0; i < net->n_layers; ++i)
    {
        memcpy(copy_ptr, &(net->layer_dims[i]), sizeof net->layer_dims[i]);
        copy_ptr += sizeof net->layer_dims[i];
    }
    for (int i = 0; i < net->n_layers; ++i)
    {
        memcpy(copy_ptr, &(net->activation_types[i]), sizeof net->activation_types[i]);
        copy_ptr += sizeof net->activation_types[i];
    }
    for (int i = 1; i < net->n_layers; ++i)
    {
        for (int k = 0; k < net->layer_dims[i]; ++k)
        {
            for (int j = 0; j < net->layer_dims[i - 1]; ++j)
            {
                memcpy(copy_ptr, &(net->layers[i].weights[j][k]), sizeof net->layers[i].weights[j][k]);
                copy_ptr += sizeof net->layers[i].weights[j][k];
            }
            memcpy(copy_ptr, &(net->layers[i].bias[k]), sizeof net->layers[i].bias[k]);
            copy_ptr += sizeof net->layers[i].bias[k];
        }
    }
    assert(copy_ptr == data + n_bytes);
    *buffer_length = n_bytes;
    return data;
}

void read_from_z85(MultiLayerPerceptron *net, char *z85_string)
{
    size_t n_bytes;
    byte *decoded_data = z85_decode(z85_string);
    read_from_bytes(net, decoded_data, &n_bytes);
    assert(n_bytes == strlen(z85_string) * 4 / 5);
    free(decoded_data);
}

void read_from_bytes(MultiLayerPerceptron *net, byte *byte_array, size_t *end_position)
{
    byte *read_ptr = byte_array;
    int num_layers;
    memcpy(&num_layers, read_ptr, sizeof(int));
    read_ptr += sizeof(int);
    int *layer_dimensions = (int *)malloc(num_layers * sizeof(int));
    int *activation_types = (int *)malloc(num_layers * sizeof(int));
    for (int i = 0; i < num_layers; ++i)
    {
        memcpy(layer_dimensions + i, read_ptr, sizeof(int));
        read_ptr += sizeof(int);
    }
    for (int i = 0; i < num_layers; ++i)
    {
        memcpy(activation_types + i, read_ptr, sizeof(int));
        read_ptr += sizeof(int);
    }
    setup(net, num_layers, layer_dimensions, activation_types);

    for (int i = 1; i < net->n_layers; ++i)
    {
        for (int k = 0; k < net->layer_dims[i]; ++k)
        {
            for (int j = 0; j < net->layer_dims[i - 1]; ++j)
            {
                memcpy(&(net->layers[i].weights[j][k]), read_ptr, sizeof(float));
                read_ptr += sizeof(float);
            }
            memcpy(&(net->layers[i].bias[k]), read_ptr, sizeof(float));
            read_ptr += sizeof(float);
        }
    }
    *end_position = (size_t)(read_ptr - byte_array);
    free(layer_dimensions);
    free(activation_types);
}

void save_to_z85_file(MultiLayerPerceptron *net, char *filename)
{
    char *encoded_data = write_to_z85(net);
    FILE *fp = fopen(filename, "w");
    fwrite(encoded_data, 1, strlen(encoded_data), fp);
    fclose(fp);
    free(encoded_data);
}

void load_from_z85_file(MultiLayerPerceptron *net, char *filename)
{
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    char *encoded_data = (char *)malloc((len + 1) * sizeof(char));
    fseek(fp, 0, SEEK_SET);
    fread(encoded_data, sizeof(char), len, fp);
    encoded_data[len] = 0;
    read_from_z85(net, encoded_data);
    free(encoded_data);
    fclose(fp);
}

void save_to_bin_file(MultiLayerPerceptron *net, char *filename)
{
    size_t n_bytes;
    byte *net_description = write_to_bytes(net, &n_bytes);
    FILE *fp = fopen(filename, "wb");
    fwrite(net_description, 1, n_bytes, fp);
    fclose(fp);
    free(net_description);
}

void load_from_bin_file(MultiLayerPerceptron *net, char *filename)
{
    int n_layers;
    FILE *fp = fopen(filename, "rb");
    fread(&n_layers, sizeof(int), 1, fp);
    int *layer_dims = (int *)malloc(n_layers * sizeof(int));
    fread(layer_dims, sizeof(int), n_layers, fp);
    size_t n_bytes = sizeof(int) * (1 + n_layers + n_layers);
    for (int i = 1; i < n_layers; ++i)
    {
        n_bytes += layer_dims[i] * (1 + layer_dims[i - 1]) * sizeof(float);
    }
    fseek(fp, 0, SEEK_SET);
    free(layer_dims);
    byte *bin_data = (byte *)malloc(n_bytes);
    fread(bin_data, 1, n_bytes, fp);
    size_t end_pos;
    read_from_bytes(net, bin_data, &end_pos);
    assert(end_pos == n_bytes);
    free(bin_data);
    fclode(fp);
}