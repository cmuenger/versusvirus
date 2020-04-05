#include <vector>

#include "import.hpp"
#include "entities.hpp"
#include "util.hpp"

#pragma once

namespace ABM
{
    std::vector<Agent> initPopulation();

    std::vector<Agent> initHouseholds(
            index_t baseHausholdIdx,
            HelpPopulation townPopData,
            std::vector<size_t> numHouseholdOfSize
    );
}