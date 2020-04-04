#include <iostream>
#include <vector>

#include "entities.hpp"

#pragma once

namespace ABM
{
    class Parameters
    {
        private:

        public:

        // have here the parameters as std::vectors of double/int
        double alpha;
        double beta_household;
        double beta_workplace;
        double beta_commuting;

        std::vector<double> phi_workplace;


        double a_dist;
        double b_dist;

        double latency_period;
        double infection_period;

        double symptomaticPercentage;

        double dt;

        Parameters(/* data */)
        : alpha(0.8), a_dist(3.8), b_dist(2.32), phi_workplace(std::vector<double>()) {}

    };
}