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

    void Population::setupDistanceWeights()
    {
        // for(size_t aIdx = 0; aIdx < Agents.size(); aIdx++)
        // {
        //     const Municipality& ahh = Households[Agents[aIdx].Household];
        //     const Workplace& awp = Workplaces[Agents[aIdx].Workplace];

        //     for(size_t bIdx = 0; bIdx < Agents.size(); bIdx++)
        //     {     
        //         const Household& bhh = Households[Agents[bIdx].Household];
        //         const Workplace& bwp = Workplaces[Agents[bIdx].Workplace];

        //         double dist1 = Dist(std::make_pair(Agents[aIdx].))
        //     }
        // }
    }

    std::vector<index_t> Population::GetAgentsOfHousehold(index_t hhIdx) const
    {
        throw "Not yet implemented";
    }

    std::vector<index_t> Population::GetAgentsOfWorkplace(index_t wpIdx) const
    {
        throw "Not yet implemented";
    }
}