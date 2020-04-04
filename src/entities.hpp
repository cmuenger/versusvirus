#include <cinttypes>
#include <vector>

#pragma once

typedef uint32_t index_t;
typedef uint16_t category_t;

namespace ABM
{

    enum Age { Minor, Adult, Old};
    enum Health { Susceptible, Exposed, Infected, Recovered};

    class Household;
    class Workplace;

    struct Agent
    {
        index_t Household;
        index_t Workplace;
        Age Age;
        Health Health;
    };

    struct Household
    {
        index_t Municipality;
    };

    struct Workplace
    {
        index_t Municipality;
    };

    std::vector<Agent> generateAgents(const std::vector<double>& data);

    std::vector<Household> generateHouseholds(const std::vector<double>& data);

    std::vector<Workplace> generateWorkplaces(const std::vector<double>& data);

    class Population
    {
        private:

        public:
        std::vector<Agent> Agents;
        std::vector<Household> Households;
        std::vector<Workplace> Workplaces;

        // Initilize class and fill Agents, Households and Workplaces.
        Population(/* data */)
        {}

        void assignAgentsToHouseholds( /* data */);

        void assignAgentsToWorkplaces( /* data */);

        std::vector<index_t> GetAgentsOfHousehold(index_t hhIdx) const;
        std::vector<index_t> GetAgentsOfWorkplace(index_t wpIdx) const;

        Household GetHouseholdOfAgent(index_t agentIdx) const;
        Workplace GetWorkplaecOfAgent(index_t agentIdx) const;
    };
}