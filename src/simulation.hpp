#include <iostream>
#include "entities.hpp"
#include "parameters.hpp"

#pragma once

namespace ABM
{
    void PerformTimeStep(Population& population, Parameters& parameters);

    void UpdateLambda(const Population& population, Parameters& parameters);

    void UpdateIndividuals(Population& population, Parameters& parameters);
}