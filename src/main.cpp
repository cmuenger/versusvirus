#include <iostream>

#include "parameters.hpp"
#include "entities.hpp"
#include "simulation.hpp"
//#include "postprocess.hpp"
#include "import.hpp"
#include "util.hpp"

using namespace ABM;

int main(int argc, char** argv)
{
    //TEST
    std::vector<HelpPopulation> pop;
    std::vector<Commuter> commuters;
    std::vector<HelpHousehold> households;
    std::map<index_t,index_t> map;
    std::vector<index_t> id;

    std::tie(pop,id,map) = ABM::importHelpPopulation();

    /*//std::cout<<pop.size()<<std::endl;
    std::vector<int> companysize{519215,48848,8909,1564};

    std::vector<int> samples(4,0);

    std::cout<<ABM::sample_multimodal(companysize)<<std::endl;
    for(int i=0;i<580000; i++)
    {
        samples[ABM::sample_multimodal(companysize)]++;
    }

    for(auto i : samples)
    {
        std::cout<<i<<std::endl;
    }*/

    commuters = ABM::importCommuter();

    households = ABM::importHouseholds();

    Population p;

    p.createMunicipalities(pop,commuters,map);

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
    p.createLookUpTableForAgents(map);

    p.assignAgentsToWorkplaces();

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

    CommandLineInterface cli(argc, argv);
    cli.ParseArgs();

    Population population; // init here <--------------------------
    Parameters parameters; // init here <--------------------------

    for(int t = 0; t < cli.getTimeHorizon(); t += cli.getTimeDelta())
    {
        PerformTimeStep(population, parameters);
        // export population for output? <-------------------------
    }

};