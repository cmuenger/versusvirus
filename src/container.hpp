#include <iostream>
#include <iostream>

namespace ABM
{
    template<typename Agent_t, typename Household_t, typename Workplace_t>
    struct Container
    {
        // maybe provide additional hashmaps or vectors for
        // efficient access
        std::vector<Agent_t> Agents;
        std::vector<Household_t> Households;
        std::vector<Workplace_t> Workplaces;
    }
}