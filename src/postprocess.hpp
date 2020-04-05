#include <iostream>

#include "entities.hpp"


#pragma once


// map parameters
#define LL_X		2485487.6	// x coordinate of the lower left corner of the picture
#define LL_Y		1075275.4	// y coordinate of the lower left corner of the picture
#define UR_X		2833806.0	// x coordinate of the upper right corner of the picture
#define UR_Y		1295931.0	// y coordinate of the upper right corner of the picture
#define MAP_WIDTH	3483		// one bin for 100m of map to be covered (2833806.0-2485487.6)/100
#define MAP_HEIGHT 	2207		// one bin for 100m of map to be covered (1295931.0-1075275.4)/100

// picture parameters
#define PIC_WIDTH 	640
#define PIC_HEIGHT 	480

namespace ABM
{



	// Input int array[hight][width][4] with data for each geographical location (hight, width)
	// that gives the Susceptible, Exposed, Infected and Recovered on the 4 channels
	// Side effect: genereate heat map png from the given map.
	void create_heatmap(int*** infection_map, int date, std::string name);

	// Input: population, municipality vector indexed by BfsIds and int for date
	// Side effect: creates a folder with date as name and containing all heat maps,
	// and possibly a jason with further data to be desplayed on the website.
    void vizualize(Population& population,  int date);
}