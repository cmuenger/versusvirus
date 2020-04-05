#include "entities.hpp"

#include <set>

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

    void Population::setupDistanceWeights(
        std::vector<Municipality>& municipalities, 
        std::map<index_t, index_t>& map, const Parameters& parameters)
    {
        std::vector<std::tuple<size_t, size_t, double>> triples = std::vector<std::tuple<size_t, size_t, double>>();
        // for(size_t aIdx = 0; aIdx < Agents.size(); aIdx++)
        // {
        //     std::cout << "At agent " << aIdx << "/" << Agents.size() << std::endl;
        //     const Municipality& ahh = municipalities[map[Households[Agents[aIdx].Household].Municipality]];
        //     const Municipality& awp = municipalities[map[Workplaces[Agents[aIdx].Workplace].Municipality]];
            
        //     for(size_t bIdx = 0; bIdx < Agents.size(); bIdx++)
        //     {     
        //         const Municipality& bhh = municipalities[map[Households[Agents[bIdx].Household].Municipality]];
        //         const Municipality& bwp = municipalities[map[Workplaces[Agents[bIdx].Workplace].Municipality]];

        //         double dist1 = Dist(ahh.Coordinates, bhh.Coordinates, parameters.a_dist, parameters.b_dist);
        //         double dist2 = Dist(ahh.Coordinates, bwp.Coordinates, parameters.a_dist, parameters.b_dist);
        //         double dist3 = Dist(awp.Coordinates, bhh.Coordinates, parameters.a_dist, parameters.b_dist);
        //         double dist4 = Dist(awp.Coordinates, bwp.Coordinates, parameters.a_dist, parameters.b_dist);
                
        //         double dist = std::min({dist1, dist2, dist3, dist4});
        //         if (dist <= parameters.cutoff)
        //         {
        //             triples.push_back( std::make_tuple(aIdx, bIdx, dist));
        //         }

        //     }
        // }

        DistanceWeights = SparseMatrix(triples, Agents.size());
    }

    void Population::createMunicipalities(std::vector<HelpPopulation> pop, std::vector<Commuter> com, std::map<index_t, index_t> map)
    {
        for(const auto& p : pop)
        {
            Municipality m;
            m.Id = map[p.BfsId];
            m.BfsId = p.BfsId;
            m.NPeople = p.NPeople;
            m.MaxWorker = p.NWorker;
            m.NWorker = 0;
            m.MaxWorkplaces = p.NWorkplaces;
            m.NWorkplaces = 0;
            m.NHouseholds = 0;

            m.commutes_map = std::vector<index_t>(0);
            m.commutes_map = std::vector<index_t>(0);
            
            
            int countCommutes=0;
            //Add commutes
            for(const auto c : com)
            {
                if(m.BfsId == c.BfsId)
                {
                    if(c.WorkId != 7777)
                    {
                        m.commutes_map.push_back(map[c.WorkId]);
                        m.commutes.push_back(c.NCommuters);
                        countCommutes++;
                    }
                }
            }
            m.Ncommutes = countCommutes;

            Municipalities.push_back(m);
        }
    }

    void Population::createHouseholds(std::vector<HelpPopulation> pop, std::vector<HelpHousehold> house)
    {
        index_t countHousehold=0;
        for(auto& m : Municipalities)
        {
            const auto& itp = std::find_if(pop.begin(), pop.end(),[&](const auto& val){ return val.BfsId == m.BfsId; }  );
            if(itp == pop.end())
            {
                std::cerr<<"Pop data not found"<<std::endl;
                continue;
            }
           

            multimodal_distribution multimodal_adult({(index_t)std::round(itp->pMiddle*itp->NPeople/100), (index_t)std::round(itp->pOld*itp->NPeople/100)});
            multimodal_distribution multimodal_age({(index_t)std::round(itp->pYoung*itp->NPeople/100),(index_t)std::round(itp->pMiddle*itp->NPeople/100), (index_t)std::round(itp->pOld)});
            
            const auto& it = std::find_if(house.begin(), house.end(),[&](const auto& val){ return val.BfsId == m.BfsId; }  );
            if(it == house.end())
            {
                std::cerr<<"House data "<<m.BfsId<<"not found"<<std::endl;
                continue;
            }

            m.NHouseholds = it->NHouseholds;

            auto getAdultAge =[&] () {
                index_t cat = multimodal_adult.sample();
                if(cat == 0)
                    return AgeCat::Adult;
                else
                    return AgeCat::Old;
            };

            auto getAge =[&] () {
                index_t cat = multimodal_age.sample();
                if(cat == 0)
                    return AgeCat::Minor;
                else if(cat == 1)
                    return AgeCat::Adult;
                else
                    return AgeCat::Old;
            };

            //Create Single Households
            for(int i=0; i< it->P1; i++)
            {
                //Setup household
                Household h;
           
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                 a.Municipality = m.BfsId;
                a.Age = getAdultAge();

                Agents.push_back(a);

                countHousehold++;
            }

            //Create Double Households
            for(int i=0; i< it->P2; i++)
            {
                //Setup household
                Household h;
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                 a.Municipality = m.BfsId;
                a.Age = getAdultAge();
                Agents.push_back(a);
                
                for(int i=0; i<1; i++)
                {
                    Agent a;
                    a.Household = countHousehold;
                    a.Health = HealthCat::Susceptible;
                    a.HasSymptoms = false;
                     a.Municipality = m.BfsId;
                    a.Age = getAge();
                    Agents.push_back(a);
                }

                 countHousehold++;
            }


            //Create Triple Households
            for(int i=0; i< it->P3; i++)
            {
                //Setup household
                Household h;
              
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                 a.Municipality = m.BfsId;
                a.Age = getAdultAge();
                Agents.push_back(a);

                for(int i=0; i<2; i++)
                {
                   Agent a;
                    a.Household = countHousehold;
                    a.Health = HealthCat::Susceptible;
                    a.HasSymptoms = false;
                     a.Municipality = m.BfsId;
                    a.Age = getAge();
                    Agents.push_back(a);
                }

                  countHousehold++;
            }

            //Create Quadruple Households
            for(int i=0; i< it->P4; i++)
            {
                //Setup household
                Household h;
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                 a.Municipality = m.BfsId;
                a.Age = getAdultAge();

                Agents.push_back(a);

                for(int i=0; i<3; i++)
                {
                   Agent a;
                    a.Household = countHousehold;
                    a.Health = HealthCat::Susceptible;
                    a.HasSymptoms = false;
                     a.Municipality = m.BfsId;
                    a.Age = getAge();

                    Agents.push_back(a);
                }

                countHousehold++;
            }


             //Create Quintett Households
            for(int i=0; i< it->P5; i++)
            {
                //Setup household
                Household h;
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                 a.Municipality = m.BfsId;
                a.Age = getAdultAge();

                Agents.push_back(a);

                for(int i=0; i<4; i++)
                {
                   Agent a;
                    a.Household = countHousehold;
                    a.Health = HealthCat::Susceptible;
                    a.HasSymptoms = false;
                     a.Municipality = m.BfsId;
                    a.Age = getAge();

                    Agents.push_back(a);
                } 
                countHousehold++;
            }

            //Create Sextett Households
            for(int i=0; i< it->P6; i++)
            {
                //Setup household
                Household h;
                h.Municipality = m.BfsId;
                Households.push_back(h);

                //Setup inhabitants
                Agent a;
                a.Household = countHousehold;
                a.Health = HealthCat::Susceptible;
                a.HasSymptoms = false;
                a.Municipality = m.BfsId;
                a.Age = getAdultAge();

                Agents.push_back(a);

                for(int i=0; i<5; i++)
                {
                   Agent a;
                    a.Household = countHousehold;
                    a.Workplace = ~0;
                    a.Health = HealthCat::Susceptible;
                    a.HasSymptoms = false;
                     a.Municipality = m.BfsId;
                    a.Age = getAge();

                    Agents.push_back(a);
                }
                 countHousehold++;
            }
        }

        std::cout<<countHousehold<<" Households created"<<std::endl;
    }

    void Population::createWorkplaces()
    {
         uniform_distribution uniform;

        std::vector<int> company_min_size = {250,50,10,1};
        std::vector<int> n_company = {1564, 8909, 48848, 519216};

        index_t maxWorker =0;
        for(const auto& m : Municipalities)
        {
            maxWorker = maxWorker < m.MaxWorker ? m.MaxWorker : maxWorker;
        }

        for(int c=0; c<company_min_size.size();c++ )
        {
            index_t M = Municipalities.size();
            std::cout<<n_company[c]<<std::endl;
            for(int n =0; n<n_company[c]; n++)
            {
              
                while(true)
                {
                    index_t m_idx = uniform.sample_int(0, M);
                    //std::cout<<m_idx<<std::endl;
                    //check if still enough empty workplace exist;
                      if(Municipalities[m_idx].NWorker > Municipalities[m_idx].MaxWorker - company_min_size[c]) continue;
                    //check if still enough worker are available;
                    if(Municipalities[m_idx].NWorkplaces > Municipalities[m_idx].MaxWorkplaces ) continue;

                    //Rejection sampling
                    float p = uniform.sample();
                    //Accept
                    //(std::cout<<p<<" "<<Municipalities[m_idx].MaxWorker/(float)(company_min_size[c]*M)<<std::endl;
                    if(p <  Municipalities[m_idx].MaxWorker/(float)(company_min_size[c]*M))
                    {
                        Workplace w;
                        w.Municipality = m_idx; //Municipalities[m_idx].BfsId;
                        w.Size =  c==0 ? WorkCat::Global : (c==1 ? WorkCat::Large : (c==2 ? WorkCat::Medium : WorkCat::Small));
                        w.MinWorker = company_min_size[c];
                        Workplaces.push_back(w);

                        Municipalities[m_idx].NWorker+=company_min_size[c];
                        Municipalities[m_idx].NWorkplaces += 1;

                        //std::cout<<"accept"<<std::endl;
                        break;
                    }
                    else
                    {
                        //std::cout<<"reject"<<std::endl;
                    }
                    
                }
            }

        }

        //Reset worker since they will be assign later;
        for(auto& m : Municipalities)
        {
            m.NWorker = 0;
        }
    }

    void Population::assignAgentsToWorkplaces()
    {
        uniform_distribution uniform;

        //Loop over Municpaltiy. 
        for(const auto& m : Municipalities)
        {   
          
            if(m.commutes.size() == 0) continue;
            multimodal_distribution multimodal(m.commutes);
            std::vector<index_t> workforce = GetWorkforceOfMunicipality(m.Id);

            std::set<index_t> workforce_set;
            for(int i=0; i<workforce.size(); i++)
            {
                workforce_set.insert(i);
                
            }
            
            int c =0;
            while(c< m.Ncommutes && !workforce_set.empty())
            {
                std::set<index_t>::iterator it;
                index_t w_idx = uniform.sample_int(0, workforce.size());
                while(workforce_set.find(w_idx) == workforce_set.end())
                {
                    w_idx = uniform.sample_int(0, workforce.size());
                }
                it = workforce_set.find(w_idx);
      
            
                c++;
            
        
                //Pick a commute. Multimodal distribution of commutes by size
                index_t c_idx = m.commutes_map[multimodal.sample()];        
                    
                //TODO sample cantons at random for unspecifed commutes
                //std::cout<<c_idx<<std::endl;
                //Get workplaces of Commute city
                std::vector<index_t> w_indices = GetWorkplacesOfMunicipality(c_idx);
                index_t W = w_indices.size();

                
                //std::cout<<W<<" "<<w_indices.size()<<std::endl;
                //Pick a workplace rejection sampling over distibution (upper bound for large companies 500); check if the still have open positions else reject
                    index_t b = workforce[*it];
                     workforce_set.erase(it);
                while(true)
                {
                    index_t w_idx = w_indices[uniform.sample_int(0, W)];

                    float p = uniform.sample();
                    //std::cout<<p<<" "<<Workplaces[w_idx].MinWorker/1000.0<<std::endl;
                    //Accept
                    if(p <   (Workplaces[w_idx].MinWorker/1000.0) )//1000 Upper bound for large companies
                    {
                        Agents[b].Workplace = w_idx;
                        Workplaces[w_idx].NWorker++;
                        break;
                    }
                }
               
            

            }
        }

        std::cout<<"Assigned Workers"<<std::endl;
        
    }

    void Population::createLookUpTableForAgents(std::map<index_t,index_t> map)
    {
        AgentsOfHousehold.resize(Households.size());
        AgentsOfWorkplace.resize(Workplaces.size());
        AgentsOfMunicipality.resize(Municipalities.size());

        for(int  i=0; i<Agents.size(); i++)
        {
           
            AgentsOfHousehold[Agents[i].Household].push_back(i);
            if(Agents[i].Workplace != ~0)
                AgentsOfWorkplace[Agents[i].Workplace].push_back(i);
            AgentsOfMunicipality[map[Agents[i].Municipality]].push_back(i);
        }
    }


    void Population::createLookUpTableForWorkplaces()
    {
        WorkplacesOfMunicipality.resize(Municipalities.size());
        
        for(int i=0; i<Workplaces.size(); i++)
        {
            WorkplacesOfMunicipality[Workplaces[i].Municipality].push_back(i);
        }
    }

    std::vector<index_t> Population::GetWorkplacesOfMunicipality(index_t c_idx) const
    {   
        if(c_idx >= WorkplacesOfMunicipality.size())
        {
            std::cerr<<c_idx<<std::endl;
            exit(-1);
        }
        return WorkplacesOfMunicipality[c_idx];
    }

    std::vector<index_t> Population::GetWorkforceOfMunicipality(index_t c_idx) const
    {
        
        std::vector<index_t> workforce;
        for(const auto& a : AgentsOfMunicipality[c_idx])
        {
            if(Agents[a].Age == AgeCat::Adult);
                workforce.push_back(a);
        }
        return workforce;
    }


    std::vector<index_t> Population::GetAgentsOfHousehold(index_t hhIdx) const
    {
        return AgentsOfHousehold[hhIdx];
        
    }

    std::vector<index_t> Population::GetAgentsOfWorkplace(index_t wpIdx) const
    {
        return AgentsOfWorkplace[wpIdx];
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
        if(adults.size()>0)
            return adults[uniform.sample_int(0, adults.size())];
        else
            return ~0;
        
    }
}