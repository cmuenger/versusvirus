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

    std::cout<<households[3].NHouseholds<<std::endl;

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