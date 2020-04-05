#include <iostream>
#include <tuple>

#include "entities.hpp"


#pragma once


// map parameters
#define LL_X		485488.108	// x coordinate of the lower left corner of the picture
#define LL_Y		75275.093	// y coordinate of the lower left corner of the picture
#define UR_X		833863.805	// x coordinate of the upper right corner of the picture
#define UR_Y		295931.120	// y coordinate of the upper right corner of the picture
#define MAP_WIDTH	3484		// one bin for 100m of map to be covered (2833806.0-2485487.6)/100
#define MAP_HEIGHT 	2207		// one bin for 100m of map to be covered (1295931.0-1075275.4)/100
#define BIN_SIZE	1000

// picture parameters
#define PIC_WIDTH 	640
#define PIC_HEIGHT 	480

#define LOW_PASS_X  40
#define LOW_PASS_Y	40

#define ALPHA 		200

namespace ABM
{

	// Input: data for the 26 cantons on the number of each kind of health states
	// Side effect, creates a json containing all data
	void create_json(const int data[26*4], std::string name);

	// Input: number of different type of people on pixel
	// Output: rgb values for the pixel
	std::tuple<int,int,int> get_color(int susceptible, int exposed, int infected, int recovered);


	// Input: int array[hight][width][4] with data for each geographical location (hight, width)
	// that gives the Susceptible, Exposed, Infected and Recovered on the 4 channels
	// Side effect: genereate heat map png from the given map.
	void create_heatmap(int*** infection_map, std::string name);

	// Input: population, municipality vector indexed by BfsIds and int for date
	// Side effect: creates a folder with date as name and containing all heat maps,
	// and possibly a jason with further data to be desplayed on the website.
    void vizualize(Population& population,  int date, std::map<index_t,index_t> map);
}