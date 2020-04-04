#include "entities.hpp"

namespace ABM
{
    namespace Rng
    {
        class InnerDevice
        {
            private:
            std::random_device _dev;
            std::mt19937 _rng;
            std::uniform_real_distribution<> _dist;

            public:

            InnerDevice() : _rng(std::mt19937(_dev())) {}

            double Value()
            {
                return _dist(_rng);
            }
        }; 

        class Device
        {
            public:
            static InnerDevice Rng;

        };
    }



    double Agent::Get_c(const double symptomaticCases) const
    {
        if (Health == HealthCat::Infected)
        {
            if( Rng::Device::Rng.Value() < symptomaticCases)
            {
                return 2.;
            }
        }

        return 1.;
    }

    double Agent::Get_I() const
    {
        return Health == HealthCat::Infected? 1. : 0.;
    }
}