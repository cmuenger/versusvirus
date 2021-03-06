#pragma once

#include <cinttypes>
#include <vector>
#include <string>
#include <random>
#include <map>

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
        index_t Municipality;
        AgeCat Age;
        HealthCat Health;
        bool HasSymptoms;

        double Get_c() const;
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
        index_t Id;
        index_t BfsId;
        index_t KId;
        std::string Name;
        std::pair<double,double> Coordinates;
        index_t Plz;
        index_t NPeople;
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
        index_t KId;
        index_t NPeople;
        double  pYoung;
        double  pMiddle;
        double  pOld;
        index_t NHouseholds;
        index_t NWorker;
        index_t NWorkplaces;
    };

    struct HelpCoordinates
    {
        index_t BfsId;
        std::string Name;
        std::pair<double,double> Coordinates;

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

        

        public:
        SparseMatrix DistanceWeights;
        std::vector<Agent> Agents;
        std::vector<Household> Households;
        std::vector<Workplace> Workplaces;
        std::vector<Municipality> Municipalities;

        std::vector<std::vector<index_t>> AgentsOfHousehold;
        std::vector<std::vector<index_t>> AgentsOfWorkplace;
        std::vector<std::vector<index_t>> WorkplacesOfMunicipality;
         std::vector<std::vector<index_t>> AgentsOfMunicipality;



        // Initilize class and fill Agents, Households and Workplaces see Figure 2 in "Additional file"
        Population(/* data */)
        {}

        void setupDistanceWeights(std::vector<Municipality>& municipalities, std::map<index_t, index_t>& map, const Parameters& parameters);

        void assignAgentsToHouseholds( /* data */);

        void assignAgentsToWorkplaces( /* data */);

        void createHouseholds(std::vector<HelpPopulation> pop, std::vector<HelpHousehold> house);        
        void createWorkplaces();
        void createMunicipalities(std::vector<HelpPopulation> pop, std::vector<Commuter> com, std::vector<HelpCoordinates>, std::map<index_t, index_t> map);

        void createLookUpTableForAgents(std::map<index_t,index_t> map);
        void createLookUpTableForWorkplaces();

        std::vector<index_t> GetAgentsOfHousehold(index_t hhIdx) const;
        std::vector<index_t> GetAgentsOfWorkplace(index_t wpIdx) const;

        Household GetHouseholdOfAgent(index_t agentIdx) const;
        Workplace GetWorkplaceOfAgent(index_t agentIdx) const;
        std::vector<index_t> GetWorkforceOfMunicipality(index_t c_idx) const;
        index_t GetRandomAdultOfHousehold(index_t hIdx) const;

        std::vector<index_t> GetWorkplacesOfMunicipality(index_t cIdx) const;
    };
}