#ifndef __RNG_H__
#define __RNG_H__

long r_state = 1103527590;

void rseed(long seed);
long lcg();
float randf(float min, float max);

#endif