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

    void Population::setupDistanceWeights(std::vector<Municipality>& municipalities, const Parameters& parameters)
    {
        std::vector<std::tuple<size_t, size_t, double>> triples = std::vector<std::tuple<size_t, size_t, double>>();
        for(size_t aIdx = 0; aIdx < Agents.size(); aIdx++)
        {
            const Municipality& ahh = municipalities[Households[Agents[aIdx].Household].Municipality];
            const Municipality& awp = municipalities[Workplaces[Agents[aIdx].Workplace].Municipality];
            
            for(size_t bIdx = 0; bIdx < Agents.size(); bIdx++)
            {     
                const Municipality& bhh = municipalities[Households[Agents[bIdx].Household].Municipality];
                const Municipality& bwp = municipalities[Workplaces[Agents[bIdx].Workplace].Municipality];

                double dist1 = Dist(ahh.Coordinates, bhh.Coordinates, parameters.a_dist, parameters.b_dist);
                double dist2 = Dist(ahh.Coordinates, bwp.Coordinates, parameters.a_dist, parameters.b_dist);
                double dist3 = Dist(awp.Coordinates, bhh.Coordinates, parameters.a_dist, parameters.b_dist);
                double dist4 = Dist(awp.Coordinates, bwp.Coordinates, parameters.a_dist, parameters.b_dist);
                
                double dist = std::min({dist1, dist2, dist3, dist4});
                if (dist <= parameters.cutoff)
                {
                    triples.push_back( std::make_tuple(aIdx, bIdx, dist));
                }

            }
        }

        DistanceWeights = SparseMatrix(triples);
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