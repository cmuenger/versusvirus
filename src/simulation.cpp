#include "simulation.hpp"
#include <omp.h>


namespace ABM
{
    namespace Rng
    {
        class Device
        {
            public:
            static InnerDevice Gen;
        };

        InnerDevice Device::Gen;
    }

    std::vector<double> HouseholdContribution(const Population& pop, const Parameters& prm, const Buffer& bf)
    {
        std::vector<double> lambda(pop.Agents.size(), 0);
        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for(index_t hhIdx = 0; hhIdx < pop.Households.size(); hhIdx++)
        {
            double contr = 0.;
            for(index_t agentIdx : pop.GetAgentsOfHousehold(hhIdx))
            {
                contr += bf.I[agentIdx]*bf.c[agentIdx];
            }
            for(index_t agentIdx : pop.GetAgentsOfHousehold(hhIdx))
            {
                lambda[agentIdx] = (contr - bf.I[agentIdx]*bf.c[agentIdx])*prm.beta_household*bf.nalpha[hhIdx]; 
            }
        }
        return lambda;
    }

    std::vector<double> WorkplaceContribution(const Population& pop, const Parameters& prm, const Buffer& bf)
    {
        std::vector<double> lambda(pop.Agents.size(), 0);
        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for(index_t wpIdx = 0; wpIdx < pop.Workplaces.size(); wpIdx++)
        {
            double contr = 0.;
            for(index_t aIdx : pop.GetAgentsOfWorkplace(wpIdx))
            {
                contr += bf.I[aIdx]*bf.c[aIdx];
            }
            for(index_t aIdx : pop.GetAgentsOfWorkplace(wpIdx))
            {
                lambda[aIdx] = (contr - bf.I[aIdx]*bf.c[aIdx])*prm.beta_workplace*bf.np[wpIdx];
            }
        }

        return lambda;
    }

    std::vector<double> infectionProbability(const Population& pop, const Parameters& prm, const Buffer& bf)
    {
        std::vector<double> prob(pop.Agents.size());
        const std::vector<double> hhcontr = HouseholdContribution(pop, prm, bf);
        const std::vector<double> wpcontr = WorkplaceContribution(pop, prm, bf);

        #ifdef _OPENMP
        #pragma omp parallel for
        #endif
        for(size_t i = 0; i < prob.size(); i++)
        {
            prob[i] = 1.- std::exp( -prm.dt*(hhcontr[i] + wpcontr[i]));
        }

        return prob;
    }

    Buffer initBuffer(const Population& pop, const double alpha)
    {
        Buffer bf;
        const size_t n = pop.Agents.size();
        bf.c = std::vector<double>(n);
        bf.I = std::vector<double>(n);
        bf.nalpha = std::vector<double>(pop.Households.size());
        bf.np = std::vector<double>(pop.Workplaces.size());

        for(size_t i = 0; i < n; i++)
        {
            bf.c[i] = pop.Agents[i].Get_c();
            bf.I[i] = pop.Agents[i].Get_I();
        }

        for(size_t i = 0; i < bf.nalpha.size(); i++)
        {
            bf.nalpha[i] = 1./std::pow( pop.GetAgentsOfHousehold(i).size(), alpha);
        }

        for(size_t i = 0; i < bf.np.size(); i++)
        {
            bf.np[i] = 1./pop.GetAgentsOfWorkplace(i).size();
        }
        return bf;
    }

    void PerformTimeStep(Population& population, Parameters& parameters, Buffer& bf)
    {

        std::vector<double> prob = infectionProbability(population, parameters, bf);
        
        for(size_t aIdx = 0; aIdx < population.Agents.size(); aIdx++)
        {
            Agent agent = population.Agents[aIdx];
            switch(agent.Health)
            {
                case HealthCat::Susceptible:
                {
                    if(Rng::Device::Gen.Value() < prob[aIdx])
                    {
                        agent.Health = HealthCat::Exposed;
                    }
                    break;
                }
                case HealthCat::Exposed:
                {
                    if(Rng::Device::Gen.Value() < 1/parameters.latency_period*parameters.dt)
                    {
                        agent.Health = HealthCat::Infected;
                        if(Rng::Device::Gen.Value() < parameters.symptomaticPercentage)
                        {
                            agent.HasSymptoms = true;
                        }
                        bf.I[aIdx] = agent.Get_I();
                        bf.c[aIdx] = agent.Get_c();
                    }
                    break;
                }
                case HealthCat::Infected:
                {
                    if(Rng::Device::Gen.Value() < 1./parameters.infection_period*parameters.dt)
                    {
                        agent.Health = HealthCat::Recovered;
                        agent.HasSymptoms = false;
                        bf.I[aIdx] = agent.Get_I();
                        bf.c[aIdx] = agent.Get_c();
                    }
                    break;
                }
                case HealthCat::Recovered:
                    break;
                default:
                    // not possible, there are only these 4 states
                    break;
            }
        }
    }
}