#ifndef __SGD_H__
#define __SGD_H__
    typedef struct sgd_optimizer{
        Optimizer* self;
        float lr;
        optimizing_step_function_t step;
    }SGDOptimizer;
#endif