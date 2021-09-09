#include "rng.h"

void rseed(long seed)
{
    r_state = seed;
}
long lcg()
{
    r_state = (0x41c64e6d * r_state + 0x3039) % 0x80000000;
    return r_state;
}
float randf(float min, float max)
{
    return min + (max - min) * (((float)lcg()) / 0x7fffffff);
}