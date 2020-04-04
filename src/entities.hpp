#include <cinttypes>
#include <vector>
#include <string>

#pragma once

typedef uint32_t index_t;
typedef uint16_t category_t;

namespace ABM
{

    enum AgeCat { Minor, Adult, Old};
    enum HealthCat { Susceptible, Exposed, Infected, Recovered};

    class Household;
    class Workplace;

    struct Agent
    {
        index_t Household;
        index_t Workplace;
        AgeCat Age;
        HealthCat Health;
    };

    struct Household
    {
        index_t Municipality;
    };

    struct Workplace
    {
        index_t Municipality;
    };

    struct Municipality
    {
        index_t BfsId;
        std::string Name;
        std::pair<double> Coordinates;
     
    };

    class Population
    {
        private:

        public:
        std::vector<Agent> Agents;
        std::vector<Household> Households;
        std::vector<Workplace> Workplaces;

        // Initilize class and fill Agents, Households and Workplaces see Figure 2 in "Additional file"
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