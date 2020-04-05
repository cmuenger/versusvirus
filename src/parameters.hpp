#pragma once

#include <iostream>
#include <vector>

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

        double phi_workplace;


        double a_dist;
        double b_dist;
        double cutoff;

        double latency_period;
        double infection_period;

        double symptomaticPercentage;

        double dt;

        Parameters();

    };
}