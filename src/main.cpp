#include <iostream>

#include "parameters.hpp"
#include "entities.hpp"
#include "simulation.hpp"
#include "postprocess.hpp"
#include "import.hpp"
#include "util.hpp"

using namespace ABM;

int main(int argc, char** argv)
{

    CommandLineInterface cli(argc, argv);
    cli.ParseArgs();
    // -------------------------------------------------------
    // perform initialisation

    std::vector<HelpPopulation> pop;
    std::vector<Commuter> commuters;
    std::vector<HelpHousehold> households;
    std::vector<HelpCoordinates> coordinates;
    std::map<index_t,index_t> bfsIdToIdx_map;
    std::vector<index_t> id;

    std::tie(pop,id,bfsIdToIdx_map) = ABM::importHelpPopulation();
    coordinates = ABM::importMunicipalies();

    commuters = ABM::importCommuter();

    households = ABM::importHouseholds();

    Population p;

    p.createMunicipalities(pop,commuters,coordinates,bfsIdToIdx_map);

    std::cout<<p.Municipalities.size()<<std::endl;
    int sum =0;
    for(const auto& m : p.Municipalities)
    {
        sum+=m.MaxWorker;
    }
    std::cout<<"Workforce:"<<sum<<std::endl;

    p.createWorkplaces();

    std::cout<<p.Workplaces.size()<<std::endl;

    std::vector<std::tuple<index_t, index_t, index_t, index_t>> histo;
    
    for(const auto& m : p.Municipalities)
    {

        histo.push_back(std::make_tuple(0,0,0,0));
    }

    for(const auto& w : p.Workplaces)
    {
        if(w.Size == WorkCat::Global)
            std::get<0>(histo[w.Municipality])++;
        if(w.Size == WorkCat::Large)
            std::get<1>(histo[w.Municipality])++;
        if(w.Size == WorkCat::Medium)
            std::get<2>(histo[w.Municipality])++;
        if(w.Size == WorkCat::Small)
            std::get<3>(histo[w.Municipality])++;
    }


    for(int i=0; i<histo.size(); i++)
    {
        std::cout<<i<<" "<<p.Municipalities[i].MaxWorker<<" "
                 <<std::get<0>(histo[i])<<" "
                  <<std::get<1>(histo[i])<<" "
                   <<std::get<2>(histo[i])<<" "
                    <<std::get<3>(histo[i])<<" "
                    <<p.Municipalities[i].NWorkplaces<<std::endl;
    }

    p.createHouseholds(pop,households);

    p.createLookUpTableForWorkplaces();
    p.createLookUpTableForAgents(bfsIdToIdx_map);

    p.assignAgentsToWorkplaces();


    std::vector<index_t> work_class(4,0);
    int n=0;
    for(const auto& a : p.Agents)
    {
        if(a.Workplace != ~0)
        {
            
            if(p.Workplaces[a.Workplace].Size == WorkCat::Small)
            {
                work_class[0]++;
            }
            else if(p.Workplaces[a.Workplace].Size == WorkCat::Medium)
            {
                work_class[1]++;
            }
            else if(p.Workplaces[a.Workplace].Size == WorkCat::Large)
            {
                work_class[2]++;
            }
            else if(p.Workplaces[a.Workplace].Size == WorkCat::Global)
            {
                work_class[3]++;
            }
            n++;
        }
    }

    std::cout<<n<<" "<<work_class[0]<<" "<<work_class[1]<<" "<<work_class[2]<<" "<<work_class[3]<<std::endl;
    
    
    Parameters parameters;

    std::cout << "starting distance initialisation..." << std::endl;
    p.setupDistanceWeights(p.Municipalities, bfsIdToIdx_map, parameters);
    std::cout << "  -- passed" << std::endl;

    // make patient zero
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<> dis(0, p.Agents.size());
        for(int i = 0; i < cli.getPatientZeros(); i++)
        {
            size_t idx = dis(rng);
            p.Agents[idx].Health = HealthCat::Exposed;
        }
    }

    // END TEST

    //---------------------------
    // Step 1
    //   - initialise agents, households, workplaces
    //   - should be precomputed and only read in here
    //   - initialise parameters of model (ideally precomputed as well)
    //   - initialise meta parameters (time, etc) -> eg from command line


    //---------------------------
    // Step 2
    //   - loop over time
    //   - update once per loop


    //---------------------------
    // Step 3
    //   - prep output for web-app
    //   - profit    

    

    parameters.dt = cli.getTimeDelta();

    Buffer bf = initBuffer(p, parameters.alpha);

    //for(int t = 0; t < cli.getTimeHorizon(); t += cli.getTimeDelta())
    for(int t = 0; t < 1; t ++)
    {
        PerformTimeStep(p, parameters, bf);
        // export population for output? <-------------------------
        vizualize(p, t);
    }

};