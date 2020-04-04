#include "entities.hpp"

namespace ABM
{
    double Agent::Get_c() const
    {
        return 
    }

    double Agent::Get_I() const
    {
        return Health == HealthCat::Infected? 1. : 0.;
    }
}