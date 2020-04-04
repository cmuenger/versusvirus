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