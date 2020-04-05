#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <random>

#include "util.hpp"
#include "parameters.hpp"


typedef size_t index_t;

namespace ABM
{

    enum AgeCat { Minor, Adult, Old};
    enum HealthCat { Susceptible, Exposed, Infected, Recovered};
    enum WorkCat : int { Global , Large, Medium, Small};

    struct Agent
    {
        index_t Household;
        index_t Workplace;
        AgeCat Age;
        HealthCat Health;
        bool HasSymptoms;

        double Get_c(const double symptomaticCases) const;
        double Get_I() const;


    };

    struct Household
    {
        index_t Municipality;
    };

    struct Workplace
    {
        index_t Municipality;
        index_t NWorker;
        index_t MinWorker;
        WorkCat Size;
        
    };

    struct Municipality
    {
        index_t BfsId;
        std::string Name;
        std::pair<double,double> Coordinates;
        index_t Plz;
        index_t NHouseholds;
        index_t NWorker;
        index_t NWorkplaces;
        index_t MaxWorker;
        index_t MaxWorkplaces;
        index_t Ncommutes;
        std::vector<index_t> commutes;
        std::vector<index_t> commutes_map;
     
    };

    struct HelpHousehold
    {
        index_t BfsId;
        index_t NHouseholds;
        index_t P1;
        index_t P2;
        index_t P3;
        index_t P4;
        index_t P5;
        index_t P6;
    };

    struct HelpPopulation
    {
        index_t BfsId;
        index_t NPeople;
        double  pYoung;
        double  pMiddle;
        double  pOld;
        index_t NHouseholds;
        index_t NWorker;
        index_t NWorkplaces;
    };

    struct Commuter
    {
        index_t BfsId;
        index_t WorkId;
        index_t CantonId;
        index_t NCommuters;
    };


    class Population
    {
        private:

        void setupDistanceWeights(std::vector<Municipality>& municipalities, const Parameters& parameters);

        public:
        SparseMatrix DistanceWeights;
        std::vector<Agent> Agents;
        std::vector<Household> Households;
        std::vector<Workplace> Workplaces;
        std::vector<Municipality> Municipalities;



        // Initilize class and fill Agents, Households and Workplaces see Figure 2 in "Additional file"
        Population(/* data */)
        {}

        void assignAgentsToHouseholds( /* data */);

        void assignAgentsToWorkplaces( /* data */);

        void createWorkplaces();


        std::vector<index_t> GetAgentsOfHousehold(index_t hhIdx) const;
        std::vector<index_t> GetAgentsOfWorkplace(index_t wpIdx) const;

        Household GetHouseholdOfAgent(index_t agentIdx) const;
        Workplace GetWorkplaceOfAgent(index_t agentIdx) const;

        index_t GetRandomAdultOfHousehold(index_t hIdx) const;

        std::vector<index_t> GetWorkplacesOfMunicipality(index_t cIdx) const;
    };
}