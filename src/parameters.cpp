#include "parameters.hpp"

namespace ABM
{
    Parameters::Parameters()
    : alpha(0.8), a_dist(3.8), b_dist(2.32), phi_workplace(0.2),
    beta_household(1/2.), beta_workplace(1./3), beta_commuting(1./5),
    symptomaticPercentage(0.67),
    latency_period(2), infection_period(8),
    dt(1) {}
}