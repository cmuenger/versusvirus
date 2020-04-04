#include "entities.hpp"

namespace ABM
{
    namespace Rng
    {
        class Device
        {
            public:
            static InnerDevice Rng;

        };
    }



    double Agent::Get_c(const double symptomaticCases) const
    {
        if (Health == HealthCat::Infected && HasSymptoms) return 2.;
        // if (Health == HealthCat::Infected)
        // {
        //     if( Rng::Device::Rng.Value() < symptomaticCases)
        //     {
        //         return 2.;
        //     }
        // }

        return 1.;
    }

    double Agent::Get_I() const
    {
        return Health == HealthCat::Infected? 1. : 0.;
    }
}