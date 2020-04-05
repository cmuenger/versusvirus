#include "simulation.hpp"

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

    

    void PerformTimeStep(Population& population, Parameters& parameters)
    {
        // update all lambdas
        for(index_t agentIdx = 0; agentIdx < population.Agents.size(); agentIdx++)
        {
            Agent agent = population.Agents[agentIdx];
            switch(agent.Health)
            {
                case HealthCat::Susceptible:
                {
                    const double l = lambda(agentIdx, population, parameters);
                    const double p = 1. - std::exp(-l*parameters.dt);
                    if(Rng::Device::Gen.Value() < p)
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
                    }
                    break;
                }
                case HealthCat::Infected:
                {
                    if(Rng::Device::Gen.Value() < 1./parameters.infection_period*parameters.dt)
                    {
                        agent.Health = HealthCat::Recovered;
                    }
                    break;
                }
                case HealthCat::Recovered:
                    break;
                default:
                    // not possible, there are only these 4 states
                    break;
            }
        };
    }

    double lambda(const index_t agentIdx, const Population& population, const Parameters& parameters)
    {
        double l = 0;
        const Agent agent = population.Agents[agentIdx];

        // sum over all household members
        double tmp = 0.;
        for(index_t residentIdx : population.GetAgentsOfHousehold(population.Agents[agentIdx].Household))
        {
            if(residentIdx == agentIdx) continue; //agent cannot infect himself
            const Agent resident = population.Agents[residentIdx];
            
            tmp += resident.Get_I()*resident.Get_c(parameters.symptomaticPercentage)*parameters.beta_household;
        }
        const double nha = std::pow( population.GetAgentsOfHousehold(agent.Household).size(), parameters.alpha);
        l += tmp/nha;

        // sum over all coworkers
        tmp = 0.;
        for(index_t coworkerIdx : population.GetAgentsOfWorkplace(population.Agents[agentIdx].Workplace))
        {
            if(coworkerIdx == agentIdx) continue; //agent cannot infect himself
            const Agent coworker = population.Agents[coworkerIdx];
            
            // todo: distinct between different workplaces
            tmp += coworker.Get_I()*coworker.Get_c(parameters.symptomaticPercentage)*parameters.beta_workplace*parameters.phi_workplace[0];
        }
        const double npj = population.GetAgentsOfWorkplace(agent.Workplace).size();
        l += tmp/npj;

        // sum over random interactions -> step through entire row in sparse matrix
        tmp = 0;
        double normalizingConstant = 0;
        for(size_t mIdx = population.DistanceWeights.RowIdx[agentIdx]; 
            mIdx < population.DistanceWeights.RowIdx[agentIdx+1]; mIdx++)
        {
            index_t rndPersonIdx = population.DistanceWeights.ColumnIdx[mIdx];
            if (rndPersonIdx == agentIdx) continue; // agent cannot infect himself
            Agent rndPerson = population.Agents[rndPersonIdx];
            tmp += rndPerson.Get_I()*rndPerson.Get_c(parameters.symptomaticPercentage)*parameters.beta_commuting
                        *population.DistanceWeights.Values[rndPersonIdx];
            normalizingConstant += population.DistanceWeights.Values[rndPersonIdx];
        }
        l += tmp/normalizingConstant;

        return l;
    }
}