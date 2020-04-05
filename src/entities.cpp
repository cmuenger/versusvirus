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

    void Population::createWorkplaces()
    {
         uniform_distribution uniform;

        std::vector<int> company_min_size = {250,5,10,1};
        std::vector<int> n_company = {1564, 8909, 48848, 519216};

        index_t maxWorker =0;
        for(const auto& m : Municipalities)
        {
            maxWorker = maxWorker < m.MaxWorker ? m.MaxWorker : maxWorker;
        }

        for(int c=0; c<company_min_size.size();c++ )
        {
            index_t M = Municipalities.size();
            
            while(true)
            {
                index_t m_idx = uniform.sample_int(0, M);

                //check if still enough empty workplace exist;
                if(Municipalities[m_idx].NWorker >= Municipalities[m_idx].MaxWorker - company_min_size[c]) continue;
                //check if still enough worker are available;
                if(Municipalities[m_idx].NWorkplaces >= Municipalities[m_idx].MaxWorkplaces - 1) continue;

                //Rejection sampling
                float p = uniform.sample();
                //Accept
                if(p <  Municipalities[m_idx].MaxWorker/(company_min_size[c]*M))
                {
                    Workplace w;
                    w.Municipality = m_idx;
                    w.Size = WorkCat::Global;

                    Workplaces.push_back(w);

                    Municipalities[m_idx].NWorker+=company_min_size[c];
                    Municipalities[m_idx].NWorkplaces += 1;
                    break;
                }
            }

        }
    }

    void Population::assignAgentsToWorkplaces()
    {
        uniform_distribution uniform;

        //Loop over Municpaltiy. 
        for(const auto& m : Municipalities)
        {   
            index_t a_idx;
            multimodal_distribution multimodal(m.commutes);
            //Pick at random an household. 
            while(true)
            {
                index_t h_idx = uniform.sample_int(0, m.NHouseholds);
                //Pick at random an adult agent
                a_idx = GetRandomAdultOfHousehold(h_idx);
                //Check if he got allready a job
                if(Agents[a_idx].Workplace != ~0)
                {
                    //Pick new household; 
                    continue;
                }
                else
                {
                    break;
                }
            }

            //Pick a commute. Multimodal distribution of commutes by size
            index_t c_idx =  multimodal.sample();        
                
            //TODO sample cantons at random for unspecifed commutes

            //Get workplaces of Commute city
            std::vector<index_t> w_indices = GetWorkplacesOfMunicipality(c_idx);
            index_t W = m.NWorkplaces;
            //Pick a workplace rejection sampling over distibution (upper bound for large companies 500); check if the still have open positions else reject
            while(true)
            {
                 index_t w_idx = w_indices[uniform.sample_int(0, W)];

                float p = uniform.sample();
                //Accept
                if(p <   (Workplaces[w_idx].MinWorker/1000) )//1000 Upper bound for large companies
                {
                  Agents[a_idx].Workplace = w_idx;
                  Workplaces[w_idx].NWorker++;
                  break;
                }
            }
        }
        
    }


    std::vector<index_t> Population::GetAgentsOfHousehold(index_t hhIdx) const
    {
        throw "Not yet implemented";
    }

    std::vector<index_t> Population::GetAgentsOfWorkplace(index_t wpIdx) const
    {
        throw "Not yet implemented";
    }

    index_t Population::GetRandomAdultOfHousehold(index_t hIdx) const
    {
        std::vector<index_t> a = GetAgentsOfHousehold(hIdx);

        std::vector<index_t> adults;
        for(const auto idx : a)
        {
            if(Agents[idx].Age == AgeCat::Adult)
            {
                adults.push_back(idx);
            }
        }

        uniform_distribution uniform;
        return adults[uniform.sample_int(0, adults.size())];
    }
}