#include "postprocess.hpp"

namespace ABM
{

	void create_heatmap(int*** infection_map, int date, std::string name){
		return;
	}

	// Input: population, municipality vector indexed by BfsIds and int for date
	// Side effect: creates a folder with date as name and containing all heat maps,
	// and possibly a jason with further data to be desplayed on the website.
    void vizualize(Population& population, int date){

    	// create output map to store values tripple * for 3 dimenstional array
    	int*** household_map= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** workplace_map= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** young_map 	= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** middle_map 	= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));
    	int*** old_map 		= (int***) calloc(4*MAP_WIDTH*MAP_HEIGHT,sizeof(int));

    	// some shortcuts to make it more readable
    	const std::vector<Household>& households = population.Households;
    	const std::vector<Workplace>& workplaces = population.Workplaces;
    	const std::vector<Municipality>& municipalities = population.Municipalities;
    	const std::vector<Agent>& agents = population.Agents;

    	// fill all maps with the number of people
    	// go through all households and fill de general household map as well as all edges
    	for(Agent agent_iter : agents){
    		// get workplace and Household coordinates
    		index_t house_m = households[agent_iter.Household].Municipality;
    		index_t workp_m = workplaces[agent_iter.Workplace].Municipality;

    		// get coordinates for map to access
			std::pair<double,double> house_coord = municipalities[house_m].Coordinates;
			std::pair<double,double> workp_coord = municipalities[workp_m].Coordinates;

			// get to array indexes
			int house_x_coord = std::round(house_coord.first - LL_X);
			int house_y_coord = std::round(house_coord.second - LL_Y);
			int workp_x_coord = std::round(workp_coord.first - LL_X);
			int workp_y_coord = std::round(workp_coord.second - LL_Y);

			// find health
			int health;
			switch(agent_iter.Health){
				case Susceptible 	: health = 0; break;
				case Exposed 		: health = 1; break;
				case Infected 		: health = 2; break;
				case Recovered 		: health = 3; break;
			}

			// increment all maps
			household_map[house_x_coord][house_y_coord][health] += 1;
			workplace_map[workp_x_coord][workp_y_coord][health] += 1;
			switch(agent_iter.Age){
				case Minor 	: young_map[house_x_coord][house_y_coord][health] += 1;
				case Adult 	: middle_map[house_x_coord][house_y_coord][health] += 1;
				case Old 	: old_map[house_x_coord][house_y_coord][health] += 1;
			}
    	}

    	create_heatmap(household_map, date, "house");
    	create_heatmap(workplace_map, date, "work");
    	create_heatmap(young_map, date, "young");
    	create_heatmap(middle_map, date, "middle");
    	create_heatmap(old_map, date, "old");

    	free(household_map);
    	free(workplace_map);
    	free(young_map);
    	free(middle_map);
    	free(old_map);

    }
}