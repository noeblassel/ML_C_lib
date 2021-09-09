#include "base85.h"

char *z85_encode(byte *data, size_t len)
{
    assert(len%4==0);
    size_t num_chars = len * 5 / 4;
    char *encoded_data = (char *)malloc(num_chars + 1);
    uint char_idx = 0;
    uint byte_idx = 0;
    u_int32_t val = 0;
    uint pow;
    while (byte_idx < len)
    {
        for (int i = 0; i < 4; ++i)
            val = val * 256 + data[byte_idx++];
        pow = 52200625;//85*85*85*85
        for (int i = 0; i < 5; i++)
            encoded_data[char_idx++] = z85_charset[val / pow % 85], pow /= 85;
        val = 0;
    }
    assert(char_idx == num_chars);
    encoded_data[char_idx] = 0;

    return encoded_data;
}

byte *z85_decode(char *encoded_data)
{
    assert(strlen(encoded_data)%5==0);
    size_t num_bytes = strlen(encoded_data) * 4 / 5;
    byte *data = (byte *)malloc(num_bytes);

    uint byte_idx = 0;
    uint char_idx = 0;
    u_int32_t val = 0;
    uint pow;

    while (char_idx < strlen(encoded_data))
    {
        for (int i = 0; i < 5; ++i)
            val = val * 85 + z85_decoding_table[(byte)encoded_data[char_idx++] - 33];
        pow = 16777216;//256*256*256
        for (int i = 0; i < 4; ++i)
            data[byte_idx++] = val / pow % 256, pow /= 256;
        val = 0;
    }
    assert(byte_idx == num_bytes);
    return data;
}