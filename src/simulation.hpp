#include <iostream>
#include <math.h>
#include "entities.hpp"
#include "parameters.hpp"
#include "util.hpp"

#pragma once

namespace ABM
{
        struct Buffer
    {
        std::vector<double> I; //I_k
        std::vector<double> c; //c_k
        std::vector<double> nalpha; // 1/n_H^alpha
        std::vector<double> np; //1/n_Pij
    };

    Buffer initBuffer(const Population& pop, const double alpha);
    void PerformTimeStep(Population& pop, Parameters& prm, Buffer& bf);
    double lambda(const index_t agentIdx, const Population& population, const Parameters& parameters);
}